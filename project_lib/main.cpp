#include <iostream>
#include "project_lib.h"

// Links:
// -- https://habrahabr.ru/post/155467/
// -- https://cmake.org/Wiki/CMake_Useful_Variables
// -- https://habrahabr.ru/post/188354/
// -- https://www.youtube.com/watch?v=ckO98bRzL9Y

// Qmake
// Generate proj for VS:
//    qmake -spec win32-msvc2008 -tp vc

using namespace std;


int main(int argc, char *argv[])
{
    cout << "Hello World!!!" << endl;

    Project_lib s;
    auto a = 42;
    cout << "Project_lib::sum(): " << s.sum(a, 3) << endl;
 //   cout << "Project_lib::sum(): " << s.sum2(a, 3) << endl;


    return 0;
}


void test ()
{
}
