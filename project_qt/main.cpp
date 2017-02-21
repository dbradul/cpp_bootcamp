#include <iostream>
#include <QCoreApplication>
#include <QTimer>
#include <QThread>
#include <QDateTime>

#include "client.h"
#include "a.h"

// Links:
// - http://doc.qt.io/qt-5.6/signalsandslots.html#signals-and-slots
// - http://wiki.qt.io/Threads_Events_QObjects

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

//    A a;
//    A a2;
//    //a.startTimer();

//    QTimer timer;
//    timer.setInterval(1000);
//    a.connect(&timer, SIGNAL(timeout()), &a, SLOT(onTimer()));
////    a.connect(&timer, &QTimer::timeout, []
////                                        {
////                                            qDebug() << QDateTime::currentMSecsSinceEpoch();
////                                        });
//    a.connect(&a, &A::currentTS, [](quint64 ts)
//                                {
//                                    qDebug() << ts;
//                                });

//    timer.start();

//    QThread th1;
//    a2.moveToThread(&th1);
//    a2.connect(&th1, SIGNAL(started()), &a2, SLOT(onSignal()));
//    th1.start();


    //a2.onSignal();

//    emit a2.signal();
//    a.foobar();
//    a.start();

    SimpleClient c;
    c.Connect();

//    A a;
//    A a2;
    //a.connect(&c, SIGNAL(valueSet(int)), &a, SLOT(onValueSet(int)));
//    a.connect(&c, SIGNAL(valueSet(int)), &a, SLOT(onValueSet(int)));
//    a2.connect(&c, SIGNAL(valueSet(int)), &a2, SLOT(onValueSet(int)));

//    c.setValue(42);
//    c.setValue(43);
//    c.setValue(44);




    //    while(true)
    //    {
    //        Event ev = queue.front();
    //        ev.handle();
    //        queue.pop();
    //    }



    return app.exec();
}
