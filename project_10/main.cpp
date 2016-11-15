#include <iostream>
#include "logger.h"
#include "a.h"

using namespace std;

// SIOF
// Drawback of func solution
// Motivation for MI: ABC, adapter
// Diamond

//H/W
// implement thread-safe Singleton pattern
// create diamond-like hierarchy


class base
{

public:
    int m;
    base( int n )
        : m(n)
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }

    virtual void action()
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }
};

class derived1 : public virtual base
{
public:
    derived1( int n )
        : base( n )
    {}

    void action()
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }
};

class derived2 : public virtual base
{
public:
    derived2( int n )
        : base( n )
    {}

    void action()
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }
};


class sub_derived : public derived1, public derived2
{
public:
    sub_derived( int n )
        : base( n )
        , derived1( n )
        , derived2( n )
//        : derived1( n )
//        , derived2( n )
    {}

    void action()
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }
};

A a1;

int main(int argc, char* argv[])
{
    cout << "Entered main!" << endl;

//    {
////        A a1;
//        return 0;
//    }

    {
        sub_derived a(5);
        a.action();
        cout << "a.m: " << a.m << endl;
    }

    {
        derived1* d1 = new sub_derived(5);
        d1->action();

        derived2* d2 = new sub_derived(5);
        d2->action();

        sub_derived a (5);
        a.derived1::action();
        a.derived2::action();
    }

    return 0;
}
