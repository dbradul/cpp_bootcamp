#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>

class SimpleClient : public QObject
{
    Q_OBJECT
public:
    explicit SimpleClient(QObject *parent = 0);
    ~SimpleClient();

    void Connect();

signals:

public slots:

    void onConnected();
    void onDisconnected();
    void onBytesWritten(qint64 bytes);
    void onReadyRead();

private:
    QTcpSocket* m_socket;
};

#endif // CLIENT_H
