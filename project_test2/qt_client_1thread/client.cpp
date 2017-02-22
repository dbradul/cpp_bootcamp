#include "client.h"
#include <QDateTime>
#include <QThread>


Client::Client(QObject *parent)
    : QObject(parent)
    , m_socket(new QTcpSocket(this))
    , m_packetsTotal(0u)
    , m_latencyTotal(0u)
    , m_receivedTotal(0u)
    , m_testStarted(0u)
{
    connect(m_socket, SIGNAL(connected()),          this, SLOT(onConnected()));
    connect(m_socket, SIGNAL(disconnected()),       this, SLOT(onDisconnected()));
    connect(m_socket, SIGNAL(readyRead()),          this, SLOT(onReadyRead()));
    connect(m_socket, SIGNAL(bytesWritten(qint64)), this, SLOT(onBytesWritten(qint64)));
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onError(QAbstractSocket::SocketError)));
}

Client::~Client()
{
    delete m_socket;
}

void Client::StartTest()
{
    m_socket->connectToHost("127.0.0.1", 9999);
}

void Client::onConnected()
{
    qDebug() << __PRETTY_FUNCTION__;

    m_testStarted = QDateTime::currentMSecsSinceEpoch();

    Packet p;
    PreparePacket(p);
    m_socket->write(reinterpret_cast<char*>(&p), sizeof(p));
}

void Client::onError(QAbstractSocket::SocketError err)
{
    qDebug() << __PRETTY_FUNCTION__ << ", error: " << err;
}

void Client::PreparePacket(Packet& p)
{
    qDebug() << __PRETTY_FUNCTION__;

    p.timestamp = QDateTime::currentMSecsSinceEpoch();
    memset(p.marker, HEADER_MARKER,  sizeof(p.marker));
    memset(p.data,   '.',            sizeof(p.data));

    //return m_packet;
}

void Client::ParsePacket(Packet& p)
{
    qDebug() << __PRETTY_FUNCTION__;

    qint64 read_bytes = m_socket->read(reinterpret_cast<char*>(&p),
                                       sizeof(p));

    while (read_bytes < sizeof(p))
    {
        qDebug() << "Read bytes: " << read_bytes;
        read_bytes += m_socket->read(reinterpret_cast<char*>(&p) + read_bytes,
                                     sizeof(p) - read_bytes);
    }
}

void Client::onDisconnected()
{
    qDebug() << __PRETTY_FUNCTION__;
}

void Client::onBytesWritten(qint64 bytes)
{
    qDebug() << __PRETTY_FUNCTION__;
    qDebug() << "We wrote: " << bytes << " bytes";
}

void Client::onReadyRead()
{
    qDebug() << __PRETTY_FUNCTION__;
    qDebug() << "Reading...";

    Packet p;
    ParsePacket(p);

    m_packetsTotal += 1;
    m_latencyTotal += QDateTime::currentMSecsSinceEpoch() - p.timestamp;

    if (m_packetsTotal%5000 == 0)
    {
        quint64 elapsedTotal = QDateTime::currentMSecsSinceEpoch() - m_testStarted;
        emit updatedThroughputInfo(m_packetsTotal*PACKET_SIZE*8*1000/(1024*1024*elapsedTotal));
        emit updatedRoundtripLatencyInfo(m_latencyTotal/m_packetsTotal);
    }

    if (m_packetsTotal == PACKET_TEST_LIMIT)
    {
        emit testFinished();
        m_socket->close();
    }
    else
    {
        Packet p;
        PreparePacket(p);
        m_socket->write(reinterpret_cast<char*>(&p), sizeof(p));
    }
}
