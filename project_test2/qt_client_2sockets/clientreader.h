#ifndef CLIENTREADER_H
#define CLIENTREADER_H

#include <QObject>
#include <QTcpSocket>
#include "packet.h"

class ClientReader : public QObject
{
    Q_OBJECT
public:
    explicit ClientReader(/*QTcpSocket* socket, */QObject *parent = 0);
    ~ClientReader();

signals:
    void updatedThroughputInfo(qint64 value); //Mb/s
    void updatedRoundtripLatencyInfo(qint64 value); //ms

public slots:
    void StartTest();
    void onConnected();
    void onDisconnected();
    void onReadyRead();

private:
    bool ParsePacket(Packet& p);

private:
    QTcpSocket* m_socket;
    quint64 m_packetsTotal;
    quint64 m_latencyTotal;
    quint64 m_receivedTotal;
    quint64 m_testStarted;
};

#endif // CLIENTREADER_H
