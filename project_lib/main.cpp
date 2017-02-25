#include <iostream>
#include "project_lib.h"

using namespace std;

int main(int argc, char *argv[])
{
    cout << "Hello World!" << endl;
    
    Project_lib s;
    cout << s.sum(2, 3) << endl;

    return 0;
}
