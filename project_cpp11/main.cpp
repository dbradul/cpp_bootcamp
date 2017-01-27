#include <iostream>
#include <set>
#include <tuple>
#include <algorithm>

using namespace std;

// Topics:
// - lambdas
// - variadic templates
// - tuples templates

// - default/delete templates
// - override/final templates
// - initializer lists
// - threads
// - using

// Links:
// http://stackoverflow.com/documentation/c%2b%2b/topics

namespace tuple_ns {

    struct Box
    {
        int n;
        std::string s;
        float d;
        bool operator<(const Box& rhs) const
        {
            // compares n to rhs.n,
            // then s to rhs.s,
            // then d to rhs.d
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



int main(int argc, char *argv[])
{
    // functions+lambdas
    {
        auto fn = [](int val1, string val2)
        {
            cout << "val1: " << val1 << ", val2: " << val2 << endl;
        };

        fn(42, "42");
    }

    // initializer list
    {
        for (auto& elem : {1,2,3})
        {
            cout << elem << endl;
        }
    }

    // tuple
    {
        using namespace tuple_ns;

        Box box = {42, "Test", 3.14};

        auto t  = tuple<int, string, float, Box, Box*>(42, "43", 44., box, nullptr);

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
        // TODO: doesn't work
//        bool less_than = tie(42, p_v2)
//                       < tie(42, p_v2);

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

    return 0;
}
