#include <iostream>
#include <cassert>
#include <cstring>

using namespace std;

void func(int** pointer)
{
    (*pointer)++;
}

void func2(int* pointer)
{
    pointer++;
}

void setNull(int*& pointer)
{
    //free(pointer);
    pointer = nullptr;
}



#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))

int main(int argc, char *argv[])
{
    cout << "=======================" << endl;
    int myvar = 25;

    int* foo = &myvar;
    int bar = *foo;

    bar++;

    cout << "bar: " << bar << endl;
    cout << "myvar: " << myvar << endl;

    int* foo2 = foo;
    (*foo2)++;

    cout << "myvar: " << myvar << endl;

    int * p1, p2;

    cout << "=======================" << endl;

    cout << foo << endl;
    cout << foo+1 << endl;

    short cval = 42;
    short* cp = &cval;

    cout << cp << endl;
    cout << cp+1 << endl;

    cout << "=======================" << endl;
    {
        int x;
              int *       p1 = &x;
        const int *       p2 = &x;
              int * const p3 = &x;
        const int * const p4 = &x;

//        *p1 = 1;
//         p1 = nullptr;

//        *p2 = 1;
//         p2 = nullptr;

//        *p3 = 1;
//         p3 = nullptr;

//        *p4 = 1;
//         p4 = nullptr;
    }

    cout << "=======================" << endl;
    {
        int p[] = {1,2,3};

        cout << p << endl;
        cout << &p[0] << endl;


        cout << p[1] << endl;
        cout << *(p+1) << endl;

        cout << p[1] << endl;
        cout << 1[p] << endl;

        int n=1;
        cout << *(p+n) << endl;


        cout << ++(*p) << endl;
        cout << (*p)++ << endl;
        cout << p[0] << endl;

        cout << *(p) << endl;
    }

    cout << "=======================" << endl;
    {
        int p[] = {1,2,3};
        int p2[3] = {1,2,3};
        int p3[3];
        int p4[3] = {1};
        cout << p4[1] << endl;
        cout << "ARRAY_SIZE: " << ARRAY_SIZE(p4) << endl;
    }


    cout << "=======================" << endl;
    {
        int* p = new int;
        int* p2 = new int();
        int* p3 = new int(42);
        int* p4 = new int[42];

        cout << "p: " << p << ", *p=" << *p << endl;
        cout << "p2: " << p2 << ", *p2=" << *p2 << endl;
        cout << "p3: " << p3 << ", *p3=" << *p3 << endl;
        cout << "p4: " << p4 << ", *p4=" << *p4 << endl;

        // ERROR!?
        delete p;
        delete p2;
        delete p3;
        delete p4;
    }

    cout << "========================" << endl;
    {
        int* p = new int[200];
        cout << "ARRAY_SIZE: " << ARRAY_SIZE(p) << endl;
        cout << p << ", len:" << *((size_t*)p - 1) << endl;
        delete[] p;
    }

    cout << "=======================" << endl;
    {
        char foo1[] = {'h','e','l','l','o'};
        char foo2[] = "hello";
        char* foo = "hello";
        char foo3[10] = {0};

        cout << foo[0] << endl;
        cout << *(foo+1) << endl;

        cout << "ARRAY_SIZE: " << ARRAY_SIZE(foo) << endl;
        cout << "ARRAY_SIZE: " << ARRAY_SIZE(foo2) << endl;
        cout << "ARRAY_SIZE: " << ARRAY_SIZE(foo3) << endl;

        ////foo[0] = 'H';
        foo2[0] = 'H';
        cout << "passed" << endl;

        cout << foo[5] << endl;
        cout << *(foo2+5) << endl;

        cout << "strlen(foo): " << strlen(foo) << endl;
        cout << "strlen(foo2): " << strlen(foo2) << endl;
        cout << "strlen(foo1): " << strlen(foo1) << endl;
        cout << "ARRAY_SIZE(foo1): " << ARRAY_SIZE(foo1) << endl;
    }

    cout << "=======================" << endl;
    {
        char foo[] = "hello";
        char * foo2 = strdup(foo);

        foo[0] = 'H';

        cout << foo << endl;
        cout << foo2 << endl;

        cout << foo << ", len:" << strlen(foo) << endl;
        cout << foo2 << ", len:" << strlen(foo2) << endl;

        free(foo2);
    }

    cout << "=======================" << endl;
    {
        char * foo = "hello";
        char * foo2 = new char[strlen(foo)+1]();

        strcpy(foo2, foo);
        cout << foo << ", len:" << strlen(foo) << endl;
        cout << foo2 << ", len:" << strlen(foo2) << endl;
        cout << "cmp:" << strcmp(foo2, foo) << endl;

        delete[] foo2;
    }


    cout << "=======================" << endl;
    {
        char * foo = "hello";
        char * foo2 = new char[strlen(foo)+1];
        char * foo3 = new char;

        int len = strlen(foo);

        while (*foo2++ = *foo++);

        foo -= (len+1);
        foo2 -= (len+1);

        cout << foo << ", len:" << strlen(foo) << endl;
        cout << foo2 << ", len:" << strlen(foo2) << endl;

//        cout << foo2 << ", len:" << *((size_t*)foo2 - 1) << endl;
//        cout << foo2 << ", len:" << *((size_t*)foo3 - 1) << endl;

        delete[] foo2;
        delete foo3;
    }

    cout << "=======================" << endl;
    {
        int value = 42;
        int * foo = &value;

        cout << "foo: " << foo << endl;
        setNull(foo);
        cout << "foo: " << foo << endl;
//        func(&foo);
//        cout << "foo: " << foo << endl;
//        func2(foo);
//        cout << "foo: " << foo << endl;
    }

    cout << "=======================" << endl;
    {
        char c = 'a';
        int i = c;

        cout << "c: " << c << endl;
        cout << "i: " << i << endl;

        i++;
        c = i;

        cout << "c: " << c << endl;
        cout << "i: " << i << endl;

        int array[100] = {0};
        array[c] = 42;
    }

    cout << "=======================" << endl;
    {
        wchar_t wc = 'ы';
        char c = 'a';
        int i = wc;

        cout << WCHAR_MIN << endl;
        cout << WCHAR_MAX << endl;
        cout << "wc: " << wc << endl;
        cout << "c: " << c << endl;
        cout << "i: " << i << endl;

        i++;
        wc = i;

        cout << "c: " << wc << endl;
        cout << "i: " << i << endl;
    }

    return 0;
}
