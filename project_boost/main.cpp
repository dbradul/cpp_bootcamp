#include <iostream>
#include <typeinfo>

#include "boost/lexical_cast.hpp"

#include "boost/any.hpp"
#include "boost/variant.hpp"
#include "boost/optional.hpp"

#include "boost/range/algorithm.hpp"
#include "boost/range/numeric.hpp"
#include "boost/range/irange.hpp"
#include "boost/range/combine.hpp"

using namespace std;

// Links:
// -- boost.org
// -- http://www.ebooksbucket.com/uploads/itprogramming/cplus/Boost_Cplusplus_Application_Development_Cookbook.pdf
// -- http://greek0.net/boost-range
// -- http://knzsoft.blogspot.com/2013/07/boost-windows-mingw.html
// -- http://stackoverflow.com/questions/38060436/what-are-the-new-features-in-c17


struct Box
{
    int i;
};

ostream& operator<<(ostream& os, const Box& b)
{
    return os << "{" << b.i << "}";
}

boost::optional<Box> getValue(bool state = false)
{
    if (state)
    {
        Box b = {42};
        return boost::optional<Box>(b);
    }
    else
    {
        return boost::optional<Box>();
    }
}


template<typename T>
ostream& operator<< (ostream& os, const vector<T>& rhs)
{
    for (const T& elem : rhs)
    {
        os << (&elem!=&rhs[0] ? ", " : "{ ");
        os << elem;
    }
    return os << " }";
}

int main(int argc, char *argv[])
{
    // LEXICAL_CAST
    {
        // to string
        string s = boost::lexical_cast<string>(42);
        s = boost::lexical_cast<string>(42);
        cout << s << endl;
        s = boost::lexical_cast<string>(42.43);
        cout << s << endl;
        s = boost::lexical_cast<string>(0x4242);
        cout << s << endl;
        s = boost::lexical_cast<string>('x');
        cout << s << endl;

        array<char, 20> buf = boost::lexical_cast<array<char, 20>>(42+43);
        cout << buf.begin() << endl;

        // from string
        int x = boost::lexical_cast<int>("42");
        x = boost::lexical_cast<int>('5');
        //x = boost::lexical_cast<int>("5.0");
        cout << x << endl;
    }

    // VARIANT
    {
        union {int i; float f;} u;
        u.f = 42.0;
        cout << u.i << endl;

        boost::variant<int, string, float, Box> var1, var2;

        var1 = 42;
        var1 = "abc";
        var1 = 42.0f;
        var1 = {42};
        var1 = 43;

        cout << "var1 as int: " << var1 << endl;
        cout << "var1 as int: " << boost::get<int>(var1) << endl;
        try
        {
            cout << "var1 as float: " << boost::get<float>(var1) << endl;
            assert(false);
        }
        catch(boost::bad_get& ex)
        {
            cout << ex.what() << endl;
        }

        cout << "BEFORE: " << var2 << endl;
        boost::get<int>(var2)+=10;
        cout <<  "AFTER: " << var2 << endl;

        vector<boost::variant<int, string>> vec;
        vec.push_back(23);
        vec.push_back("abc");
        for (auto elem: vec)
        {
            cout << elem << endl;
        }
    }

    // ANY
    {
        boost::any x, y, z;
        x = string("abc");
        x = 42;
        x = 4.2;
        y = 'z';
        z = vector<int>();

        cout << boost::any_cast<double>(x) << endl;
        cout << boost::any_cast<char>(y) << endl;
        cout << boost::any_cast<vector<int>>(z).size() << endl;

        x = 42;
        try
        {
            cout << boost::any_cast<double>(x) << endl;
            assert(false);
        }
        catch(boost::bad_any_cast& ex)
        {
            cout << ex.what() << endl;
        }

        int i;
        boost::any ptr = &i;
        int* pInt = boost::any_cast<int*>(ptr);
        *pInt = 42;
        assert(i==42);

        std::vector<boost::any> some_values;
        some_values.push_back(10);
        const char* c_str = "Hello there!";
        some_values.push_back(c_str);
        some_values.push_back(std::string("Wow!"));
        std::string& s = boost::any_cast<std::string&>(some_values.back());
        s += " That is great!\n";
        std::cout << s;
    }

    // OPTIONAL
    {
        //boost::optional<Box> value = getValue(true);
        boost::optional<Box> value = getValue(true);

        if (!value)
        {
            cout << "Box is not initialized!" << endl;
            cout << "Falling back to default: " << value.get_value_or({43}) << endl;
        }
        else
        {
            cout << value.get() << endl;
            cout << value->i << endl;
            value->i = 44;
            cout << value.get() << endl;
            cout << value->i << endl;
        }

        boost::optional<int> i1(5);
        boost::optional<int> i2(10);
        boost::optional<int> i3;
        boost::optional<int> i4(std::numeric_limits<int>::min());

        cout << (i1<i2) << endl;
        cout << (i2<i1) << endl;
        cout << (i3<i2) << endl;
        cout << (i3<i4) << endl;
    }

    // RANGE
    {
        std::vector<int> vec = {3, 9, 0, 6, 4, 8, 2, 5, 7, 1};
        boost::range::sort(vec, std::greater<int>());
        //sort(vec.begin(), vec.end(), std::greater<int>());
        cout << vec << endl;

        cout << boost::range::count_if(vec, [](int e) {return (e>5);}) << endl;
        cout << std::count_if(vec.begin(), vec.end(), [](int e) {return (e>5);}) << endl;

        cout << "sum: " << boost::accumulate(vec, 0) << endl;
        cout << "product: " << boost::accumulate(vec, 1, std::multiplies<int>()) << endl;

        for (int i : boost::irange(5, 10)) { cout << i << " "; }
        cout << endl;

        for (int i : boost::irange(1, 10, 2)) { cout << i << " "; }
        cout << endl;

        for (int i : boost::irange(1, 10, 200)) { cout << i << " "; }
        cout << endl;

        std::string str = "abcde";
        std::vector<int> vect = {1, 2, 3, 4, 5};
        for (const auto & zipped : boost::combine(str, vect)) {
            char c; int i;
            boost::tie(c, i) = zipped;

            //std::cout << typeid(zipped).name() << endl;

            cout << "c: " << c << ", i: " << i << endl;
        }
    }

    // TOKENIZER
    // ASIO
    // COROUTINES

    return 0;
}
