#include "server.h"
#include <QTcpSocket>

Server::Server(QObject *parent)
    : QObject(parent)
    , m_server(new QTcpServer(this))
{
}


Server::~Server()
{
    delete m_server;
}

void Server::start()
{
    connect(m_server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));

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

    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(onBytesWritten(qint64)));
}

void Server::onBytesWritten(qint64 bytes)
{
    qDebug() << "We wrote: " << bytes << " bytes";
}


void Server::onReadyRead()
{
    QTcpSocket* socket = static_cast<QTcpSocket*>(sender());

    QByteArray data = socket->readAll();
    QString s_data = QString::fromUtf8(data.data());

    qDebug() << "Read: " << s_data << endl;

    socket->write(data);

    emit dataRead(data.size());
}
