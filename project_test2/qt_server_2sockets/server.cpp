#include "server.h"
#include <QTcpSocket>

Server::Server(QObject *parent)
    : QObject(parent)
    , m_server(new QTcpServer(this))
    , m_socketRd(nullptr)
    , m_socketWr(nullptr)
    , m_handshakeFinished(false)
{
    connect(m_server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
    connect(m_server, SIGNAL(acceptError(QAbstractSocket::SocketError)),
            this,     SLOT(onError(QAbstractSocket::SocketError)));

    if(!m_server->listen(QHostAddress::Any, 9999))
    {
        qDebug() << "Server could not start!";
    }
    else
    {
        qDebug() << "Server started!";
    }
}

Server::~Server()
{
    delete m_server;
}

void Server::onNewConnection()
{
    qDebug() << "Connected (server)!";

    QTcpSocket *socket = m_server->nextPendingConnection();

    connect(socket, SIGNAL(readyRead()),    this, SLOT(onReadyRead()));
}

void Server::onDisconnected()
{
    qDebug() << "Disconnected (server)!";
}

void Server::onBytesWritten(qint64 bytes)
{
    qDebug() << "We wrote: " << bytes << " bytes";
}

void Server::handshake()
{
    QTcpSocket* socket = static_cast<QTcpSocket*>(sender());

    char buf[2];
    socket->read(buf, sizeof(buf));

    if (memcmp("RD", buf, sizeof(buf))==0)
    {
        qInfo() << "Writer inited";
        m_socketWr = socket;
        connect(m_socketWr,     SIGNAL(bytesWritten(qint64)), this, SLOT(onBytesWritten(qint64)));
        connect(m_socketWr,     SIGNAL(disconnected()), this, SLOT(onDisconnected()));
        disconnect(m_socketWr,  SIGNAL(readyRead()),    this, SLOT(onReadyRead()));
    }
    else if (memcmp("WR", buf, sizeof(buf))==0)
    {
        qInfo() << "Reader inited";
        m_socketRd = socket;
        connect(m_socketRd,     SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    }

    if (m_socketRd && m_socketWr)
    {
        m_handshakeFinished = true;
    }
}

void Server::onReadyRead()
{
    QTcpSocket* socket = static_cast<QTcpSocket*>(sender());

    if (!m_handshakeFinished)
    {
        handshake();
    }
    else
    {
        QByteArray data = socket->readAll();

        qDebug() << "Read: " << data.size() << " bytes" << endl;

        m_socketWr->write(data);
    }
}

void Server::onError(QAbstractSocket::SocketError err)
{
    qDebug() << __PRETTY_FUNCTION__ << ": " << err;
}
