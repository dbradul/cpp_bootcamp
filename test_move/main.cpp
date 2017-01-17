#include <string>
#include <iostream>
#include <iomanip>
#include <utility>
#include <fstream>
#include <map>
#include <unordered_map>
#include <vector>

using namespace std;


//struct A
//{
//    std::string s;
//    A() : s("test")
//    {
//            cout << __PRETTY_FUNCTION__ << endl;
//    }
//    A(const A& o) : s(o.s)
//    {
//            cout << __PRETTY_FUNCTION__ << endl;
//    }

//    explicit operator const char* () const
//    {
//        return s.c_str();
//    }

//    A(A&& o) :
//        s(std::move(o.s))
//    {
//            cout << __PRETTY_FUNCTION__ << endl;
//    }
//};

//A f(A a)
//{
//    return a;
//}



//struct B : A
//{
//    std::string s2;
//    int n;
//    // implicit move constructor B::(B&&)
//    // calls A's move constructor
//    // calls s2's move constructor
//    // and makes a bitwise copy of n
//};

//struct C : B
//{
//    ~C() { } // destructor prevents implicit move ctor C::(C&&)
//};

//struct D : B
//{
//    D() { }
//    ~D() { }          // destructor would prevent implicit move ctor D::(D&&)
//    D(D&&) = default; // force a move ctor anyway
//};

//typedef multimap<int, string, greater<int>> saveMap;
//typedef unordered_map<string, int> CounterMap;

//CounterMap frequency(vector<string> numbers) {
//    CounterMap counts;
//    for (int j = 0; j < numbers.size(); ++j) {
//        CounterMap::iterator i(counts.find(numbers[j]));
//        if (i != counts.end()) {
//            i->second++;
//        } else {
//            counts[numbers[j]] = 1;
//        }
//    }
//    return counts;
//}






//int main()
//{

//    std::cout << "Trying to move A\n";
//    A a0;

//    cout << a0.s << endl;
//    foo(move(a0));
//    cout << a0.s << endl;

//    foo(static_cast<const char*>(A()));

//    A a0;
//    A a1(f(a0)); // move-construct from rvalue temporary
//    A a2(a0); // move-construct from rvalue temporary

////    int f = 5;
//    string s = string("abc") + string("abc");
////    int ff = f();

//    ifstream file;
//    file.open ("/home/db/Work/test.txt");
//    std::string word;
//    vector <string> vs;
//    while (file >> word)
//    {
//        vs.push_back(word);
//        cout << word << endl;
//    }


//    CounterMap v = frequency(vs);
//    saveMap b;
//    for (CounterMap::iterator it = v.begin(); it != v.end(); ++it) {

//        b.insert(make_pair(it->second, it->first));
//    }
//    for (saveMap::iterator it = b.begin(); it != b.end(); ++it) {
//        cout << it->first << "  " << it->second << endl;
//    }

//    file.close();

//    return 0;


//    A a2 = std::move(a1);
//    cout << a1.s << endl;

//    std::cout << "Trying to move B\n";
//    B b1;
//    std::cout << "Before move, b1.s = " << b1.s << "\n";
//    B b2 = std::move(b1); // calls implicit move ctor
//    std::cout << "After move, b1.s = " << b1.s << "\n";

//    std::cout << "Trying to move C\n";
//    C c1;
//    C c2 = std::move(c1); // calls the copy constructor

//    std::cout << "Trying to move D\n";
//    D d1;
//    D d2 = std::move(d1);
//}


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

//    noncopyable(noncopyable&& rhs)
//        : m_value(move(rhs.m_value))
//    {
//        cout << __PRETTY_FUNCTION__ << endl;
//    }


//private:
//  noncopyable(const noncopyable&);
//  noncopyable& operator=(const noncopyable&);
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

namespace std {

    using expl_members::preA;

    template<>
    struct hash<preA>
    {
        size_t operator()(const preA& key) const
        {
            size_t result  = hash<string>()(key.str_value);
            size_t result2 = hash<int>()(key.int_value);

            return result ^ (result2 << 1);
        }
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

    struct HashT_A
    {
        size_t operator()(const A& key) const
        {
            size_t result  = hash<string>()(key.str_value);
            size_t result2 = hash<int>()(key.int_value);
            size_t result3 = hash<preA>()(key.a_value);

            return result ^ (result2 << result3);
        }
    };


    bool operator==(const A& obj1, const A& obj2);

    struct EqualT_A
    {
        bool operator()(const A& key1, const A& key2) const
        {
            cout << __PRETTY_FUNCTION__ << endl;
            return key1==key2;
        }
    };

    bool operator==(const preA& obj1, const preA& obj2)
    {
        return obj1.int_value == obj2.int_value
            && obj1.str_value == obj2.str_value;
    }

    bool operator==(const A& obj1, const A& obj2)
    {
        return obj1.int_value == obj2.int_value
            && obj1.str_value == obj2.str_value
            && obj1.a_value   == obj2.a_value;
    }
}


int main(int argc, char* argv[])
{
    noncopyable a;
    a.m_value = 42;
    noncopyable b = move(a);
    cout << a.m_value << endl;
    cout << b.m_value << endl;

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
        foo(std::move(a));
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
        cout << endl;

        string a = "abc";
        string b = move(a);
        cout << a << ", " << a.length() << ", " << a.c_str() << endl;
        cout << b << ", " << b.length() << ", " << b.c_str() << endl;
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

    // TBD:
    // 1. mutual exclusion of copy and move
    // 2. inheritance copy/move
    //

    {
        using namespace expl_members;
        cout << "---m-o-v-e---" << endl;
        cout << endl;

        vector<A> v1;
        A a(42, "yyy", {11, "xxx"});

//        v1.push_back(a);
//        v1.push_back(move(a));
//        v1.push_back(A(42, "yyy", {11, "xxx"}));
//        v1.emplace_back(42, "yyy", preA()/*{11, "xxx"}*/);
        vector<A> v2 = move(v1);
//        v2 = move(v1);

        unordered_map<string, A> m;
        m["a"]=move(a);
        m.emplace("a", move(a));
//        //m.insert(a);

        cout << a << endl;
        cout << v1 << endl;
        cout << v2 << endl;
    }


    {
        cout << endl;

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

    //    b = move(a);
//    b = a;
}
