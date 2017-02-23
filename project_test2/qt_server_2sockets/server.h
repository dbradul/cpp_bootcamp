#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    ~Server();

signals:
    void runWorker( QTcpSocket *socket );

public slots:
    void onNewConnection();
    void onDisconnected();
    void onBytesWritten(qint64 bytes);
    void onReadyRead();
    void onError(QAbstractSocket::SocketError);

private:
    void handshake();

private:
    QTcpServer* m_server;
    QTcpSocket* m_socketRd;
    QTcpSocket* m_socketWr;

    bool m_handshakeFinished;
};

#endif // SERVER_H
