#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QByteArray>


class Client : public QObject
{
    Q_OBJECT
public:

    static const size_t PACKET_SIZE     = 4*4*1024;
    static const char  HEADER_MARKER    = 'X';
    const size_t PACKET_TEST_LIMIT      = 1024u*1024u;

    //#pragma pack(push, 1)
    struct Packet
    {
        char   marker[4];
        qint64 timestamp;
        char   data[PACKET_SIZE - sizeof(timestamp) - sizeof(marker)];
    } __attribute__((packed));
    //#pragma pack(pop)

    explicit Client(QObject *parent = 0);
    ~Client();

signals:

    void updatedThroughputInfo(qint64 value); //Mb/s
    void updatedRoundtripLatencyInfo(qint64 value); //ms
    void testFinished();

public slots:

    void StartTest();
    void onConnected();
    void onDisconnected();
    void onBytesWritten(qint64 bytes);
    void onReadyRead();

private:

    void PreparePacket(Packet& p);
    void ParsePacket(Packet& p);

private:
    QTcpSocket* m_socket;

    quint64 m_packetsTotal;
    quint64 m_latencyTotal;
    quint64 m_receivedTotal;
    qint64 m_testStarted;
};

#endif // CLIENT_H
