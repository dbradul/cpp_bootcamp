#include <iostream>
#include <cassert>
#include <type_traits>

using namespace std;

#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))



template<typename T>
T Reduce(const T& (*pFunc)(const T&, const T&), T* arr, size_t size)
{
    T value = arr[0];
    for(size_t i = 1; i < size; i++)
    {
        value = pFunc(value, arr[i]);
    }

    return value;
}


struct Box
{
    int size;
};

namespace std{

template<>
const Box& min<Box>(const Box& value1, const Box& value2)
{
    return value1.size < value2.size ? value1 : value2;
}

template<>
const Box& max<Box>(const Box& value1, const Box& value2)
{
    return value1.size > value2.size ? value1 : value2;
}

}



namespace explicit_keyword {

class Int {
    int m_value;

public:

    explicit Int (int value=0)
        : m_value(value)
    {
    }

    explicit operator int() const
    {
        return m_value;
    }
};

    void foo(int)
    {}

}

namespace implicit_casting {

    void foo(int)
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }

    void foo2(float)
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }

    template<typename T>
    void fooT(T)
    {
        static_assert(std::is_integral<T>::value, "Type T must be integral");
        cout << __PRETTY_FUNCTION__ << endl;
    }

//    void foo(float);
    void foo(float) = delete;
    void foo2(int) = delete;
}

namespace static_cast_ns {

    int foo()
    {
        return 1;
    }

    struct A
    {
        A(){}
//        template<typename T> A(T const& d) = delete;
//        template<typename T> A const& operator=(T const& rhs) = delete;
    };
    struct B:A{};
    struct D{};
}

namespace const_cast_ns {

    int foo(int value)
    {
        return ++value;
    }

    int foo2(int& value)
    {
        return ++value;
    }


    const int& f_c(const int obj)
    {
        static int g_obj = obj;
        return g_obj;
    }

    int& f(int obj)
    {
        const int tmp = obj;
        return const_cast<int&>(f(tmp));
    }

    void f_cr(const int& obj)
    {
    }

//    const Objects ref = obj;
//    return const_cast<Object&>( foo(ref));
}

namespace dynamic_cast_ns {

    struct _A{};
    struct _B:_A{};

    struct A{virtual ~A(){}};
    struct B:A{};
    struct D{virtual ~D(){}};
}

namespace reinterpret_cast_ns {

    bool isLittleEndian()
    {
        const int i = 1;
        const char* const c = reinterpret_cast<const char*>(&i);
        return (*c == 1);
    }

}


int main(int argc, char *argv[])
{
    {
        int arrInt[] = { 1, 2, 3 };
        int minInt = Reduce(min, arrInt, ARRAY_SIZE(arrInt));
        int maxInt = Reduce(max, arrInt, ARRAY_SIZE(arrInt));
        cout << "min int: " << minInt << endl;
        cout << "max int: " << maxInt << endl;
    }

    {
        Box arrBox[] = { {1}, {2}, {3} };

        Box minBox = Reduce(min, arrBox, ARRAY_SIZE(arrBox));
        Box maxBox = Reduce(max, arrBox, ARRAY_SIZE(arrBox));

        cout << "min Box: " << minBox.size << endl;
        cout << "max Box: " << maxBox.size << endl;
    }

//    return 0;

    {
        using namespace implicit_casting;
        using namespace static_cast_ns;

        long l = 42;
        int i = l;
        float d = 42.;
        float f = i; //why is the warning
        f = 42.42;   //why is the warning
        i = f;       //why?
        i = 1<<31;
        short s = i; //why?
        short s2 = 1; //why??????
        cout << f << endl;
        cout << i << endl;
        cout << s << endl;
        cout << s2 << endl;
        cout << d << endl;

        int unsigned v1 = -1u;
        int signed v2 = v1;
        cout << "v1: " << v1 << endl;
        cout << "v2: " << v2 << endl;
        v1 = v2;
        cout << "v1: " << v1 << endl;
        cout << "v1<v2: " << (v1<v2) << endl;

//        foo(f);
//        foo2(i);
//        fooT(f);
//        fooT(i);

        A a;
        B b;
        a = b; //slicing

        A* p_a = new A();
        B* p_b = new B();
        p_a = p_b;
        //p_b = p_a;
    }


    {
        using namespace explicit_keyword;

        Int a(42);
        int b = static_cast<int>(a);
        foo(static_cast<int>(a));
        cout << "b: " << b << endl;
    }

    {
        using namespace  static_cast_ns;

        int i = 42;
        float f = static_cast<float>(i);
        i = static_cast<int>(f);
        cout << f << endl;
        cout << i << endl;

        static_cast<void>(foo());

        A a;
        B b;
        a = b;
//        b = static_cast<A>(a);

        A* p_a  = new A();
        A* p_a2 = new B();
        B* p_b = new B();
        p_a = p_b;
        p_b = static_cast<B*>(p_a); //UB
        p_b = static_cast<B*>(p_a2);
        //D* p_d = new D();
        //p_d = static_cast<D*>(p_a);

        int* p_i = &i;
        float* p_f = &f;
//        p_i = static_cast<int*>(p_f);
//        p_f = static_cast<float*>(p_i);
        p_i = static_cast<int*>(static_cast<void*>(p_f));
        cout << "*p_i: " << *p_i << endl;
    }


    {
        using namespace  const_cast_ns;

        const int i = 42;
        int j = i;
        //foo2(const_cast<int>(i));
        //foo2(i);
        foo2(j);

        int i2 = 42;
        const int * pi = &i2;
        int* j2 = const_cast<int *> (pi);

        int i3 = 42;
        const int& pi3 = i3;
        int& j3 = const_cast<int&> (pi3);
        foo2(const_cast<int&>(pi3));

        // UB
        const int val = 10;
        const char str[] = "abcxyz";
        int *ptr1 = const_cast <int *>(&val);
        cout << "ptr: " << ++(*ptr1) << endl;

        char *ptr2 = const_cast <char *>(str);
        ptr2[0]='Z';
        cout << "ptr2: " << ptr2 << endl;

    }

    {
        using namespace  dynamic_cast_ns;

        {
            _A* p_a = new _B();
            _B* p_b = new _B();
            p_a = p_b;
            p_b = static_cast<_B*>(p_a); //not poymorphic
            ///p_b = dynamic_cast<_B*>(p_a); //not poymorphic
        }

        A* p_a = new A();
        B* p_b = new B();
        D* p_d = new D();

        p_b = dynamic_cast<B*>(p_a);
        cout << "p_b = " << p_b << endl;

        p_b = dynamic_cast<B*>(p_d);
        cout << "p_b = " << p_b << endl;

        p_a = new B();
        p_b = dynamic_cast<B*>(p_a);
        cout << "p_b = " << p_b << endl;


        {
             A& ref_a = *p_a;
             B& ref_b = *p_b;
             D& ref_d = *p_d;

             try
             {
                 ref_b = dynamic_cast< B&>(ref_a);
                 cout << "p_b = " << &dynamic_cast< B&>(ref_a) << endl;

                 ref_b = dynamic_cast< B&>(ref_d);
                 cout << "p_b = " << &dynamic_cast< B&>(ref_d) << endl;
             }
             catch (exception& ex)
             {
                 cout << "Exception (dynamic cast): " << ex.what() << endl;
             }


             A& p_a2 = *p_b;
             ref_b = dynamic_cast< B&>(p_a2);
             cout << "p_b = " << &dynamic_cast< B&>(p_a2) << endl;
        }

    }

    {
        using namespace reinterpret_cast_ns;

        cout << "isLittleEndian: " << isLittleEndian() << endl;

        float f = 42;
        float* p_f = &f;
//        p_i = static_cast<int*>(p_f);
//        p_f = static_cast<float*>(p_i);
        int* p_i = static_cast<int*>(static_cast<void*>(p_f));
        int* p_i_2 = reinterpret_cast<int*>(p_f);

        cout << "*p_i: " << *p_i << endl;
        cout << "*p_i_2: " << *p_i_2 << endl;
    }

    {
        string s("abc");
        cout << "s.size(): " << s.size() << ", s=" << s << endl;
        string s1 = s;
        cout << "s.size(): " << s.size() << ", s=" << s << endl;
        cout << "s1.size(): " << s1.size() << ", s1=" << s1 << endl;

        string s2 = static_cast<string&&>(s);
        cout << "s.size(): " << s.size() << ", s=" << s << endl;
        cout << "s2.size(): " << s2.size() << ", s2=" << s2 << endl;

        string s4 = move(s2);
        cout << "s2.size(): " << s2.size() << ", s2=" << s2 << endl;
        cout << "s4.size(): " << s4.size() << ", s4=" << s4 << endl;
    }

    return 0;
}
