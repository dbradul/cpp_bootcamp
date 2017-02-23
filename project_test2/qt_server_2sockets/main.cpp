#include <iostream>
#include "server.h"
#include <QCoreApplication>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    Server s(&app);

    return app.exec();
}
