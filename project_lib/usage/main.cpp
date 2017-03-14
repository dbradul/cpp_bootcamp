#include <iostream>
#include "project_lib.h"

using namespace std;

int main(int argc, char *argv[])
{
    extern int gVar;

    cout << "Hello World!!!" << endl;
    cout << "Global var: " << gVar << endl;

    Project_lib lib;

    cout << lib.sum(42, 3) << endl;

    return 0;
}
