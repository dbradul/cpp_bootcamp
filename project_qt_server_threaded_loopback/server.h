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

public slots:
    void Start();
    void onNewConnection();

private:
    QTcpServer* m_server;
};

#endif // SERVER_H
