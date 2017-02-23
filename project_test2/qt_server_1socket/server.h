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
    void dataRead(int);

public slots:
    void start();
    void onNewConnection();
    void onBytesWritten(qint64 bytes);
    void onReadyRead();

private:
    QTcpServer* m_server;
};

#endif // SERVER_H
