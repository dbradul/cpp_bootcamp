#include <iostream>
#include "project_lib.h"

// Links:
// -- https://habrahabr.ru/post/155467/
// -- https://cmake.org/Wiki/CMake
// -- https://habrahabr.ru/post/188354/

using namespace std;

int main(int argc, char *argv[])
{
    cout << "Hello World!!!" << endl;

    Project_lib s;
    auto a = 42;
    cout << s.sum(2, 3) << endl;

    return 0;
}
