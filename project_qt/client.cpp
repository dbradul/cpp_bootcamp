#include "client.h"

SimpleClient::SimpleClient(QObject *parent)
    : QObject(parent)
    , m_socket(new QTcpSocket(this))
{
    connect(m_socket, SIGNAL(connected()),          this, SLOT(onConnected()));
    connect(m_socket, SIGNAL(disconnected()),       this, SLOT(onDisconnected()));
    connect(m_socket, SIGNAL(readyRead()),          this, SLOT(onReadyRead()));
    connect(m_socket, SIGNAL(bytesWritten(qint64)), this, SLOT(onBytesWritten(qint64)));
}

SimpleClient::~SimpleClient()
{
    delete m_socket;
}

void SimpleClient::Connect()
{
    m_socket->connectToHost("127.0.0.1", 9999);
}

SimpleClient::setValue(int newValue)
{
    m_value = newValue;
    emit valueSet(m_value);
}

void SimpleClient::onConnected()
{
    qDebug() << "Connected (client)!";

    // send
    m_socket->write("hello server\r\n\r\n\r\n\r\n");

    qDebug() << "Written!";
}

void SimpleClient::onDisconnected()
{
    qDebug() << "Disconnected!";
}

void SimpleClient::onBytesWritten(qint64 bytes)
{
    qDebug() << "We wrote: " << bytes << " bytes";
}

void SimpleClient::onReadyRead()
{
    qDebug() << "Reading...";
    qDebug() << m_socket->readAll();

    m_socket->close();
}
