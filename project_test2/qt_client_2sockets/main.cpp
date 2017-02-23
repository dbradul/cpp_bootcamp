#include <QCoreApplication>
#include <QDebug>
#include <QThread>
#include "clientwriter.h"
#include "clientreader.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ClientReader reader;
    QThread thReader;
    reader.moveToThread(&thReader);
    reader.connect(&thReader, SIGNAL(started()), &reader, SLOT(StartTest()));

    reader.connect(&reader, &ClientReader::updatedThroughputInfo, [](quint64 value)
                            {
                                qInfo() << "Current throughput: " << value << "Mb/s";
                            });
    reader.connect(&reader, &ClientReader::updatedRoundtripLatencyInfo, [](quint64 value)
                            {
                                qInfo() << "Round-trip latency: " << value << "ms";
                            });
    thReader.start();


    ClientWriter writer;
    QThread thWriter;
    writer.moveToThread(&thWriter);
    writer.connect(&thWriter, SIGNAL(started()), &writer, SLOT(StartTest()));
    thWriter.start();


    return a.exec();
}
