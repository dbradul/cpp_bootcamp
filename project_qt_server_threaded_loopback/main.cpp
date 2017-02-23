#include <QCoreApplication>
#include "server.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    Server s;
    s.Start();

    return app.exec();
}
