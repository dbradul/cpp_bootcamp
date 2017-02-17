#include <iostream>
#include <memory>

#include "object.h"
#include "calldispatcher.h"

using namespace std;

class A : public Object
{
private:
    void dispatchMethod(const string& methodName) override
    {
        if ("onFoo" == methodName)
        {
            onFoo();
        }
        else if ("onBar" == methodName)
        {
            onBar();
        }
        else if ("onBaz" == methodName)
        {
            onBaz();
        }
        else
        {

        }
    }

public:

    A(CallDispatcher::Ptr& dispatcher)
        : Object(dispatcher)
    {
    }

public /*slots*/:

    void onFoo()
    {
        cout << __PRETTY_FUNCTION__ << ": " << this << endl;
    }

    void onBar()
    {
        cout << __PRETTY_FUNCTION__ << ": " << this << endl;
    }

    void onBaz()
    {
        cout << __PRETTY_FUNCTION__ << ": " << this << endl;
    }
};


class B : public Object
{

public:
    B(CallDispatcher::Ptr& dispatcher)
        : Object(dispatcher)
    {
    }

public /*signals*/:

    void foo()
    {
        m_dispatcher->sendSignal(this, "foo");
    }

    void bar()
    {
        m_dispatcher->sendSignal(this, "bar");
    }

    void baz()
    {
        m_dispatcher->sendSignal(this, "baz");
    }
};




int main(int argc, char *argv[])
{
    auto d = make_shared<CallDispatcher>();

    A a1(d);
    A a2(d);
    B b1(d);
    B b2(d);

    d->connect(&b1, "foo", &a1, "onFoo");

    // 1 signal => many slots
    d->connect(&b1, "bar", &a1, "onBar");
    d->connect(&b1, "bar", &a1, "onBaz"); //names shouldn't nesseraly match
    d->connect(&b1, "bar", &a2, "onBar");

    // 2 signals => 1 slot
    d->connect(&b1, "baz", &a2, "onBaz");
    d->connect(&b2, "baz", &a2, "onBaz");

    b1.foo(); // a1.onFoo()
    b1.bar(); // a1.onBar(), a1.onBaz(), a2.onBar()
    b1.baz(); // a2.onBaz()

    b2.foo(); // <nothing>
    b2.bar(); // <nothing>
    b2.baz(); // a2.onBaz()

    d->disconnect(&b1, "foo", &a1, "onFoo");
    b1.foo(); // <nothing>

    return 0;
}
