#ifndef LOOPBACKCONNECTION_H
#define LOOPBACKCONNECTION_H

#include <QObject>
#include <QTcpSocket>

class LoopbackConnection : public QObject
{
    Q_OBJECT
public:
    explicit LoopbackConnection(qintptr socketDescr, QObject *parent = 0);
    ~LoopbackConnection();

signals:

public slots:
    void onConnected();
    void onDisconnected();
    void onBytesWritten(qint64 bytes);
    void onReadyRead();
    void onError(QAbstractSocket::SocketError err);
    void onStateChanged(QAbstractSocket::SocketState state);

private:
    QScopedPointer<QTcpSocket> m_socket;
};

#endif // LOOPBACKCONNECTION_H
