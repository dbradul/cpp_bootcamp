#include <QTcpSocket>
#include <QThread>

#include "server.h"
#include "loopbackconnection.h"

Server::Server(QObject *parent)
    : QObject(parent)
    , m_server(new QTcpServer(this))
{
    connect(m_server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
}

Server::~Server()
{
    delete m_server;
}

void Server::Start()
{
    if(!m_server->listen(QHostAddress::Any, 9999))
    {
        qDebug() << "Server could not start!";
    }
    else
    {
        qDebug() << "Server started!";
    }
}

void Server::onNewConnection()
{
    qDebug() << "Connected (server)!";

    QTcpSocket *socket = m_server->nextPendingConnection();

    QThread* thConn          = new QThread();
    LoopbackConnection* conn = new LoopbackConnection(socket->socketDescriptor());
    conn->moveToThread(thConn);
    conn->connect(thConn, SIGNAL(finished()), thConn, SLOT(deleteLater()));
    conn->connect(thConn, SIGNAL(finished()), conn,   SLOT(deleteLater()));

    thConn->start();
}
