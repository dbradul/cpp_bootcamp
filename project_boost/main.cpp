#include <iostream>
#include <typeinfo>
#include <algorithm>
#include <chrono>
#include <thread>

#include "boost/lexical_cast.hpp"

#include "boost/any.hpp"
#include "boost/variant.hpp"
#include "boost/optional.hpp"

#include "boost/range/algorithm.hpp"
#include "boost/range/numeric.hpp"
#include "boost/range/irange.hpp"
#include "boost/range/combine.hpp"

#include "boost/bimap.hpp"
#include "boost/chrono.hpp"
#include "boost/bimap/multiset_of.hpp"

#include "boost/coroutine/all.hpp"
#include "boost/thread/thread.hpp"

#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/spirit/include/qi.hpp>

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <boost/asio/io_service.hpp>
#include <boost/asio/steady_timer.hpp>

using namespace std;

// Links:
// -- boost.org
// -- http://www.ebooksbucket.com/uploads/itprogramming/cplus/Boost_Cplusplus_Application_Development_Cookbook.pdf
// -- https://theboostcpplibraries.com/
// -- http://greek0.net/boost-range
// -- http://knzsoft.blogspot.com/2013/07/boost-windows-mingw.html
// -- http://stackoverflow.com/questions/38060436/what-are-the-new-features-in-c17
// -- https://habrahabr.ru/post/86899/


// sudo apt-get install libboost-all-dev


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

namespace my_bimap
{
    template< class MapType >
    void print_map(const MapType & map)
    {
        for( auto i = map.begin(); i != map.end(); ++i )
        {
            cout << i->first << " -> " << i->second << std::endl;
        }
    }
}

namespace my_bind
{
    int sum (int a, int b)
    {
        return (a + b);
    }

    struct A
    {
        void Print() const
        {
            std::cout << __PRETTY_FUNCTION__ << std::endl;
        }

        void Print(int value) const
        {
            std::cout << __PRETTY_FUNCTION__ << ": " << value << std::endl;
        }
    };
}


namespace my_coroutines
{
    using namespace boost::coroutines;

    class generator {
        public:
            generator(int lower, int upper)
                : lower_bound(lower)
                , upper_bound(upper) {}
            int operator()()
            {
                return lower_bound++;
            }

            operator bool() const
            {
                return lower_bound < upper_bound;
            }

        private:
            int lower_bound, upper_bound;
    };

    void simple_coroutine(coroutine<void>::push_type& sink)
    {
        cout << "BEFORE" << endl;
        sink();
        cout << "AFTER" << endl;
    }

    void generator_2(coroutine<int>::push_type& sink, int lower, int upper)
    {
        auto local_lower = lower;
        while (local_lower < upper)
        {
            sink(local_lower++);
        }
    }

    void fib(coroutine<long long>::push_type& sink)
    {
        long long prev = 1;
        long long prev_prev = 0;
        while (true)
        {
            long long new_value = prev + prev_prev;
            sink(new_value);
            tie(prev, prev_prev) = make_tuple(new_value, prev);
        }
    }

//    int range_generator(coro_type::self& self, int min, int max)
//    {
//        while (min < max - 1)
//        {
//            self.yield(min++);
//        }
//        self.exit(); // This is used instead
//    }
}

namespace asio
{
    void handler(const boost::system::error_code &ec)
    {
        cout << __PRETTY_FUNCTION__ << endl;
        cout << "handler thread: " << boost::this_thread::get_id() << endl;
    }
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
        s = boost::lexical_cast<string>(Box{42});
        cout << s << endl;

        array<char, 20> buf = boost::lexical_cast<array<char, 20>>(42+43);
        cout << buf.begin() << endl;

        // from string
        int x = boost::lexical_cast<int>("42");
        x = boost::lexical_cast<int>('5');
        x = boost::lexical_cast<double>("5.0");
        cout << x << endl;
    }

    // VARIANT
    {
        union {
            int i;
            float f;
        } u;
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

    // BIMAP
    {
        using namespace boost::bimaps;
        using namespace my_bimap;

        boost::bimap<string, string> en_de;
        en_de.insert({"Hi",         "Hallo"});
        en_de.insert({"Hello",      "Hallo"});
        en_de.insert({"Hi there",   "Hallo"});
        en_de.insert({"German",     "Deutch"});
        en_de.insert({"Yes",        "Ya"});

        cout << "en_de.size(): " << en_de.size() << endl;

        cout << "en_de.left: " << endl;
        print_map(en_de.left);

        cout << "en_de.right: " << endl;
        print_map(en_de.right);

        auto it1 = en_de.left.find("Hi");
        auto it2 = en_de.right.find("Hallo");
        std::cout << it1->first << " <=> " << it1->second << endl;
        std::cout << it2->first << " <=> " << it2->second << endl;


        // mutliset
        boost::bimap<string, multiset_of<string>> en_de_2;
        en_de_2.insert({"Hi",     "Hallo"});
        en_de_2.insert({"Hello",  "Hallo"});
        en_de_2.insert({"German", "Deutch"});
        en_de_2.insert({"Yes",    "Ya"});

        cout << "en_de_2.left: " << endl;
        print_map(en_de_2.left);

        cout << "en_de_2.right: " << endl;
        print_map(en_de_2.right);

        // counter
        boost::bimap<string, multiset_of<size_t, greater<int>>> counter;
        counter.insert({"Hi",     2});
        counter.insert({"Hello",  3});
        counter.insert({"German", 1});
        counter.insert({"Yes",    2});

        cout << "counter.left: " << endl;
        print_map(counter.left);

        cout << "counter.right: " << endl;
        print_map(counter.right);

    }


    // ASIO
    {
        using namespace boost::asio;
        using namespace asio;

//        {
//            io_service ioservice;

//            steady_timer timer{ioservice, std::chrono::seconds{3}};
//            timer.async_wait(handler);
//            steady_timer timer2{ioservice, std::chrono::seconds{3}};
//            timer2.async_wait(handler);
//            steady_timer timer3{ioservice, std::chrono::seconds{3}};
//            timer3.async_wait(handler);
//            steady_timer timer4{ioservice, std::chrono::seconds{3}};
//            timer4.async_wait(handler);
//            steady_timer timer5{ioservice, std::chrono::seconds{4}};
//            timer5.async_wait(handler);

//            cout << "io_service thread: " << boost::this_thread::get_id() << endl;
//            ioservice.run();
//        }

//        return 0;

        {
            io_service ioservice;

            steady_timer timer{ioservice, std::chrono::seconds{3}};
            timer.async_wait(handler);
            steady_timer timer2{ioservice, std::chrono::seconds{3}};
            timer2.async_wait(handler);
            steady_timer timer3{ioservice, std::chrono::seconds{3}};
            timer3.async_wait(handler);
            steady_timer timer4{ioservice, std::chrono::seconds{3}};
            timer4.async_wait(handler);
            steady_timer timer5{ioservice, std::chrono::seconds{3}};
            timer5.async_wait(handler);
            steady_timer timer6{ioservice, std::chrono::seconds{3}};
            timer6.async_wait(handler);

//            ioservice.post(handler);
//            ioservice.post(handler);
//            ioservice.post(handler);
//            ioservice.post(handler);
//            ioservice.post(handler);
//            ioservice.post(handler);
//            ioservice.post(handler);
//            ioservice.post(handler);

            std::thread thread1{[&ioservice](){ ioservice.run(); }};
            std::thread thread2{[&ioservice](){ ioservice.run(); }};
            std::thread thread3{[&ioservice](){ ioservice.run(); }};

            cout << "io_service thread(2): " << boost::this_thread::get_id() << endl;

            thread1.join();
            thread2.join();
            thread3.join();
        }

//        cout << "<<<<<<<<<<<" << endl;
//        {
//            io_service ioservice;
//            io_service ioservice2;

//            steady_timer timer{ioservice, std::chrono::seconds{3}};
//            timer.async_wait(handler);
//            steady_timer timer2{ioservice, std::chrono::seconds{3}};
//            timer2.async_wait(handler);
//            steady_timer timer3{ioservice, std::chrono::seconds{3}};
//            timer3.async_wait(handler);
//            steady_timer timer4{ioservice, std::chrono::seconds{3}};
//            timer4.async_wait(handler);
//            steady_timer timer5{ioservice, std::chrono::seconds{3}};
//            timer5.async_wait(handler);
//            steady_timer timer6{ioservice, std::chrono::seconds{3}};
//            timer6.async_wait(handler);

//            std::thread thread1{[&ioservice](){ ioservice.run(); }};
//            std::thread thread2{[&ioservice](){ ioservice.run(); }};
//            thread1.join();
//            thread2.join();
//        }
    }

//    return 0;

    // BIND
    {
        using namespace my_bind;

        auto f = std::bind(sum, 1, 42);
        cout << "f(): " << f() << endl;

        auto f2 = std::bind(sum, std::placeholders::_1, std::placeholders::_2);
        cout << "f (): " << f (2, 3) << endl;
        cout << "f2(): " << f2(2, 3) << endl;

        auto f3 = boost::bind(sum, _1, _2);
        cout << "f3(): " << f3(2, 3) << endl;

        auto f4 = boost::bind(sum, _2, _1);
        cout << "f4(): " << f4(2, 3) << endl;

        auto f5 = boost::bind(sum, 10, _1);
        cout << "f5(): "  << f5(2, 3) << endl;
        cout << "f5()*: " << f5(3) << endl;

        auto f5_1 = boost::bind(sum, 1, 2);
        cout << "f5_1(): "  << f5_1() << endl;

        A a;
        auto f6 = boost::bind(&A::Print, a);
        cout << "f6(): ";
        f6();

        auto f7 = boost::bind(&A::Print, a, _1);
        cout << "f7(): ";
        f7(34);
    }

    // COROUTINES
    {
        using namespace my_coroutines;

        // functor version
        generator gen(1, 10);
        while (gen)
        {
            std::cout << gen() << "\n";
        }

//        return 0;

        coroutine<void>::pull_type source (simple_coroutine);
        std::cout << ", ";
        source();
        std::cout << "!\n";

//        return 0;

        coroutine<int>::pull_type source_2 {boost::bind(generator_2, _1, 1, 10)};
        while(source_2)
        {
            std::cout << source_2.get() <<  " ";
            source_2();
        }

        coroutine<long long>::pull_type fib_gen (fib);
        while(fib_gen)
        {
            std::cout << fib_gen.get() <<  "\n";
            fib_gen();

            static int limit;
            if (limit++ > 40)
            {
                break;
            }
        }


        return 0;
    }


    {
        // TOKENIZER
        typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
        std::string s = "Boost C++ Libraries. This is the top-notch library, used as \
        a playground for new features, before approved in standard.";
        boost::char_separator<char> sep{" .,-"};
        tokenizer tok{s, sep};
        for (const auto &t : tok)
        {
            std::cout << t << '\n';
        }
    }

    {
        // SPLIT
        std::string s = "Boost C++ Libraries. This is the top-notch library, used as \
        a playground for new features, before approved in standard.";
        vector<string> tokens;
        boost::split(tokens, s, boost::is_any_of(" .,-"), boost::token_compress_on);
        for (const auto &t : tokens)
        {
            std::cout << t << '\n';
        }
    }

    {
        // MISC
        string str1="     hello world!     ";
        boost::trim(str1);
        cout << str1 << endl;
    }

    {
        // REPLACE
        string str1="Hello Dolly, Hello  Dolly, Hello World!";
        boost::replace_first(str1, "Dolly", "Jane");
        cout << str1 << endl;
        boost::replace_last(str1, "Hello", "Goodbye");
        cout << str1 << endl;
        boost::erase_all(str1, " ");
        cout << str1 << endl;
        boost::erase_head(str1, 6);
        cout << str1 << endl;
    }

    {
        using namespace boost::gregorian;
        using namespace boost::posix_time;

        boost::gregorian::date d{2017, 3, 6};
        std::cout << d.year() << '\n';
        std::cout << d.month() << '\n';
        std::cout << d.day() << '\n';
        std::cout << d.day_of_week() << '\n';
        std::cout << d.end_of_month() << '\n';

        date d1{2014, 2, 18};
        date d2{2017, 3, 6};
        date_duration dd1 = d2 - d1;
        std::cout << dd1.days() << '\n';

        date_duration dd{4};
        std::cout << dd.days() << '\n';
        weeks ws{4};
        std::cout << ws.days() << '\n';
        months ms{4};
        std::cout << ms.number_of_months() << '\n';
        years ys{4};
        std::cout << ys.number_of_years() << '\n';

        date d5{2014, 1, 31};
        months ms2(1);
        date d6 = d5 + ms2;
        std::cout << d6 << '\n';
        date d7 = d6 - ms2;
        std::cout << d7 << '\n';

        date d8{9999, 1, 1};
        date d9{9999, 2, 28};
        date_period dp{d8, d9};
        std::cout << dp.contains(d8) << '\n';
        std::cout << dp.contains(d9) << '\n';

        ptime pt1{date{2017, 3, 6}, time_duration{12, 0, 0}};
        time_duration td{6, 30, 0};
        ptime pt2 = pt1 + td;
        std::cout << pt2.time_of_day() << '\n';

        boost::posix_time::ptime t1 = boost::posix_time::microsec_clock::local_time();
        std::cout << t1.time_of_day() << '\n';
        boost::this_thread::sleep(boost::posix_time::millisec(500));
        boost::posix_time::ptime t2 = boost::posix_time::microsec_clock::local_time();
        std::cout << t2.time_of_day() << '\n';
        boost::posix_time::time_duration diff = t2 - t1;
        std::cout << diff.total_milliseconds() << std::endl;
    }

    {
        using namespace boost::spirit;

        {
            std::string s;
            std::getline(std::cin, s);
            auto it = s.begin();
            bool match = qi::parse(it, s.end(), ascii::digit);
            std::cout << std::boolalpha << match << '\n';
            if (it != s.end())
            {
                std::cout << std::string{it, s.end()} << '\n';
            }
        }

        {
            std::string s;
            std::getline(std::cin, s);
            auto it = s.begin();

//            bool match = qi::phrase_parse(it, s.end(), ascii::digit >> ascii::digit,
//              ascii::space);

//            bool match = qi::phrase_parse(it, s.end(),
//              qi::lexeme[ascii::digit >> ascii::digit], ascii::space);

//            bool match = qi::phrase_parse(it, s.end(), +ascii::digit, ascii::space);

//            bool match = qi::phrase_parse(it, s.end(), qi::int_, ascii::space);

            bool match = qi::phrase_parse(it, s.end(),
                         qi::int_[([](int i){ std::cout << i << '\n'; })], ascii::space);
            std::cout << std::boolalpha << match << '\n';
            if (it != s.end())
            {
                std::cout << std::string{it, s.end()} << '\n';
            }
        }
    }


    cout << "boost::this_thread: " << boost::this_thread::get_id() << endl;

    // ASIO
    // PYTHON

    return 0;
}
