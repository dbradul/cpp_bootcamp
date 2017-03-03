#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    ~Server();

signals:

public slots:
    void Start();

protected:
    void incomingConnection(qintptr socketDescriptor) override;
};

#endif // SERVER_H
