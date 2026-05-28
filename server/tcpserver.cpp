#include "tcpserver.h"
#include <QDebug>
#include <QDataStream>
#include <QDateTime>
#include <QCryptographicHash>
#include <qrandom.h>
TcpServer::TcpServer(QObject *parent) : QObject(parent)
{
    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &TcpServer::onNewConnection);
}

TcpServer::~TcpServer()
{
    stopServer();
}

bool TcpServer::startServer(quint16 port)
{
    if (server->listen(QHostAddress::Any, port)) {
        qDebug() << "Server started on port:" << port;
        return true;
    } else {
        qDebug() << "Server failed to start. Error:" << server->errorString();
        return false;
    }
}

void TcpServer::stopServer()
{
    if (server->isListening()) {
        server->close();
    }
    for (auto& client : clients) {
        client.socket->close();
        client.socket->deleteLater();
    }
    clients.clear();
}

void TcpServer::onNewConnection()
{
    QTcpSocket* socket = server->nextPendingConnection();
    if (!socket) {
        return;
    }

    ClientInfo client;
    client.socket = socket;
    client.clientId = generateClientId();
    client.connectTime = QDateTime::currentDateTime();
    client.isAuthenticated = false;

    clients.append(client);

    connect(socket, &QTcpSocket::disconnected, this, &TcpServer::onClientDisconnected);
    connect(socket, &QTcpSocket::readyRead, this, &TcpServer::onReadyRead);

    qDebug() << "New client connected. ID:" << client.clientId;
    emit clientConnected(client.clientId);

    QString welcomeMsg = QString("Welcome to Smart Security Server. Your ID: %1").arg(client.clientId);
    sendMessageToClient(client.clientId, welcomeMsg);
}

void TcpServer::onClientDisconnected()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) {
        return;
    }

    QString clientId;
    for (int i = 0; i < clients.size(); ++i) {
        if (clients[i].socket == socket) {
            clientId = clients[i].clientId;
            clients.removeAt(i);
            break;
        }
    }

    socket->deleteLater();
    qDebug() << "Client disconnected. ID:" << clientId;
    emit clientDisconnected(clientId);
}

void TcpServer::onReadyRead()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) {
        return;
    }

    QString clientId;
    for (const auto& client : clients) {
        if (client.socket == socket) {
            clientId = client.clientId;
            break;
        }
    }

    QByteArray data = socket->readAll();
    
    // 检查是否有大小前缀
    if (data.size() < static_cast<int>(sizeof(qint64))) {
        return;
    }

    QDataStream stream(data);
    qint64 expectedBytes;
    stream >> expectedBytes;

    if (expectedBytes <= 0 || expectedBytes > 10 * 1024 * 1024) {
        qDebug() << "Invalid data size received:" << expectedBytes;
        return;
    }

    // 检查数据是否完整
    if (data.size() < static_cast<int>(sizeof(qint64) + expectedBytes)) {
        qDebug() << "Incomplete data received";
        return;
    }

    QByteArray payload = data.mid(sizeof(qint64), expectedBytes);

    if (payload.startsWith("MSG:")) {
        QString message = QString::fromUtf8(payload.mid(4));
        qDebug() << "Message from" << clientId << ":" << message;
        emit messageReceived(clientId, message);
    } else if (payload.startsWith("FRAME:")) {
        QByteArray frameData = payload.mid(6);
        QImage frame;
        if (frame.loadFromData(frameData)) {
            emit frameReceived(clientId, frame);
        } else {
            qDebug() << "Failed to load image from data";
        }
    }
}

void TcpServer::sendFrameToAllClients(const QImage& frame, const std::vector<DetectionResult>& results)
{
    QByteArray data = serializeFrame(frame, results);
    for (const auto& client : clients) {
        if (client.socket->state() == QAbstractSocket::ConnectedState) {
            client.socket->write(data);
            client.socket->flush();
        }
    }
}

void TcpServer::sendMessageToClient(const QString& clientId, const QString& message)
{
    QByteArray data = "MSG:" + message.toUtf8();
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << static_cast<qint64>(data.size());
    out.writeRawData(data.data(), data.size());

    for (const auto& client : clients) {
        if (client.clientId == clientId && client.socket->state() == QAbstractSocket::ConnectedState) {
            client.socket->write(block);
            client.socket->flush();
            break;
        }
    }
}

void TcpServer::sendMessageToAllClients(const QString& message)
{
    QByteArray data = "MSG:" + message.toUtf8();
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << static_cast<qint64>(data.size());
    out.writeRawData(data.data(), data.size());

    for (const auto& client : clients) {
        if (client.socket->state() == QAbstractSocket::ConnectedState) {
            client.socket->write(block);
            client.socket->flush();
        }
    }
}

QString TcpServer::generateClientId()
{
    QString timestamp = QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch());
    QString random = QString::number(rand() % 10000);
    QString raw = timestamp + random;
    return QCryptographicHash::hash(raw.toUtf8(), QCryptographicHash::Md5).toHex().left(8);
}

QByteArray TcpServer::serializeFrame(const QImage& frame, const std::vector<DetectionResult>& results)
{
    QByteArray frameData;
    QBuffer buffer(&frameData);
    buffer.open(QIODevice::WriteOnly);
    frame.save(&buffer, "JPG", 80);

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

    QByteArray combinedData = "FRAME:" + frameData + "RESULTS:" + resultsData;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << static_cast<qint64>(combinedData.size());
    out.writeRawData(combinedData.data(), combinedData.size());

    return block;
}

void TcpServer::deserializeFrame(const QByteArray& data, QImage& frame, std::vector<DetectionResult>& results)
{
    if (!data.startsWith("FRAME:")) {
        return;
    }

    int resultsPos = data.indexOf("RESULTS:");
    if (resultsPos == -1) {
        return;
    }

    QByteArray frameData = data.mid(6, resultsPos - 6);
    frame.loadFromData(frameData);

    QByteArray resultsData = data.mid(resultsPos + 8);
    QDataStream resultsStream(resultsData);
    qint32 resultCount;
    resultsStream >> resultCount;

    results.clear();
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
}
