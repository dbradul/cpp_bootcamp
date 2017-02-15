#include "client.h"

SimpleClient::SimpleClient(QObject *parent)
    : QObject(parent)
    , m_socket(new QTcpSocket(this))
{
    connect(m_socket, SIGNAL(connected()),      this, SLOT(connected()));
    connect(m_socket, SIGNAL(disconnected()),   this, SLOT(disconnected()));
    connect(m_socket, SIGNAL(readyRead()),      this, SLOT(readyRead()));
    connect(m_socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
}

SimpleClient::~SimpleClient()
{
    delete m_socket;
}

void SimpleClient::Connect()
{
    m_socket->connectToHost("127.0.0.1", 9999);
}

void SimpleClient::connected()
{
    qDebug() << "Connected (client)!";

    // send
    m_socket->write("hello server\r\n\r\n\r\n\r\n");
    m_socket->waitForBytesWritten(1000);
    m_socket->waitForReadyRead(3000);

    qDebug() << "Written!";

    m_socket->close();
}

void SimpleClient::disconnected()
{
    qDebug() << "Disconnected!";
}

void SimpleClient::bytesWritten(qint64 bytes)
{
    qDebug() << "We wrote: " << bytes << " bytes";
}

void SimpleClient::readyRead()
{
    qDebug() << "Reading...";
    qDebug() << m_socket->readAll();
}
