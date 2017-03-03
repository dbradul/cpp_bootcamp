#include <QTcpSocket>
#include <QThread>

#include "server.h"
#include "loopbackconnection.h"

Server::Server(QObject *parent)
    : QTcpServer(parent)
{
}

Server::~Server()
{
}

void Server::Start()
{
    if(!listen(QHostAddress::Any, 9999))
    {
        qDebug() << "Server could not start!";
    }
    else
    {
        qDebug() << "Server started!";
    }
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << "Connected (server)!";

    QThread* thConn          = new QThread();
    LoopbackConnection* conn = new LoopbackConnection(socketDescriptor);
    conn->moveToThread(thConn);
    conn->connect(thConn, SIGNAL(finished()), thConn, SLOT(deleteLater()));
    conn->connect(thConn, SIGNAL(finished()), conn,   SLOT(deleteLater()));

    thConn->start();
}

