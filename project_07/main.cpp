#include <iostream>

using namespace std;

//- virtual destructor
//- virtual constructor
//- anonymous namespaces
//- pure virtual function call
//- initialization vs assignment
//- default constuctor. its importance when deal with containers (arrays)
//- default destructor.
//- explicit a=5
//- what happens when you see a=b? copy? sharing?
//- what about A a = b?
//- copy constructor and assignment operator. common functionality.
//- when to implement, when to prohibit
//- rule of three
//- NRVO (implicit hidden param)

namespace
{
   int var_a;
}

namespace virtual_ctor
{
    class A
    {
    public:
        virtual ~A(){}

        virtual A* clone() = 0;
    };

    class B : public A
    {
    public:

        B()
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }

        B(const B& rhs)
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }

        A* clone()
        {
            return new B(*this);
        }

        ~B(){}
    };

    class C : public A
    {
    public:

        C()
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }

        C(const C& rhs)
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }

        A* clone()
        {
            return new C(*this);
        }

        ~C(){}
    };
}

namespace pure_virtual_call
{
    class A
    {
    public:
        virtual ~A(){}

        A()
        {
            cout << __PRETTY_FUNCTION__ << endl;
            //clone();
            //pre_clone();
        }

        void pre_clone()
        {
            clone();
        }

        virtual A* clone() = 0;
    };

    class B : public A
    {
    public:

        B()
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }

        B(const B& rhs)
        {

            cout << __PRETTY_FUNCTION__ << endl;
        }

        A* clone()
        {
            return new B(*this);
        }

        ~B(){}
    };
}

namespace ctor_order
{
    class A
    {

    public:
        A()
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }

        ~A()
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }
    };

    class B
    {
        A m_a;

        public:
        B()
            : m_a()
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }

        ~B()
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }
    };

    class C
    {
        A m_a;
        B m_b;

        public:
        C()
            : m_a()
            , m_b()
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }

        ~C()
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }
    };
}

namespace assignment_init
{
    class A
    {
        int m_data;
        //int& m_data2;

    public:
        A()
            : m_data(0)
            //, m_data2(m_data)
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }

        A(const A& rhs)
            : m_data(rhs.m_data)
            //, m_data2(rhs.m_data2)
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }

        A(int value)
            : m_data(value)
            //, m_data2(value)
        {
            cout << __PRETTY_FUNCTION__ << endl;
            //m_data2 = value;
        }

        ~A()
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }
    };

    class B
    {
        A m_a;

        public:
        B()
           //: m_a(42)
        {
            cout << __PRETTY_FUNCTION__ << endl;
            m_a = A(43);
        }

        ~B()
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }
    };

}

namespace default_ctor
{
    class A
    {
        int m_data;

    public:
        A()
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }

        A(const A& rhs)
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }

        A(int value /*= 10*/)
           : m_data(value)
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }

        ~A()
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }
    };
}

namespace explicit_ctor
{
    class A
    {
        int m_data;

    public:
        A()
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }

        A(const A& rhs)
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }

        explicit A(int value /* = 10*/)
           : m_data(value)
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }

        ~A()
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }
    };
}

namespace copy_or_ref
{
    class A
    {
    public:

        int* m_data_ptr;
        int m_data_value;

        A()
            : m_data_ptr(new int(0))
            , m_data_value(42)
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }

        explicit A(int value)
           : m_data_ptr(new int(value))
           , m_data_value(value)
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }

        A(const A& rhs)
        {
            cout << __PRETTY_FUNCTION__ << endl;
            m_data_value = rhs.m_data_value;
            m_data_ptr = new int(*rhs.m_data_ptr);
        }

        A& operator =(const A& rhs)
        {
            cout << __PRETTY_FUNCTION__ << endl;

            if (&rhs != this)
            {
                A tmp(rhs);
                swap(tmp);
            }

            return *this;
        }

        void swap(A& from)
        {
            std::swap(from.m_data_value,  m_data_value);
            std::swap(from.m_data_ptr,    m_data_ptr);
        }

        void cleanup()
        {

            if (m_data_ptr)
            {
                delete m_data_ptr;
                m_data_ptr = NULL;
            }
        }

        ~A()
        {
            cout << __PRETTY_FUNCTION__ << endl;
            cleanup();
        }
    };

    void foo(A a)
    {
        cout << a.m_data_value << endl;
        cout << a.m_data_ptr << " = " << *a.m_data_ptr << endl;
    }

    A foo()
    {
        bool cond = true;
        //A b(112);
        A a(111);
        //return a;
        return cond ? a : A (112);
        //        return A(111);
    }
}


int main(int argc, char *argv[])
{
    {
        using namespace virtual_ctor;

        A* a1 = new B;
        A* a2 = new C;

        A* a12 = a1->clone();
        A* a22 = a2->clone();

        delete a1;
        delete a2;
        delete a12;
        delete a22;
    }

    {
        using namespace pure_virtual_call;

        B* a1 = new B;
        delete a1;
    }

    {
        using namespace ctor_order;
        C c;
    }

    {
        using namespace assignment_init;
        B b;
    }

    {
        using namespace default_ctor;
        //A a;
        A a[10] = {};
    }

    {
        using namespace explicit_ctor;
        //A a = 5;
        A a2(5);
    }

    {
        using namespace copy_or_ref;

        A a(5);
        cout << a.m_data_ptr << " = " << *a.m_data_ptr << endl;
        cout << a.m_data_value << endl;
        cout << &a << endl;

        A a2 = a;
        cout << a2.m_data_ptr << " = " << *a2.m_data_ptr << endl;
        cout << a2.m_data_value << endl;
        cout << &a2 << endl;

        A a1;
        a1 = a;
        cout << a1.m_data_ptr << " = " << *a1.m_data_ptr << endl;
        cout << a1.m_data_value << endl;
    }

    {
        using namespace copy_or_ref;

        cout << "================[NRVO]===================" << endl;

//        A a(5);
//        cout << a.m_data_ptr << " = " << *a.m_data_ptr << endl;
//        cout << a.m_data_value << endl;

//        foo(a);

        A a2 = foo();
    }

    return 0;
}
