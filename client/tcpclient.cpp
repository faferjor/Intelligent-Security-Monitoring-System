#include "tcpclient.h"
#include <QDebug>
#include <QDataStream>
#include <QDateTime>

TcpClient::TcpClient(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);

    connect(socket, &QTcpSocket::connected, this, &TcpClient::onConnected);
    connect(socket, &QTcpSocket::disconnected, this, &TcpClient::onDisconnected);
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred), this, &TcpClient::onErrorOccurred);
    connect(socket, &QTcpSocket::readyRead, this, &TcpClient::onReadyRead);
}

TcpClient::~TcpClient()
{
    if (socket->state() == QAbstractSocket::ConnectedState) {
        socket->disconnectFromHost();
    }
    socket->deleteLater();
}

bool TcpClient::connectToServer(const QString& host, quint16 port)
{
    if (isConnected()) {
        return true;
    }

    serverAddress = host;
    serverPort = port;
    socket->connectToHost(host, port);

    return socket->waitForConnected(3000);
}

void TcpClient::disconnectFromServer()
{
    if (isConnected()) {
        socket->disconnectFromHost();
    }
}

void TcpClient::sendMessage(const QString& message)
{
    if (!isConnected()) {
        return;
    }

    QByteArray data = "MSG:" + message.toUtf8();
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << static_cast<qint64>(data.size());
    out.writeRawData(data.data(), data.size());

    socket->write(block);
    socket->flush();
}

void TcpClient::sendFrame(const QImage& frame)
{
    if (!isConnected() || frame.isNull()) {
        return;
    }

    QByteArray frameData;
    QBuffer buffer(&frameData);
    buffer.open(QIODevice::WriteOnly);
    frame.save(&buffer, "JPG", 80);

    QByteArray data = "FRAME:" + frameData;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << static_cast<qint64>(data.size());
    out.writeRawData(data.data(), data.size());

    socket->write(block);
    socket->flush();
}

void TcpClient::sendDetectionResults(const std::vector<DetectionResult>& results)
{
    if (!isConnected()) {
        return;
    }

    QByteArray resultsData;
    QDataStream resultsStream(&resultsData, QIODevice::WriteOnly);
    resultsStream << static_cast<qint32>(results.size());
    for (const auto& result : results) {
        resultsStream << static_cast<qint32>(result.classId);
        resultsStream << QString::fromStdString(result.className);
        resultsStream << result.confidence;
        resultsStream << static_cast<qint32>(result.bbox.x);
        resultsStream << static_cast<qint32>(result.bbox.y);
        resultsStream << static_cast<qint32>(result.bbox.width);
        resultsStream << static_cast<qint32>(result.bbox.height);
        resultsStream << static_cast<qint32>(result.center.x);
        resultsStream << static_cast<qint32>(result.center.y);
    }

    QByteArray data = "RESULTS:" + resultsData;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << static_cast<qint64>(data.size());
    out.writeRawData(data.data(), data.size());

    socket->write(block);
    socket->flush();
}

void TcpClient::onConnected()
{
    qDebug() << "Connected to server:" << serverAddress << ":" << serverPort;
    emit connected();
}

void TcpClient::onDisconnected()
{
    qDebug() << "Disconnected from server";
    emit disconnected();
}

void TcpClient::onErrorOccurred(QAbstractSocket::SocketError socketError)
{
    QString errorString = socket->errorString();
    qDebug() << "Connection error:" << errorString;
    emit connectionError(errorString);
}

void TcpClient::onReadyRead()
{
    buffer.append(socket->readAll());
    processServerData();
}

void TcpClient::processServerData()
{
    while (buffer.size() >= static_cast<int>(sizeof(qint64))) {
        if (expectedBytes == 0) {
            QDataStream stream(buffer);
            stream >> expectedBytes;
            buffer.remove(0, sizeof(qint64));

            if (expectedBytes <= 0 || expectedBytes > 10 * 1024 * 1024) {
                qDebug() << "Invalid data size received:" << expectedBytes;
                buffer.clear();
                expectedBytes = 0;
                return;
            }
        }

        if (buffer.size() >= expectedBytes) {
            QByteArray data = buffer.left(expectedBytes);
            buffer.remove(0, expectedBytes);
            expectedBytes = 0;

            deserializeServerData(data);
        } else {
            break;
        }
    }
}

void TcpClient::deserializeServerData(const QByteArray& data)
{
    if (data.startsWith("MSG:")) {
        QString message = QString::fromUtf8(data.mid(4));
        emit serverMessageReceived(message);
    } else if (data.startsWith("FRAME:")) {
        int resultsPos = data.indexOf("RESULTS:");
        if (resultsPos != -1) {
            QByteArray frameData = data.mid(6, resultsPos - 6);
            QImage frame;
            if (frame.loadFromData(frameData)) {
                QByteArray resultsData = data.mid(resultsPos + 8);
                QDataStream resultsStream(resultsData);
                qint32 resultCount;
                resultsStream >> resultCount;

                std::vector<DetectionResult> results;
                for (qint32 i = 0; i < resultCount; ++i) {
                    DetectionResult result;
                    qint32 classId;
                    QString className;
                    qint32 x, y, width, height, centerX, centerY;

                    resultsStream >> classId;
                    resultsStream >> className;
                    resultsStream >> result.confidence;
                    resultsStream >> x;
                    resultsStream >> y;
                    resultsStream >> width;
                    resultsStream >> height;
                    resultsStream >> centerX;
                    resultsStream >> centerY;

                    result.classId = classId;
                    result.className = className.toStdString();
                    result.bbox = cv::Rect(x, y, width, height);
                    result.center = cv::Point(centerX, centerY);
                    results.push_back(result);
                }

                emit frameReceived(frame, results);
            }
        }
    }
}

QByteArray TcpClient::serializeFrame(const QImage& frame)
{
    QByteArray frameData;
    QBuffer buffer(&frameData);
    buffer.open(QIODevice::WriteOnly);
    frame.save(&buffer, "JPG", 80);
    return frameData;
}

QByteArray TcpClient::serializeResults(const std::vector<DetectionResult>& results)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << static_cast<qint32>(results.size());
    for (const auto& result : results) {
        stream << static_cast<qint32>(result.classId);
        stream << QString::fromStdString(result.className);
        stream << result.confidence;
        stream << static_cast<qint32>(result.bbox.x);
        stream << static_cast<qint32>(result.bbox.y);
        stream << static_cast<qint32>(result.bbox.width);
        stream << static_cast<qint32>(result.bbox.height);
        stream << static_cast<qint32>(result.center.x);
        stream << static_cast<qint32>(result.center.y);
    }
    return data;
}

