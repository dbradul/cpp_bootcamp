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

    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();

private:
    QTcpSocket* m_socket;
};


#endif // CLIENT_H
