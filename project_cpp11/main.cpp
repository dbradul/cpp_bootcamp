#include <iostream>
#include <set>
#include <tuple>
#include <algorithm>
#include <iomanip>
#include <iosfwd>

using namespace std;

// Topics:
// - lambdas
// - variadic templates
// - tuples templates
// - default/delete members
// - override/final templates
// - initialization members
// - perfect forwarding
// - delegating ctors
// - threads
// - enums

// Links:
// http://stackoverflow.com/documentation/c%2b%2b/topics
// (perfect forwarding) https://habrahabr.ru/post/242639/


// H/W: LRU cache


namespace lambda {

    void foo(int val)
    {
        cout << __PRETTY_FUNCTION__ << "val: " << val << endl;
    }

    void foo(float val)
    {
        cout << __PRETTY_FUNCTION__ << "val: " << val << endl;
    }

    void foo(double val)
    {
        cout << __PRETTY_FUNCTION__ << "val: " << val << endl;
    }
}

namespace tuple_ns {

    struct Box
    {
        Box()
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }

        Box(int par1, string param2, float param3)
            : n(par1)
            , s(param2)
            , d(param3)
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }

        Box(const Box& rhs)
            : n(rhs.n)
            , s(rhs.s)
            , d(rhs.d)
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }

        Box(Box&& rhs)
            : n(move(rhs.n))
            , s(move(rhs.s))
            , d(move(rhs.d))
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }

        int n;
        std::string s;
        float d;
        bool operator<(const Box& rhs) const
        {
            return std::tie(n, s, d) < std::tie(rhs.n, rhs.s, rhs.d);
        }
    };

    ostream& operator<< (ostream& os, const Box& box)
    {
        os << "{";
        os << "n: " << box.n << ", "
           << "s: " << box.s << ", "
           << "d: " << box.d;
        os << "}";
    }

    template<std::size_t I = 0, typename... Tp>
    inline typename std::enable_if<I == sizeof...(Tp), void>::type
    print_tuple(const std::tuple<Tp...>&, std::ostream& os = std::cout)
    { }


    template<std::size_t I = 0, typename... Tp>
    inline typename std::enable_if<I < sizeof...(Tp), void>::type
    print_tuple(const std::tuple<Tp...>& t, std::ostream& os = std::cout)
    {
        os << ((I>0)? ", " : "");
        os << std::get<I>(t);
        print_tuple<I + 1, Tp...>(t, os);
    }


    template<typename... Tp>
    void print(const std::tuple<Tp...>& t, std::ostream& os = std::cout
                                         , string trailing_token = "\n")
    {
        os << "{";
        print_tuple(t, os);
        os << "}";

        os << trailing_token;
    }

    template<typename... Tp>
    ostream& operator<<(ostream& os, const std::tuple<Tp...>& t)
    {
        print(t, os, "");
        return os;
    }

    tuple<int, string> foo()
    {
        return make_tuple(42, "str42");
    }
}


namespace initlzr_list {

    template<typename T, size_t N>
    class array
    {
    public:
        array (T* arr, size_t n)
        {
            cout << __PRETTY_FUNCTION__ << endl;
            std::copy(arr, arr+std::min(n,N), m_data);
        }

        array (initializer_list<T> lst)
        {
            cout << __PRETTY_FUNCTION__ << endl;
            std::copy(lst.begin(), lst.begin()+std::min(lst.size(),N), m_data);
        }

        T operator[](size_t idx)
        {
            return m_data[idx];
        }

        T* begin()
        {
            return &m_data[0];
        }

        T* end()
        {
            return m_data+N;
        }

    private:
        T m_data[N];
    };

    template<typename T, size_t N>
    ostream& operator<< (ostream& os, /*const*/ array<T, N>& rhs)
    {
        os << "{";
        for (const T& elem : rhs)
        {
            //os << (&elem!=&(*rhs.begin()) ? ", " : "");
            os << elem;
        }
        os << "}";
        return os;
    }

}

namespace variadic_tmpl {


template<typename T>
void debug_print(const T& value)
{
    cout << value;
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


void print_safe()
{
    cout << endl;
}


template<typename T, typename ... Rest>
void print_safe(const T& first, const Rest& ... rest)
{
    cout << endl;
    debug_print(first);
    print_safe(rest...);
}


}


namespace pf {

void func(int a, double b)
{
    cout << __PRETTY_FUNCTION__ << endl;
    cout << "a: " << a << ", b: " << b << endl;
}

//void func(int& a, double& b)
//{
//    cout << __PRETTY_FUNCTION__ << endl;
//    cout << "a: " << a << ", b: " << b << endl;
//}

//template <typename T1, typename T2>
//void wrapper(T1 e1, T2 e2)
//{
//    func(e1, e2);
//}


template <typename T1, typename T2>
void wrapper(const T1& e1, T2& e2)
{
    func(e1, e2);
}


//template <typename T1, typename T2>
//void wrapper(T1&& e1, T2&& e2) {
//    func(forward<T1>(e1), forward<T2>(e2));
//}

}


int main(int argc, char *argv[])
{
    // functions+lambdas
    {
        using namespace lambda;

        auto x = [](){};
        auto y = []{};
        x();
        y();

        // capturing
        int a = 0;
        //auto f = []()   { return a+1; };
        auto f1 = [a]()            { return a+1; };
        auto f2 = [](const int a)  { return a+1; };
        auto f3 = [&a]()           { return a++; };

        cout << f1() << endl;
        cout << f2(a) << endl;
        cout << a << endl;
        f3();
        cout << a << endl;

        auto f4 = [&]() { return a++; };
        auto f5   = [=]() mutable { return a++; };
        auto f5_2 = [](int a) { return a++; };
        f4();
        cout << a << endl;
        f5();
        cout << a << endl;

        auto f6 = [&a]() { return a++; };
        auto f7 = [=,&a]() /*mutable*/ { return a++; };
        auto f7_2 = [&,a]() mutable { return a++; };
        f6();
        cout << a << endl;
        f7();
        cout << a << endl;
        f7_2();
        cout << a << endl;

        int b = 42;
        auto f8 = [b]() mutable { return b++; };
        auto f9 = [b=0]() { return b; };
        cout << f8() << endl;
        cout << b << endl;
        cout << f9() << endl;
        cout << b << endl;

        int value = 5;
        auto timesN = [value](int N) { return N * value; };
        cout << timesN(2) << endl;
        value = 15;
        cout << timesN(2) << endl;


        auto timesN_ref = [&value,&a,b](int N) { /*++*/b, ++a;return N * value; };
        cout << timesN_ref(2) << endl;
        value = 25;
        cout << timesN_ref(2) << endl;

        auto fn = [](int val1, string val2)
        {
            cout << "val1: " << val1 << ", val2: " << val2 << endl;
            return val2 == to_string(val1);
        };
        cout << "fn(42, \'42\'): " << fn(42, "42") << endl;


        auto fn2 = [](){ return 42; };
        foo(fn2());

        auto fn3 = []() -> float { return 42; };
        foo(fn3());

        auto fn4 = [](int a) -> float
        {
            if (a>10)
            {
                return 42;
            }
            else
            {
                return '4';
            }
        };
        foo(fn4(5));

//        auto fn5 = [](auto a, auto b) -> auto { return a+b; };
//        foo(fn5(1,2));
    }

    // initializer list
    {
        #define ARR_SIZE(a) (sizeof(a))/(sizeof(a[0]))

        int raw_array[] = {1,2,3,4,5};
        initlzr_list::array<int, 3> arr2(raw_array, ARR_SIZE(raw_array));
        cout << arr2 << endl;

        for (auto& elem : arr2)
        {
            cout << elem << endl;
        }

        initlzr_list::array<int, 5> arr3 = {39,40,41,42,43,44,45};
        cout << arr3 << endl;
    }

    // variadic templates
    {
        using namespace variadic_tmpl;

        print_safe(1, 42.000000000001, true, string("qwerty"));
    }

    // tuple
    {
        using namespace tuple_ns;

        Box box = {42, "Test", 3.14};

        auto t  = tuple<int, string, float, Box, Box*> (42, "43", 44., box, nullptr);

        int val1    = get<0>(t);
        string val2 = get<1>(t);
        float val3  = get<2>(t);
        Box val4    = get<3>(t);
        Box* val5   = get<4>(t);

        cout << " val1: " << val1
             << " val2: " << val2
             << " val3: " << val3
             << " val4: " << val4
             << " val5: " << val5
             << endl;

        auto t2 = make_tuple(41, "42", 43., box, &box);
        cout << " val1: " << get<0>(t2)
             << " val2: " << get<1>(t2)
             << " val3: " << get<2>(t2)
             << " val4: " << get<3>(t2)
             << " val5: " << get<4>(t2)
             << endl;

        print(t2);

        // tie
        // unpack from tuple
        auto t3 = foo();
        int t3_v1;
        string t3_v2;
        tie(t3_v1, t3_v2) = t3;
        cout << "t3_v1: " << t3_v1 << endl;
        cout << "t3_v2: " << t3_v2 << endl;

        auto p = pair<double, string>(2.71, "e");
        double p_v1;
        string p_v2;
        tie(p_v1, p_v2) = p;
        cout << "p_v1: " << p_v1 << endl;
        cout << "p_v2: " << p_v2 << endl;

        // compound comparision
        bool less_than = tie(p_v1, p_v2)
                       < tie(p_v1, p_v2);
        cout << "less_then: " << less_than << endl;

        // pack to tuple
        tuple<double, string> t5 = tie(p_v1, p_v2);
        cout << t5 << endl;

        double p2_v1, p2_v3;
        string p2_v2, p2_v4;

        cout << "BEFORE (to): "     << tie(p2_v1, p2_v2, p2_v3, p2_v4) << endl;
        cout << "BEFORE (from): "   << tie(p_v1,  p_v2,  t3_v1, t3_v2) << endl;
        tie(p2_v1, p2_v2, p2_v3, p2_v4) = tie(p_v1, p_v2, t3_v1, t3_v2);
        cout << "AFTER (to): " << tie(p2_v1, p2_v2, p2_v3, p2_v4) << endl;
    }

    {
        using namespace  pf;
        using namespace tuple_ns;

        vector<Box> v;
        //v.push_back(Box(1,"abc",42.));
        Box b = {1, "asd", 42.};
        v.emplace_back(b);
        v.emplace_back(1, "asd", 42.);

        int x = 42;
        double y = 21.;
        wrapper(42, y);
    }

    return 0;
}
