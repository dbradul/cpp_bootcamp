#include <iostream>
#include "server.h"
#include <QCoreApplication>
#include <QThread>

using namespace std;

int main(int argc, char *argv[])
{
    cout << "Enter!" << endl;

    QCoreApplication app(argc, argv);

    Server s;

    QThread th1;
    s.moveToThread(&th1);
    s.connect(&th1, SIGNAL(started()), &s, SLOT(start()));
    th1.start();

    s.connect(&s, &Server::dataRead, [](int size)
                                    {

                                        qDebug() << "Data read: " << size;
                                    });

    return app.exec();
}
