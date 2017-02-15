#include <iostream>
#include "client.h"
#include <QCoreApplication>


using namespace std;

int main(int argc, char *argv[])
{
    cout << "Enter!" << endl;

    QCoreApplication app(argc, argv);

    SimpleClient c;

    c.Connect();

    return app.exec();;
}
