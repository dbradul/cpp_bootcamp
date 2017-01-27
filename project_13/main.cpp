#include <iostream>
#include <memory>
#include <cassert>
#include <thread>

#include "unique_ptr.h"
#include "shared_ptr.h"

// RAII
// Don't use auto_ptr
// Inheritance works!
// Strategy: unique->shared
// assign pointer to shared_ptr as soon as it is created.
//  This allows to avoid subtle bugs


using namespace std;


namespace RAII{


    class File_handle
    {
        FILE* p;

    public:
        File_handle(const char* n, const char* a)
        {
            cout << __PRETTY_FUNCTION__ << endl;
            p = fopen(n,a);
            if (p==0) throw -1;//std::exception(-1);
        }
        File_handle(FILE* pp)
        {
            p = pp;
            if (p==0) throw -1;//std::exception(-1);
        }

        ~File_handle()
        {
            cout << __PRETTY_FUNCTION__ << endl;
            fclose(p);
        }

        operator FILE*() { return p; }
        // ...
    };

}



struct A {

    A ():
        value(0)
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }

    A (int value):
        value(value)
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }

    ~A ()
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }

    virtual void foo()
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }

    int value;
};


struct B : A
{
    B (int value=21):
        A(value),
        value_derived(value)
    { }

    void foo() override
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }

    int value_derived;
};


struct C{
    int value_c;
};


template<class T>
struct Deleter
{
    void operator()(T* t)
    {
        std::cout << "deleting..." << std::endl;
        delete[] t;
    }
};


template<typename TO, typename FROM>
my::unique_ptr<TO> static_unique_pointer_cast(my::unique_ptr<FROM>&& ptr)
{
    return my::unique_ptr<TO>(static_cast<TO*>(ptr.release()));
}

template<typename TO, typename FROM>
my::shared_ptr<TO> static_pointer_cast(my::shared_ptr<FROM>& ptr)
{
    return my::shared_ptr<TO>(static_cast<TO*>(ptr.get()));
}

unique_ptr<A> test_unique_pointer_return()
{
    unique_ptr<B> ptrb(new B);
    return move(ptrb);
}


void std_unique_pointer()
{
    // handle both raw ptr and arrays
    // move ownership
    // deleter
    // when to use
    //  - factories
    //  - exclusive ownership: file, mutexes
    //  - pimpl

    std::unique_ptr<A> ptr (new A(42));
    cout << "ptr->value: " << ptr->value << endl;

    A* pA = new A;
    ptr.reset(pA);
    cout << "should call dtor so far" << endl;
    ptr.reset(nullptr);
    cout << "should call 2nd dtor so far" << endl;


    ptr.reset(new A(42));
    //unique_ptr<A> ptr2 = ptr;
    cout << "ptr: " << ptr.get()->value << endl;
    cout << "ptr: " << ptr->value << endl;

    unique_ptr<A> ptr3 = move(ptr);
    cout << "ptr: " << ptr.get() << endl;
    cout << "ptr3: " << ptr3.get()->value << endl;
    A a = *(ptr3.get());
    ptr3.reset(nullptr);
    cout << "a.value" << a.value << endl;

    A* raw_ptr = ptr3.release();
    delete raw_ptr;

    {
        // but thoroughly consider using vector or std::array instead
        unique_ptr<A[]> ptr (new A[10]);
        unique_ptr<A[], Deleter<A>> ptr2 (new A[10]);
    }
}


void std_weak_pointer()
{
    // motivation
    //  - caches, observers, cyclic dependencies
    // tightly connected with shared_ptr
    // http://prnt.sc/dzpsij

    shared_ptr<A> ptr = make_shared<A>(42);
    cout << ptr.use_count() << endl;
    weak_ptr<A> ptr_weak(ptr);
    cout << ptr.use_count() << endl;

    weak_ptr<B> weakB;
    {
        shared_ptr<B> ptrBB(make_shared<B>(42));
        weakB = ptrBB;
    }
    cout << "Expired: " << weakB.expired() << endl;

    {
        shared_ptr<B> ptrBB(make_shared<B>(42));
        weak_ptr<B> weakB = ptrBB;

        if (!weakB.expired())
        {
            shared_ptr<B> objPtr = weakB.lock();
            if(objPtr)
            {
                cout << "from weak_ptr: " << objPtr->value << endl;
            }
        }
    }

    return;
}

void foo(std::shared_ptr<A> ptr)
{
    cout << "inside func: " << ptr.use_count() << endl;
}

void std_shared_pointer()
{
    // motivation
    //  - share resources
    // move can be applied
    // std::make_shared is preferred to use
    // when to use
    //  - shared resources
    // overhead
    //  - more memory
    //  - memory for control block (ref_counter) is allocated
    //  - read/write are costly operations
    // pitfalls:
    //  - one raw ptr, 2+ sh_ptr

    std::shared_ptr<A> ptr_std(new A);
    std::shared_ptr<A> ptr_std2 = ptr_std;
    cout << ptr_std.use_count() << endl;
    cout << ptr_std2.use_count() << endl;
    ptr_std.reset(); // unref
    cout << ptr_std.use_count() << endl;
    cout << ptr_std2.use_count() << endl;

    foo(ptr_std2);

    cout << ptr_std2.use_count() << endl;
    cout << "after move: " << endl;
    std::shared_ptr<A> ptr_std3 = move(ptr_std2);
    cout << ptr_std2.use_count() << endl;
    cout << ptr_std3.use_count() << endl;

    cout << "unique ptr: " << endl;
    std::unique_ptr<A> ptr_uniq(new A);
    std::shared_ptr<A> ptr_std4 = move(ptr_uniq);

    cout << ptr_std4.use_count() << endl;
    cout << ptr_uniq.get() << endl;
    cout << ptr_std4.use_count() << endl;
    cout << ptr_uniq.get() << endl;
    cout << ptr_std4.get() << endl;

    shared_ptr<A> ptr = shared_ptr<A>(new A);
    shared_ptr<B> ptrB = shared_ptr<B>(new B);
    shared_ptr<A> ptrAB = shared_ptr<A>(new B);

    ptr->foo();
    ptrB->foo();
    ptrAB->foo();

    assert(   ptr.use_count()==1
           && ptrB.use_count()==1
           && ptrAB.use_count()==1);

    ptr = std::static_pointer_cast<A>(ptrB);
    ptr->foo();

    assert(   ptr.use_count()==2
           && ptrB.use_count()==2
           && ptrAB.use_count()==1);

    {
        shared_ptr<A> ptr  (new A(43));
        shared_ptr<A> ptr2  = std::make_shared<A>(43);
    }

    return ;
}

void my_shared_pointer()
{
    using my::shared_ptr;

    shared_ptr<A> ptr = shared_ptr<A>(new A);
    shared_ptr<B> ptrB = shared_ptr<B>(new B);
    shared_ptr<A> ptrAB = shared_ptr<A>(new B);
    shared_ptr<B> ptrBB;

    assert (ptr.use_count()  == 1);
    assert (ptrB.use_count() == 1);
    assert (ptrAB.use_count()== 1);
    assert (ptrBB.use_count()== 0);

    ptrBB = ptrB;
    assert (ptrBB.use_count()== 2);


    assert(ptrAB != ptr);
    assert(ptr != nullptr);
    ptrAB = ptr;

    assert(ptr.use_count()   == 2);
    assert(ptrAB.use_count() == ptr.use_count());

    shared_ptr<A> ptrAB2 = ptrAB;

    assert(ptrAB.use_count() == ptr.use_count());
    assert(ptrAB2.use_count() == ptrAB.use_count());
    assert(ptrAB2.use_count() == 3);

    assert(ptrAB == ptr);
    assert(ptrAB.get() == ptr.get());
    assert(ptrAB->value == ptr->value);

    int numCnt = ptr.use_count();

    {
        shared_ptr<A> ptr2 = ptr;
    }

    if (ptrAB) { }

    assert(ptr.use_count() == numCnt);

    shared_ptr<A> ptr2 = ptr;
    ptr.reset(nullptr);
    assert(ptr2.use_count() == numCnt);

}

void my_unique_pointer()
{
    // deref unintialized pointer
    using my::unique_ptr;

    // MOVE
    const int init_value = 42;
    unique_ptr<A> aptr0(new B(init_value));
    //cout << aptr0->value << endl;
    assert((*aptr0).value == init_value);

    unique_ptr<A> aptr1(new B(init_value));
    assert (aptr1 != nullptr);
    unique_ptr<A> aptr2 = move(aptr1);
    unique_ptr<A> aptr3  (move(aptr2));

    unique_ptr<A> aptr4;
    aptr4 = move(aptr3);
    aptr3 = move(aptr4);

    assert (aptr1 == nullptr);
    assert (aptr2 == nullptr);
    assert (aptr3 != nullptr);
    assert (aptr4 == nullptr);

    assert (aptr1.get() == nullptr);
    assert (aptr2.get() == nullptr);
    assert (aptr3.get() != nullptr);

    assert (static_unique_pointer_cast<B>(move(aptr3))->value_derived == init_value);
    assert (aptr3.get() == nullptr);
    assert (aptr3 == nullptr);
}


int main(int argc, char *argv[])
{
    {
        using namespace RAII;
        ////http://prnt.sc/dzp3f2

        //File_handle a("/tmp/null", "a+");
    }

    std_unique_pointer();

    std_shared_pointer();

    std_weak_pointer();

    my_unique_pointer();

    my_shared_pointer();

    cout << "SUCCESS" << endl;

    return 0;
}
