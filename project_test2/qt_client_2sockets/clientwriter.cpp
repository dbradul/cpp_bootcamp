#include "clientwriter.h"
#include <QDateTime>
#include <QThread>


ClientWriter::ClientWriter(QObject *parent)
    : QObject(parent)
    , m_socket(new QTcpSocket(this))
{
    connect(m_socket,  SIGNAL(connected()),          this, SLOT(onConnected()));
    connect(m_socket,  SIGNAL(disconnected()),       this, SLOT(onDisconnected()));
    connect(m_socket,  SIGNAL(bytesWritten(qint64)), this, SLOT(onBytesWritten(qint64)));
}

ClientWriter::~ClientWriter()
{
    delete m_socket;
}

void ClientWriter::StartTest()
{
    m_socket->connectToHost("127.0.0.1", 9999);
}

void ClientWriter::onConnected()
{
    qDebug() << __PRETTY_FUNCTION__ << " ClientWriter";

    m_socket->write("WR", 2);
}

void ClientWriter::onDisconnected()
{
    qDebug() << __PRETTY_FUNCTION__;
}

void ClientWriter::PreparePacket(Packet& p)
{
    qDebug() << __PRETTY_FUNCTION__;

    memset(p.marker, HEADER,  sizeof(p.marker));
    memset(p.data,   '.',            sizeof(p.data));
    memset(p.data + (sizeof(p.data)-sizeof(TAIL)),
                    TAIL,
                    sizeof(TAIL));

    p.timestamp = QDateTime::currentMSecsSinceEpoch();
}

void ClientWriter::onBytesWritten(qint64 bytes)
{
    qDebug() << __PRETTY_FUNCTION__;
    qDebug() << "We wrote: " << bytes << " bytes";

    Packet p;
    PreparePacket(p);

    m_socket->write(reinterpret_cast<char*>(&p), sizeof(p));
}

