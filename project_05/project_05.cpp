#include <iostream>

using namespace std;

static int a_var = 10;
int b_var = 20;


void project_05_foo_global()
{
    cout << __PRETTY_FUNCTION__ << endl;
}


static void project_05_foo_local()
{
    cout << __PRETTY_FUNCTION__ << endl;
}
