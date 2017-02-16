#include "worker.h"
#include <QThread>
#include <QDebug>

Worker::Worker(QObject *parent) : QObject(parent)
{
}

void Worker::doWork()
{
    for(int i=0; i<100; ++i)
    {
        qDebug() << QString("Working(%d)...").arg(i);
        QThread::msleep(500);
    }
}
