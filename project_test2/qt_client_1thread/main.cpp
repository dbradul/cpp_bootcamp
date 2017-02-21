#include <QCoreApplication>
#include <QDebug>
#include <QDateTime>
#include <QThread>
#include "client.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Client c;
    QThread th1;
    c.moveToThread(&th1);
    c.connect(&th1, SIGNAL(started()), &c, SLOT(StartTest()));
    //c.StartTest();

    c.connect(&c, &Client::updatedThroughputInfo, [=](quint64 value)
                            {
                                qInfo() << "Current throughput: " << value << "Mb/s";
                            });
    c.connect(&c, &Client::updatedRoundtripLatencyInfo, [=](quint64 value)
                            {
                                qInfo() << "Round-trip latency: " << value << "ms";
                            });
    th1.start();

    return a.exec();
}
