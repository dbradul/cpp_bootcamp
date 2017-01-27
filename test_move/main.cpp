#include <string>
#include <iostream>
#include <iomanip>
#include <utility>
#include <fstream>
#include <map>
#include <unordered_map>
#include <vector>

using namespace std;

// Links:
// - https://msdn.microsoft.com/en-us/library/dn457344.aspx
// - http://bit.ly/1RpuoAY

#include <memory>
#include <iostream>
#include <ctime>

using namespace std;

struct noncopyable
{
    int m_value;

    noncopyable()
        : m_value (0){}

    noncopyable(int value)
        : m_value (value){}

    noncopyable(noncopyable&& rhs)
        : m_value(move(rhs.m_value))
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }

    noncopyable(const noncopyable&rhs)
        : m_value(rhs.m_value)
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }

    noncopyable& operator=(const noncopyable& rhs)
    {
        cout << __PRETTY_FUNCTION__ << endl;
        m_value = rhs.m_value;
    }

//private:
};

//void foo(int value)
//{
//    cout << __PRETTY_FUNCTION__ << endl;
//}

//void foo(int& value)
//{
//    cout << __PRETTY_FUNCTION__ << endl;
//}

namespace impl_members {

    void foo(int&& value)
    {
        ++value;
        cout << __PRETTY_FUNCTION__ << endl;
    }

    struct A
    {
        int value;
        int* ptr;
        char* str;
    };
}

namespace expl_members{

    struct preA
    {
        int    int_value;
        string str_value;
    };
}


namespace expl_members {

    struct A
    {
        int    int_value;
        string str_value;
        preA   a_value;

        A()
            : int_value (0)
            , str_value ()
            , a_value   ()
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }

        A(int int_val, string str_val, preA a_val)
            : int_value (int_val)
            , str_value (str_val)
            , a_value   (a_val)
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }

        A(const A& copy)
            : int_value (copy.int_value)
            , str_value (copy.str_value)
            , a_value   (copy.a_value)
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }

        A& operator=(const A& copy)
        {
            cout << __PRETTY_FUNCTION__ << endl;
            A(copy).swap(*this);
            return *this;
        }

        void swap(A& copy)
        {
            ::swap(int_value, copy.int_value);
            ::swap(str_value, copy.str_value);
            ::swap(a_value,   copy.a_value);
        }

        A(A&& copy)
            : int_value (move(copy.int_value))
            , str_value (move(copy.str_value))
            , a_value   (move(copy.a_value))
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }

        A& operator=(A&& copy)
        {
            cout << __PRETTY_FUNCTION__ << endl;
            A(move(copy)).swap(*this);
            return *this;
        }


//        A(const A& copy) =default;
//        A& operator=(const A& copy) =default;
//        A(A&&) =default;
//        A& operator=(A&&) =default;

        ~A()
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }
    };

    ostream& operator<< (ostream& os, const preA& a)
    {
        os << "{";
        os << a.int_value << ", "
           << a.str_value;
        os << "}";

        return os;
    }

    ostream& operator<< (ostream& os, const A& a)
    {
        os << "{";
        os << a.int_value << ", "
           << a.str_value << ", "
           << a.a_value;
        os << "}";

        return os;
    }

    template<typename T>
    ostream& operator<< (ostream& os, const vector<T>& rhs)
    {
        os << rhs.size() << ", " << rhs.capacity() << ": { ";
        for (const T& elem : rhs)
        {
            os << (&elem!=&rhs[0] ? ", " : "");
            os << elem;
        }
        os << " }";
        return os;
    }

}


int main(int argc, char* argv[])
{
    {
        string y = "abc";
        string x = y;
        cout << y << ", " << y.length() << ", " << y.c_str() << endl;
        cout << x << ", " << x.length() << ", " << x.c_str() << endl;

        string b = move(y);
        cout << y << ", " << y.length() << ", " << y.c_str() << endl;
        cout << b << ", " << b.length() << ", " << b.c_str() << endl;


        cout << endl;

    }

    {
        noncopyable a;
        a.m_value = 42;
        noncopyable b = move(a);
        cout << a.m_value << endl;
        cout << b.m_value << endl;
    }




    {
        cout << endl;

        int a = 42;
        int b = move(a);
        cout << a << endl;
        cout << b << endl;
    }



    {
        cout << endl;

        float a = 42;
        float b = move(a);
        cout << a << endl;
        cout << b << endl;
    }


    {
        cout << endl;

        bool a = true;
        bool b = move(a);
        cout << a << endl;
        cout << b << endl;
    }


    {
        cout << endl;

        char a = 'a';
        char b = move(a);
        cout << a << endl;
        cout << b << endl;
    }

    {
        cout << endl;

        const char* a = "abc";
        cout << a << endl;
        const char* b = move(a);
        cout << a << endl;
        cout << b << endl;
    }


    {
        using namespace impl_members;
        cout << endl;

        int a = 42;
        cout << a << endl;
        // fallback to less-efficient
        foo(42);
//        foo2(1+1);
        cout << a << endl;
    }


    {
        using namespace impl_members;
        cout << endl;

        int data = 42;
        char* str = "abc";
        A a = {42, &data, str};
        cout << a.value << ", " << a.ptr << ", " << a.str << endl;
        A b = move(a);
        cout << a.value << ", " << a.ptr << ", " << a.str << endl;
        cout << b.value << ", " << b.ptr << ", " << b.str << endl;
    }



    {
        using namespace expl_members;
        cout << endl;

        A a(42, "yyy", {11, "xxx"}/*preA()*/);
        A b = move(a);
//        b = a;

        cout << a << endl;
        cout << b << endl;
    }



    {
        using namespace expl_members;
        cout << "---m-o-v-e---" << endl;
        cout << endl;

        vector<A> v1;
        A a(42, "yyy", {11, "xxx"});


        v1.push_back(a);
        v1.push_back(move(a));
//        v1.push_back(A(42, "yyy", {11, "xxx"}));
        v1.emplace_back(42, "yyy", preA()/*{11, "xxx"}*/);
        vector<A> v2 = move(v1);
//        v2 = move(v1);

        cout << v1 << endl;
        cout << v2 << endl;

        vector<string> vv1;
        string ss1="abc";
        vv1.push_back(ss1);
        cout << ss1 << endl;
        vv1.push_back(move(ss1));
        cout << ss1 << endl;

        cout << vv1 << endl;

//        return 0;

        unordered_map<string, A> m;
        m["a"]=move(a);
        m.emplace("a", move(a));
//        //m.insert(a);

        cout << a << endl;
        cout << v1 << endl;
        cout << v2 << endl;
    }

//    return 0;

    {
        cout << endl;

        string s("abc");
        cout << "s.size(): " << s.size() << ", s=" << s << endl;
        string s1 = s;
        cout << "s.size(): " << s.size() << ", s=" << s << endl;
        cout << "s1.size(): " << s1.size() << ", s1=" << s1 << endl;

        string s2 = "xyz";
        s2 = static_cast<string&&>(s);
        cout << "s.size(): " << s.size() << ", s=" << s << endl;
        cout << "s2.size(): " << s2.size() << ", s2=" << s2 << endl;

        string s4 = "QWERTY";
        s4 = move(s2);
        cout << "s2.size(): " << s2.size() << ", s2=" << s2 << endl;
        cout << "s4.size(): " << s4.size() << ", s4=" << s4 << endl;
    }

}
