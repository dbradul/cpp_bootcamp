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

// Unique_ptr:
//  - embedded as a member data
//  - limited scope of usage
//  - containers
//  - when meaningless to copy resourse (file handler, mutex, socket)
//  - can't be copied, can only be moved!
//  - notable members:
//        -- release() // stop ownership
//        -- reset() // premature deletion
// Shared
//  - many-to-one
//
// Weak pointer
//    - can be null, use lock to check
//

using namespace std;

static int gTest;

struct A {

    A ():
        value(0)
    {
        gTest++;
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


    int value;
};


struct B : A
{
    B (int value=21):
        A(value),
        value_derived(value)
    { }

    int value_derived;
};


struct C{
    int value_c;
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
    return ptrb;
}

void my_test_unique_pointer()
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

    // CONTAINERS
    vector<unique_ptr<A>> v;
    v.push_back(move(aptr0));
    assert (aptr0.get() == nullptr);
    assert (aptr0 == nullptr);

    vector<unique_ptr<A>> v2(move(v));
    assert (v2[0]->value == init_value);
    ///cout << v[0]->value << endl;

    // ARRAY
    unique_ptr<A, my::default_deleter<A[]>> arr_ptr(new A[10]);
    arr_ptr.reset(nullptr);
}

void test_shared_pointer()
{
    shared_ptr<A> ptr = shared_ptr<A>(new A);
    shared_ptr<B> ptrB = shared_ptr<B>(new B);
    shared_ptr<A> ptrAB = shared_ptr<A>(new B);


    std::shared_ptr<B> ptrBB2;
    assert (ptrBB2 == nullptr);
    //ptrBB = static_pointer_cast<B>(ptrAB);

    assert(   ptr.use_count()==1
           && ptrB.use_count()==1
           && ptrAB.use_count()==1);

    ptrAB = ptr;

    assert(   ptr.use_count()==2
           && ptrB.use_count()==1
           && ptrAB.use_count()==2);

    ptr = std::static_pointer_cast<A>(ptrB);

    assert(   ptr.use_count()==2
           && ptrB.use_count()==2
           && ptrAB.use_count()==1);

    {
        shared_ptr<A> ptr2 = ptr;

        assert(ptr.use_count()==3);
    }

    assert(ptr.use_count()==2);

    weak_ptr<B> weakB;
    {
        shared_ptr<B> ptrBB(make_shared<B>(42));
        weakB = ptrBB;
    }
    cout << "Expired: " << weakB.expired() << endl;

    ptr->value = 42;

    cout << ptr->value << endl;


    cout << (ptr.get() ? "!<null>":"<null>")<< endl;
    cout << ptr->value << endl;
    cout << ptr.get()->value << endl;

    {
        std::shared_ptr<A> ptr_to_base(std::make_shared<B>(42));

        // ptr_to_base->value_derived(); // Error won't compile

        cout << std::static_pointer_cast<B>(ptr_to_base)->value_derived << endl; // OK
        // (constructs a temporary shared_ptr, then calls operator->)

        cout << static_cast<B*>(ptr_to_base.get())->value_derived << endl; // also OK
        // (direct cast, does not construct a temporary shared_ptr)
    }
}

void my_test_shared_pointer()
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


//    ptrBB = static_pointer_cast<B>(ptrAB);
//    weak_ptr<B> weakB = ptrBB; // TBD: add my weak_ptr
//    cout << "Expired: " << weakB.expired() << endl;

}


int main(int argc, char *argv[])
{
    test_shared_pointer();

    my_test_unique_pointer();

    my_test_shared_pointer();

    {
        int* p = new int(42);

        //shared_ptr<int> pInt(new int(42));
        shared_ptr<int> pInt(std::make_shared<int>(42));
        cout << pInt.use_count() << endl; // ref-count is 1

        shared_ptr<int> pInt2(p);
        cout << pInt2.use_count() << endl; // ref-count is 1
    }

    cout << "SUCCESS" << endl;


    return 0;
}
