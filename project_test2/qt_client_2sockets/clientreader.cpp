#include "clientreader.h"
#include <QThread>
#include <QDateTime>
#include <exception>
#include <sstream>
#include <cassert>

using namespace std;

ClientReader::ClientReader(QObject *parent)
    : QObject(parent)
    , m_socket(new QTcpSocket(this))
    , m_packetsTotal(0u)
    , m_latencyTotal(0u)
    , m_receivedTotal(0u)
    , m_testStarted(0u)
{
    connect(m_socket, SIGNAL(connected()),      this, SLOT(onConnected()));
    connect(m_socket, SIGNAL(disconnected()),   this, SLOT(onDisconnected()));
    connect(m_socket, SIGNAL(readyRead()),      this, SLOT(onReadyRead()));
}

ClientReader::~ClientReader()
{
    delete m_socket;
}

void ClientReader::StartTest()
{
    m_socket->connectToHost("127.0.0.1", 9999);
}

void ClientReader::onConnected()
{
    qDebug() << __PRETTY_FUNCTION__ << " ClientReader";

    m_socket->write("RD", 2);
}

void ClientReader::onDisconnected()
{
    qDebug() << __PRETTY_FUNCTION__;
}

bool ClientReader::ParsePacket(Packet& p)
{
    qDebug() << __PRETTY_FUNCTION__;

    bool bResult = true;

    char header[sizeof(p.marker)];
    qint64 read_bytes = m_socket->read(header, sizeof(header));
    assert(read_bytes==sizeof(header));

    // if header doesn't match ...
    if (memcmp(p.marker, header, sizeof(header)) != 0)
    {
        // ...align bytestream with packet's start
        char c = '\0';
        while (c != TAIL)
        {
            m_socket->read(&c, 1);
        }
        bResult = false;
    }
    else
    {
        qint64 packet_size = sizeof(p)-sizeof(p.marker);
        qint64 reminder = m_socket->read(reinterpret_cast<char*>(&p) + sizeof(p.marker),
                                         packet_size);
        while (reminder < packet_size)
        {
            qint64 bytes = m_socket->read(reinterpret_cast<char*>(&p) + reminder,
                                          packet_size - reminder);

            reminder += bytes;
        }
    }

    return bResult;
}


void ClientReader::onReadyRead()
{
    if (0 == m_testStarted)
    {
        m_testStarted = QDateTime::currentMSecsSinceEpoch();
    }

    while (m_socket->bytesAvailable()>=sizeof(Packet))
    {
        Packet p;

        if (!ParsePacket(p))
        {
            // qWarning() << "Failed packet read!";
            continue;
        }

        m_packetsTotal += 1;
        m_latencyTotal += QDateTime::currentMSecsSinceEpoch() - p.timestamp;

        if (m_packetsTotal%5000 == 0)
        {
            quint64 elapsedTotal = QDateTime::currentMSecsSinceEpoch() - m_testStarted;
            emit updatedThroughputInfo(m_packetsTotal*PACKET_SIZE*8*1000/(1024*1024*elapsedTotal));
            emit updatedRoundtripLatencyInfo(m_latencyTotal/m_packetsTotal);
        }
    }
}
