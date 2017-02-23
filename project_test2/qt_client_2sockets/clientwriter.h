#ifndef CLIENTWRITER_H
#define CLIENTWRITER_H

#include <QObject>
#include <QTcpSocket>
#include "packet.h"

class ClientWriter : public QObject
{
    Q_OBJECT
public:

    explicit ClientWriter(QObject *parent = 0);
    ~ClientWriter();

signals:

public slots:
    void StartTest();
    void onConnected();
    void onDisconnected();
    void onBytesWritten(qint64 bytes);

private:
    void PreparePacket(Packet& p);

private:
    QTcpSocket* m_socket;
};

#endif // CLIENTWRITER_H
