#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QByteArray>
#include <QImage>
#include <QTimer>
#include <vector>
#include "detectormodel.h"
#include "QDateTime"
#include <QBuffer>
struct ClientInfo {
    QTcpSocket* socket;
    QString clientId;
    QDateTime connectTime;
    bool isAuthenticated;
};

class TcpServer : public QObject
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = nullptr);
    ~TcpServer();

    bool startServer(quint16 port);
    void stopServer();
    void sendFrameToAllClients(const QImage& frame, const std::vector<DetectionResult>& results);
    void sendFrameToClient(const QString& clientId, const QImage& frame, const std::vector<DetectionResult>& results);
    void sendMessageToClient(const QString& clientId, const QString& message);
    void sendMessageToAllClients(const QString& message);

    QList<ClientInfo> getConnectedClients() const { return clients; }
    int getClientCount() const { return clients.size(); }

signals:
    void clientConnected(const QString& clientId);
    void clientDisconnected(const QString& clientId);
    void messageReceived(const QString& clientId, const QString& message);
    void frameReceived(const QString& clientId, const QImage& frame);

private slots:
    void onNewConnection();
    void onClientDisconnected();
    void onReadyRead();

private:
    QTcpServer* server;
    QList<ClientInfo> clients;

    QString generateClientId();
    QByteArray serializeFrame(const QImage& frame, const std::vector<DetectionResult>& results);
    void deserializeFrame(const QByteArray& data, QImage& frame, std::vector<DetectionResult>& results);
};

#endif // TCPSERVER_H
