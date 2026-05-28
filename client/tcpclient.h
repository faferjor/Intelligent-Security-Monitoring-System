#ifndef TCPCLIENT_H
#define TCPCLIENT_H
#include <QBuffer>
#include <QObject>
#include <QTcpSocket>
#include <QByteArray>
#include <QImage>
#include <QTimer>
#include "detectormodel.h"

class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = nullptr);
    ~TcpClient();

    bool connectToServer(const QString& host, quint16 port);
    void disconnectFromServer();
    void sendMessage(const QString& message);
    void sendFrame(const QImage& frame);
    void sendDetectionResults(const std::vector<DetectionResult>& results);

    bool isConnected() const { return socket && socket->state() == QAbstractSocket::ConnectedState; }
    QString getServerAddress() const { return serverAddress; }
    quint16 getServerPort() const { return serverPort; }

signals:
    void connected();
    void disconnected();
    void connectionError(const QString& errorString);
    void messageReceived(const QString& message);
    void frameReceived(const QImage& frame, const std::vector<DetectionResult>& results);
    void serverMessageReceived(const QString& message);

private slots:
    void onConnected();
    void onDisconnected();
    void onErrorOccurred(QAbstractSocket::SocketError socketError);
    void onReadyRead();
    void processServerData();

private:
    QTcpSocket* socket;
    QString serverAddress;
    quint16 serverPort;
    QByteArray buffer;
    qint64 expectedBytes = 0;

    QByteArray serializeFrame(const QImage& frame);
    QByteArray serializeResults(const std::vector<DetectionResult>& results);
    void deserializeServerData(const QByteArray& data);
};

#endif // TCPCLIENT_H
