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


namespace regular_inheritance
{

class base
{

public:
    int m;
    int arr[10];

    base( int n )
        : m(n)
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }

    void action()
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }

//    virtual void v_action()
//    {
//        cout << __PRETTY_FUNCTION__ << endl;
//    }
};

class derived1 : public /*virtual*/ base
{
public:

    double d;


    derived1( int n )
        : base( n )
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }

//    void action()
//    {
//        cout << __PRETTY_FUNCTION__ << endl;
//    }

//    void v_action() override
//    {
//        cout << __PRETTY_FUNCTION__ << endl;
//    }
};

class derived2 : public /*virtual*/ base
{
public:
    derived2( int n )
        : base( n )
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }

//    void action()
//    {
//        cout << __PRETTY_FUNCTION__ << endl;
//    }

//    void v_action() override
//    {
//        cout << __PRETTY_FUNCTION__ << endl;
//    }
};


//class sub_derived : public virtual derived1
//                  , public virtual derived2
class sub_derived : public derived1
                  , public derived2
{
public:

    sub_derived( int n )
//        : base( n )
//        , derived1( n )
        : derived1( n )
        , derived2( n )
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }

//    void v_action() override
//    {
//        cout << __PRETTY_FUNCTION__ << endl;
//    }


//    void action()
//    {
//        cout << __PRETTY_FUNCTION__ << endl;
//    }
};

}





namespace virtual_inheritance
{

class base
{

public:
    int m;
    int arr[10];

    base( int n )
        : m(n)
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }

    void action()
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }

    virtual void v_action()
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }
};

class derived1 : public virtual base
{
public:

    double d;

    derived1( int n )
        : base( n )
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }

//    void action()
//    {
//        cout << __PRETTY_FUNCTION__ << endl;
//    }

//    void v_action() override
//    {
//        cout << __PRETTY_FUNCTION__ << endl;
//    }
};

class derived2 : public virtual base
{
public:
    derived2( int n )
        : base( n )
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }

//    void action()
//    {
//        cout << __PRETTY_FUNCTION__ << endl;
//    }

//    void v_action() override
//    {
//        cout << __PRETTY_FUNCTION__ << endl;
//    }
};


//class sub_derived : public virtual derived1
//                  , public virtual derived2
class sub_derived1 : public derived1
                   , public derived2
{
public:

    sub_derived1( int n )
        : base( n )
        , derived1( n )
//        : derived1( n )
        , derived2( n )
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }

    void v_action() override
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }

//    void action()
//    {
//        cout << __PRETTY_FUNCTION__ << endl;
//    }
};

//class sub_derived : public virtual derived1
//                  , public virtual derived2
class sub_derived2 : public derived1
                   , public derived2
{
public:

    sub_derived2( int n )
        : base( n )
        , derived1( n )
//        : derived1( n )
        , derived2( n )
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }

    void v_action() override
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }

//    void action()
//    {
//        cout << __PRETTY_FUNCTION__ << endl;
//    }
};

class sub_sub_derived : public sub_derived1
                      , public sub_derived2
{
public:

    sub_sub_derived( int n )
        : base( n )
        , sub_derived1( n )
//        : sub_derived1( n )
        , sub_derived2( n )
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }

    void v_action() override
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }

//    void action()
//    {
//        cout << __PRETTY_FUNCTION__ << endl;
//    }
};

}




A a1;

int main(int argc, char* argv[])
{
    cout << "Entered main!" << endl;

//    {
////        A a1;
//        return 0;
//    }

    {
        cout << "########### regular_inheritance ###########" << endl;

        using namespace regular_inheritance;
        cout << sizeof(base) << endl;
        cout << sizeof(derived1) << endl;
        cout << sizeof(derived2) << endl;
        cout << sizeof(sub_derived) << endl;
        sub_derived a(5);
//        a.v_action();
//        a.action();
        //cout << "a.m: " << a.m << endl;
    }

    {
        cout << "########### virtual_inheritance ###########" << endl;

        using namespace virtual_inheritance;
        cout << sizeof(base) << endl;
        cout << sizeof(derived1) << endl;
        cout << sizeof(derived2) << endl;
        cout << sizeof(sub_derived1) << endl;
        cout << sizeof(sub_derived2) << endl;
        cout << sizeof(sub_sub_derived) << endl;
//        sub_derived1 a(5);
//        a.v_action();
//        a.action();


        sub_sub_derived aa(5);
        aa.v_action();
        aa.action();
        //cout << "a.m: " << a.m << endl;
    }



//    {
//        derived1* d1 = new sub_derived(5);
//        d1->action();

//        derived2* d2 = new sub_derived(5);
//        d2->action();

//        sub_derived a (5);
//        a.derived1::action();
//        a.derived2::action();
//    }

    return 0;
}
