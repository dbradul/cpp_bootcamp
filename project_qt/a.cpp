#include "a.h"
#include <QDebug>
#include <QTimer>
#include <QThread>
#include "worker.h"


A::A(QObject *parent) : QObject(parent)
{
    connect(this, SIGNAL(aaaa()),    this, SLOT(bbbb()));

    connect(this, SIGNAL(signal()),  this, SLOT(onSignal()));
    connect(this, SIGNAL(signal2()), this, SLOT(onSignal2()));
}

A::~A()
{
}

void A::start()
{
    emit signal();
}

void A::startTimer()
{
    auto timer = new QTimer(this);
    timer->setInterval(500);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimer()));
    timer->start();
}

void A::onSignal()
{
    qDebug() << __PRETTY_FUNCTION__;

    QThread* th = new QThread();
    Worker* worker = new Worker();
    worker->moveToThread(th);
    connect(th, SIGNAL(started()), worker, SLOT(doWork()));
    th->start();

    //QThread::sleep(100);

    emit signal2();
}

void A::onSignal2()
{
    qDebug() << __PRETTY_FUNCTION__;
    //emit signal();
}

void A::onTimer()
{
    qDebug() << __PRETTY_FUNCTION__;
}

