#include <iostream>
#include <limits>
#include <vector>
#include <typeinfo>
#include <iomanip>
#include <bitset>
#include <cassert>
#include "stack.h"


using namespace std;

// Motivation:
//  - avoid code duplication
//  - examlpe: code generators in web
// Theory
//  functions templates
//  class templates
//  partial specialization for functions
//  partial specialization for classes
//
// Examples
//  Containers
//  Static polymorphism
//
// Tips and trics
//  Compile-time factorial
//  Deeply-nested vectors
//
// ?Caveats
//
// H/w:
// 1. Create:
//   a) template-based stack
//   b) with specialization for booleans
// 2. Rewrite map/filter/reduce with templates



namespace function_template {

//    bool max(char val1, char val2)
//    {
//        return val1 < val2 ? val2 : val1;
//    }

//    bool max(short val1, short val2)
//    {
//        return val1 < val2 ? val2 : val1;
//    }

//    bool max(int val1, int val2)
//    {
//        return val1 < val2 ? val2 : val1;
//    }

//    bool max(long val1, long val2)
//    {
//        return val1 < val2 ? val2 : val1;
//    }

//    short max(short val1, short val2)
//    {
//        cout << __PRETTY_FUNCTION__ << endl;

//        return val1 < val2 ? val2 : val1;
//    }

//    unsigned int max(int val1, int val2)
//    {
//        cout << __PRETTY_FUNCTION__ << endl;

//        return val1 < val2 ? val2 : val1;
//    }

//    long max(unsigned long val1, unsigned long val2)
//    {
//        cout << __PRETTY_FUNCTION__ << endl;

//        return val1 < val2 ? val2 : val1;
//    }

//    float max(float val1, float val2)
//    {
//        cout << __PRETTY_FUNCTION__ << endl;

//        return val1 < val2 ? val2 : val1;
//    }

//    string max(string val1, string val2)
//    {
//        cout << __PRETTY_FUNCTION__ << endl;

//        return val1 < val2 ? val2 : val1;
//    }

    template<typename T>
    const T& max(const T& val1, const T& val2)
    {
        cout << __PRETTY_FUNCTION__ << endl;

        return val1 < val2 ? val2 : val1;
    }

//    template<>
//    const string& max<string>(const string& val1, const string& val2)
//    {
//        cout << __PRETTY_FUNCTION__ << endl;

//        return val1 < val2 ? val2 : val1;
//    }
}

namespace class_template
{

template <typename T>
struct list
{
    list* next_;
    T data_;

    list()
        : next_(nullptr)
        , data_()
    {}

    list(T data, list* next)
        : next_(next)
        , data_(data)
    {}

    void push_front(T data)
    {
        //auto elem = new list(data, next_);
        auto elem = new list<T>(data, next_);

        next_ = elem;
    }

    T pop_front()
    {
        if (next_)
        {
            list* tmp = next_;
            T data = next_->data_;
            next_ = next_->next_;
            delete tmp;
            return data;
        }

        throw runtime_error("ERROR: List is empty!");
    }

    bool is_empty()
    {
        return !next_;
    }
};

}



namespace full_specialization {

    //    using namespace class_template;

    // real life: unique_ptr, vector<bool>
//    template <>
//    struct list<bool>
//    {
//    };


    template<typename T>
    void foo(T value1)
    {
        cout << __PRETTY_FUNCTION__ << endl;
        cout << "value1: " << value1 << endl;
    }

    template<>
    void foo<double>(double value1)
    {
        cout << __PRETTY_FUNCTION__ << endl;
        cout << setprecision(15) << "value1 (d): " << value1 << endl;
    }

    template<>
    void foo<bool>(bool value1)
    {
        cout << __PRETTY_FUNCTION__ << endl;
        cout << std::boolalpha << "value1 (b): " << value1 << endl;
    }

    template<>
    void foo<string>(string value1)
    {
        cout << __PRETTY_FUNCTION__ << endl;
        cout << "value1 (s): \"" << value1  << "\"" << endl;
    }

    template<typename T, typename T2>
    void foo2()
    {
        cout << __PRETTY_FUNCTION__ << endl;
        T  value1;
        T2 value2;
        cout << "value1: " << value1 << endl;
        cout << "value2: " << value2 << endl;

        cout << "general case for " << typeid(T ).name() << " and " \
                                    << typeid(T2).name() << endl;
    }

    template<>
    void foo2<int, char>()
    {
        cout << __PRETTY_FUNCTION__ << endl;
        cout << "special case for int and char" << endl;
    }

}


namespace partial_specialization {

    template<class T>
    struct default_deleter
    {
        void operator()(T* t)
        {
            cout << "...regular delete..." << endl;
            delete t;
        }
    };

    template<class T>
    struct default_deleter<T[]>
    {
        void operator()(T* t)
        {
            cout << "...array delete[]..." << endl;
            delete[] t;
        }
    };


    template <typename T, typename Deleter=default_deleter<T>>
    class unique_ptr
    {
        public:
            unique_ptr(T* value):
                m_data(value)
            {}

            // many methods are missing
            // ...

            ~unique_ptr()
            {
                if (m_data)
                {
                    m_deleter(m_data);
                }
            }

        private:
            T* m_data;
            Deleter     m_deleter;
    };


    template<class T1, class T2>
    struct A
    {
        A()
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }
    };

    template<class T1>
    struct A<T1, string>
    {
        A()
        {
            cout << "(S)" << __PRETTY_FUNCTION__ << endl;
        }
    };

    template<class T1>
    struct A<T1, double>
    {
        A()
        {
            cout << "(D)" << __PRETTY_FUNCTION__ << endl;
        }
    };


    template<typename T, typename T2>
    void foo2(T value1, T2 value2)
    {
        cout << __PRETTY_FUNCTION__ << endl;
        cout << "value1: " << value1 << endl;
        cout << "value2: " << value2 << endl;

        cout << "general case for " << typeid(T ).name() << " and " \
                                    << typeid(T2).name() << endl;
    }

    template<typename T>
    void foo2(T value1, int value2)
    {
        cout << __PRETTY_FUNCTION__ << endl;

        cout << "value1: " << value1 << endl;
        cout << "value2: " << value2 << endl;
    }

//    template<typename T>
//    void foo2<T>()
//    {
//        cout << __PRETTY_FUNCTION__ << endl;

//        T   value1;
//        int value2;
//        cout << "value1: " << value1 << endl;
//        cout << "value2: " << value2 << endl;
//    }
}

namespace static_polymorphism {

    template <class T>
    struct Shape
    {
        void draw()
        {
            static_cast<T*>(this)->draw();
        }
    };

    struct Circle: public Shape<Circle>
    {
        void draw()
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }
    };

    struct Square: public Shape<Square>
    {
        void draw()
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }
    };

    ////////////////////
    struct Counter
    {
        static long _num_of_instances;

        Counter()
        {
            _num_of_instances++;
        }
    };
    long Counter::_num_of_instances = 0;

    template<typename T>
    struct CounterT
    {
        static long _num_of_instances;

        CounterT()
        {
            _num_of_instances++;
        }
    };
    //long CounterT::_num_of_instances = 0;


    class A_ : public Counter
    {
    };


    class A : public CounterT<A>
    {
    };
    template<>
    long CounterT<A>::_num_of_instances = 0;

    class B : public CounterT<B>
    {
    };
    template<>
    long CounterT<B>::_num_of_instances = 0;
}


namespace deeply_nested {

    template<typename T>
    void debug_print(const T& value)
    {
        cout << value;
    }

    template<typename T>
    void debug_print(const vector<T>& value)
    {
        cout << "{ ";

        for(const T& elem : value)
        {
            debug_print(elem);
            cout << ", ";
        }

        cout << "}";
    }

    template<>
    void debug_print<int>(const int& value)
    {
        cout << value;
    }

    template<>
    void debug_print<bool>(const bool& value)
    {
        cout << std::boolalpha << value;
    }

    template<>
    void debug_print<string>(const string& value)
    {
        cout << "\"" << value << "\"";// << endl;
    }

    template<>
    void debug_print<double>(const double& value)
    {
        cout << setprecision(15) << value;// << endl;
    }

}

namespace fun {

    template <unsigned n>
    struct factorial
    {
      enum { value = n * factorial<n-1>::value };
    };

    template <>
    struct factorial<0>
    {
      enum { value = 1 };
    };
}

int main(int argc, char *argv[])
{
//    int v = 0b01010101;
//    bool value = true;

//    v ^= (-value ^ v) & (1 << 3);

//    cout << bitset<8>(v) << endl;
//    return 0;

    {
        using function_template::max;

        cout << max(1ul, 2ul) << endl;
        cout << max(1, 2) << endl;
        cout << max(1.f, 2.f) << endl;
        cout << max<double>(1., 2.) << endl;

        // any class that supports comparision (or implemented comparator)
//        cout << max(string("abce"), string("abcd")) << endl;
        //cout << max(string("abce"), string("abcd")) << endl;
        cout << ::max(string("abce"), string("abcd")) << endl;
    }

    {
        using namespace class_template;

//        auto head =  new list();
        auto head =  new list<int>();
        head->push_front(1);
        head->push_front(2);
        head->push_front(3);

        while(!head->is_empty())
        {
            auto elem_data = head->pop_front();
            cout << "List elem: " << elem_data << endl;
        }
    }

    {
        cout << "======================================" << endl;
        using namespace class_template;

//        auto head =  new list();
        auto head =  new list<bool>();
        head->push_front(1);
        head->push_front(2);
        head->push_front(3);

        while(!head->is_empty())
        {
            auto elem_data = head->pop_front();
            cout << "List elem: " << elem_data << endl;
        }
    }

    {
        cout << "======================================" << endl;
        using namespace full_specialization;

//        foo(42, string("abc"));
//        foo2<int, int>();
        foo2<int, char>();
        foo(42);
        foo(42.123456789);
        foo(true);
        foo("abc");
        foo(string("abc"));

//        foo2<int, void*>();
    }

    {
        cout << "======================================" << endl;
        using partial_specialization::unique_ptr;
        using partial_specialization::default_deleter;
        using partial_specialization::A;

        {
            auto my_ptr  = unique_ptr<int>(new int(42));
            auto my_ptr2 = unique_ptr<int>(new int[42]);
        }

        {
            auto my_ptr3 = unique_ptr<int,
                           default_deleter<int[]>>(new int[42]);
        }

        {
            A<void*, double> a1;
            A<void*, string> a2;
            A<void*, int> a3;

        }
    }

    {
        cout << "======================================" << endl;
        using namespace static_polymorphism;
        {
            Shape< Square > *b1 = new Square();
            Shape< Circle > *b2 = new Circle();

            b1->draw();
            b2->draw();

            {
                Counter c1;
                Counter c2;
                Counter c3;
                Counter c4;
                cout << Counter::_num_of_instances << endl;

                A_ a1;
                A_ a2;
                cout << A_::_num_of_instances << endl;
            }

            {
                cout << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" << endl;
                A a1;
                A a2;
                cout << A::_num_of_instances << endl;

                B b1;
                B b2;
                B b3;
                cout << B::_num_of_instances << endl;
            }
        }
    }

    {
        cout << "======================================" << endl;
        using namespace deeply_nested;
        {
            {
                vector<int> v1 = {1, 2, 3, 4};
                vector<vector<int>> v2 = {{1}, {2}, {3}, {4}};
                vector<vector<bool>> v3 = {{true}, {false, 2!=3, 1, 0, 1}};
                vector<vector<vector<string>>> v4 = {{{"a", "b", "c"}}, {{"x"}}};

                debug_print(v1);
                cout << endl;
                debug_print(v2);
                cout << endl;
                debug_print(v3);
                cout << endl;
                debug_print(v4);
                cout << endl;
            }
        }
    }

    {
        cout << "======================================" << endl;
        stack<int> s;
        s.push(42);
        cout << s.pop() << endl;

        stack<bool> s2;
        s2.push(1);
        cout << s2.pop() << endl;
    }

    {
        stack<char> ethalon_stack;
        stack<bool> special_stack;

        for (int i=0; i<16*8; ++i)
        {
            bool random_bool = true;//...;
            special_stack.push(random_bool);
            ethalon_stack.push(random_bool);
        }

        for (int i=0; i<16*8; ++i)
        {
            bool bool_value1 = special_stack.pop();
            bool bool_value2 = ethalon_stack.pop();
            //assert(bool_value1 == bool_value2);
        }
    }


    {
        using namespace fun;

        cout << "======================================" << endl;
        cout << factorial<7>::value << endl;
    }

    return 0;
}
