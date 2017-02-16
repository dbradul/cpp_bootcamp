#include <iostream>
#include <QCoreApplication>

#include "client.h"
#include "a.h"

// Links:
// - http://doc.qt.io/qt-5.6/signalsandslots.html#signals-and-slots
// - http://wiki.qt.io/Threads_Events_QObjects

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    A a;
    a.startTimer();
    a.start();

    SimpleClient c;
    c.Connect();

    //    while(true)
    //    {
    //        Event ev = queue.front();
    //        ev.handle();
    //        queue.pop();
    //    }
    return app.exec();
}
