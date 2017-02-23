#include "loopbackconnection.h"
#include <QThread>


LoopbackConnection::LoopbackConnection(qintptr socketDescr, QObject *parent)
    : QObject(parent)
    , m_socket(new QTcpSocket(this))
{
    m_socket->setSocketDescriptor(socketDescr);

    connect(m_socket, SIGNAL(connected()),          this, SLOT(onConnected()));
    connect(m_socket, SIGNAL(bytesWritten(qint64)), this, SLOT(onBytesWritten(qint64)));
    connect(m_socket, SIGNAL(readyRead()),          this, SLOT(onReadyRead()));
    connect(m_socket, SIGNAL(disconnected()),       this, SLOT(onDisconnected()));
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)),              this, SLOT(onError(QAbstractSocket::SocketError)));
    connect(m_socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),       this, SLOT(onStateChanged(QAbstractSocket::SocketState)));
}

LoopbackConnection::~LoopbackConnection()
{
    delete m_socket;
}

void LoopbackConnection::onConnected()
{
    qDebug() << __PRETTY_FUNCTION__;
}

void LoopbackConnection::onDisconnected()
{
    qDebug() << "Disconnected";
    thread()->quit();
}

void LoopbackConnection::onBytesWritten(qint64 bytes)
{
    qDebug() << "We wrote: " << bytes << " bytes";
}

void LoopbackConnection::onReadyRead()
{
    QByteArray data = m_socket->readAll();

    qDebug() << "Read: " << data.size() << endl;

    m_socket->write(data);
}

void LoopbackConnection::onError(QAbstractSocket::SocketError err)
{
    qWarning() << "Error: " << err;
}

void LoopbackConnection::onStateChanged(QAbstractSocket::SocketState state)
{
    qWarning() << "State changed to: " << state;
}

