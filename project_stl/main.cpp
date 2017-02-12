#include <iostream>
#include <cassert>
#include <bits/stdc++.h>
#include <fstream>
#include "array.h"
#include "forward_list.h"
#include "list.h"

// Containers (summary):
//    - use vector as default container
//    - use array when fixed size is needed
//    - use map when association is needed
//    - use unordered_map when speed is crititcal, but order is not
//    - use set when need fast lookup/insert, order
//    - use multi_* of the above when don't need uniqueness
//    - use algorythm as the 1st resort


// Iterators:
// http://www.cplusplus.com/reference/iterator/

// Links:
// https://channel9.msdn.com/Series/C9-Lectures-Stephan-T-Lavavej-Standard-Template-Library-STL-/C9-Lectures-Introduction-to-STL-with-Stephan-T-Lavavej
// stroustrup.com/Programming/20_containers.ppt

// H/w:
//  1. Sort word frequency in reversed order
//  2. Concatenate all elements in vector<string>
//  3. Create deque? (http://stackoverflow.com/questions/6292332/what-really-is-a-deque-in-stl)

using namespace std;

struct Box1
{
    int value;
    char ch_value;
};

//bool operator<(const Box1 &a, const Box1 &b)
//{
//    return a.value<b.value;
//}

struct Box1Cmp
{
    bool operator()(const Box1 &a, const Box1 &b)
    {
        return a.value<b.value;
    }
};

struct Box
{
    int value;

    Box()
        : value(0)
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }

    Box(int value_)
        : value(value_)
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }

    Box(const Box& obj)
        : value(obj.value)
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }

    Box(Box&& obj) /*noexcept*/
        : value(move(obj.value))
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }

    Box& operator=(const Box& obj)
    {
        cout << __PRETTY_FUNCTION__ << endl;
        value = obj.value;
        return *this;
    }

    Box& operator=(Box&& obj)
    {
        cout << __PRETTY_FUNCTION__ << endl;
        value = move(obj.value);
        return *this;
    }
};


ostream& operator<< (ostream& os, const Box1& rhs)
{
    os << "{" << rhs.value << ": " << rhs.ch_value << "}";// << endl;
    return os;
}

ostream& operator<< (ostream& os, const Box& rhs)
{
    os << rhs.value;
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

template<typename T>
ostream& operator<< (ostream& os, const my::forward_list<T>& rhs)
{
    os << "{";
    for (const T& elem : rhs)
    {
        os << (&elem!=&(*rhs.begin()) ? ", " : "");
        os << elem;
    }
    os << "}";
    return os;
}

template<typename T, size_t N>
ostream& operator<< (ostream& os, const std::array<T, N>& rhs)
{
    os << "{";
    for (const T& elem : rhs)
    {
        os << (&elem!=&(*rhs.begin()) ? ", " : "");
        os << elem;
    }
    os << "}";
    return os;
}

template<typename T>
ostream& operator<< (ostream& os, const std::forward_list<T>& rhs)
{
    os << "{";
    for (const T& elem : rhs)
    {
        os << (&elem!=&(*rhs.begin()) ? ", " : "");
        os << elem;
    }
    os << "}";
    return os;
}

template<typename T>
ostream& operator<< (ostream& os, const my::list<T>& rhs)
{
    os << "{";
    for (const T& elem : rhs)
    {
        os << (&elem!=&(*rhs.begin()) ? ", " : "");
        os << elem;
    }
    os << "}";
    return os;
}

template<typename K, typename V,
                     typename Hash = std::hash<K>,
                     typename KeyEqual = std::equal_to<K>,
                     typename Allocator = std::allocator< std::pair<const K, V>>>
ostream& operator<< (ostream& os, const unordered_map<K, V, Hash, KeyEqual, Allocator>& rhs)
{
    os << rhs.size() << ": { ";
    for (const pair<K, V>& elem : rhs)
    {
        os << (elem.first!=rhs.begin()->first ? ", " : "");
        os << "(" << elem.first << "->" << elem.second << ")";
    }
    os << " }";
    return os;
}

template<typename K, typename V, typename Cmp = std::less<K>>
ostream& operator<< (ostream& os, const map<K, V, Cmp>& rhs)
{
    os << rhs.size() << ": { ";
    for (const pair<K, V>& elem : rhs)
    {
        os << (elem.first!=rhs.begin()->first ? ", " : "");
        os << "(" << elem.first << "->" << elem.second << ")";
    }
    os << " }";
    return os;
}

struct CmpLen
{
    bool operator()(const std::string& a, const std::string& b) const
    {
        return a.length() < b.length();
    }
};

struct HashT
{
    size_t operator()(const std::string& key) const
    {
        size_t result = 2166136261;

        for (size_t i = 0; i != key.size(); ++i) {
          result = (result * 16777619) ^ key[i];
        }

        return result ^ (1<< key.length());
    }
};

struct EqualT
{
    bool operator()(const std::string& key1, const std::string& key2) const
    {
        cout << __PRETTY_FUNCTION__ << endl;
        return key1==key2;
    }
};

struct Sum {
    Sum() { sum = 0; }
    void operator()(int n) { sum += n; }

    int sum;
};

template<typename T>
struct iter_selector
{
    const string iter_category;
};

template<>
struct iter_selector<std::bidirectional_iterator_tag>
{
    const string iter_category = "bidirectional_iterator";
};

template<>
struct iter_selector<std::random_access_iterator_tag>
{
    const string iter_category = "random_access_iterator";
};

template<>
struct iter_selector<std::input_iterator_tag>
{
    const string iter_category = "input_iterator";
};

template<>
struct iter_selector<std::output_iterator_tag>
{
    const string iter_category = "output_iterator";
};

template<>
struct iter_selector<std::forward_iterator_tag>
{
    const string iter_category = "forward_iterator";
};

template<class Iter>
void test_iter(Iter)
{
    cout << iter_selector<
            typename std::iterator_traits<Iter>::iterator_category>().iter_category << endl;
}

#define ARRAY_SIZE(a) sizeof(a)/sizeof(a[0])


int main()
{
    // STL = containers <-> itertors <-> algorithms
    {
        std::vector<int> v1 = {1,2,3,4};

        // [begin, end)

        for(/*vector<Box>::iterator*/ auto i = v1.begin(); i < v1.end(); ++i)
        {
            cout << *i << endl;
        }

        for(int* i = &v1[0]; i < &v1[0]+4; ++i)
        {
            cout << *i << endl;
        }

        std::list<int> l = { 0, 2, 5, 6, 3 }; // C++11 initializer lists
        std::vector<int> v = { 0, 2, 5, 6, 3 };  // C++11 initializer lists
        std::array<int, 5> a = { 0, 2, 5, 6, 3 };

        cout << (std::find(l.begin(), l.end(), 1) != l.end()) << endl;
        cout << (std::find(v.begin(), v.end(), 1) != v.end()) << endl;
        cout << (std::find(a.begin(), a.end(), 1) != a.end()) << endl;

        cout << std::count(l.begin(), l.end(), 5) << endl;
        cout << std::count(v.begin(), v.end(), 5) << endl;
        cout << std::count(a.begin(), a.end(), 5) << endl;

        sort(v.begin(), v.end());
        //stable_sort
        sort(a.begin(), a.end());

        cout << v << endl;

        int arr[] = {1,3,5,0,-7,15};
        sort(arr, arr+ARRAY_SIZE(arr));

        for(int elem : arr)
        {
            cout << elem << endl;
        }
    }


    // sequence containers: vector, list, deque
    {
        std::vector<Box> v = {{1}, {2}, {3}, {4}};
        std::vector<int> v2 = {1, 2, 3, 4, 5, 6, 7, 8, 2};

        cout << v << endl;

        //v.reserve(36);

        for(size_t i = 0; i < 20; i++)
        {
            ////v.insert(v.begin()+2, {42});
            v.push_back({42});
            cout << v << endl;
        }

        ////for(auto i = v.begin(); i != v.end(); ++i)
        for(auto i = v.begin(); i != v.end(); ++i)
        {
            i->value++;
        }
        cout << v << endl;

        // int
        auto it2 = find(v2.rbegin(), v2.rend(), 2);
        if (it2 != v2.rend())
        {
            cout << "found!" << endl;
            (*it2)++;
            cout << v2 << endl;
        }
        else
        {
            cout << "not found!" << endl;
        }

        // Box
        int value = 43;
        vector<Box>::reverse_iterator it = find_if(v.rbegin(),
                                           v.rend(),
                                           [value](const Box& box) {return box.value==value;});
        if (it != v.rend())
        {
            cout << "Found box!" << endl;
            //it->value++;
            cout << v << endl;
        }
        else
        {
            cout << "Box not found!" << endl;
        }

        //int
        {
            auto it = find(v2.begin(), v2.end(), 3);
            if (it != v2.end())
            {
                cout << v2 << endl;
                v2.erase(it);
                cout << v2 << endl;
            }
        }

        {
            for (auto it=v2.begin(); it!=v2.end();
                                          /*it++*/)
            {
                if(*it%2 == 0)
                   it = v2.erase(it);
                else
                   ++it;
            }

            cout << v2 << endl;
        }

        {
            std::vector<int> v2 = {1, 2, 3, 4, 5, 6, 7, 8, 2};
            // erase-remove idiom

            v2.erase( remove_if( v2.begin(),
                                 v2.end(),
                                 [](int& elem)
                                 { return !elem%2; } ), v2.end() );

            cout << v2 << endl;
        }

        {
            string str = "Текст с несколькими   пробелами";

            str.erase(remove(str.begin(), str.end(), ' '),
                      str.end());
            cout << str << '\n';
        }
    }

    {
        list<int> l = { 1, 1, 1 };

        l.push_front(2);
        l.push_back(3);

        auto it = std::find(l.begin(), l.end(), 1);
        if (it != l.end()) {
            l.insert(it, 42);
        }

        for (int n : l) {
            std::cout << n << '\n';
        }
    }

    {
        std::deque<int> d = {7, 5, 16, 8};

        d.push_front(21);
        d.push_back(42);

        for(int n : d)
        {
            std::cout << n << '\n';
        }
    }

    // associative containers: map, mutlimup, set, multiset, unordered_*
    {
        map<string, Box, CmpLen> m = {
                                {"aaaa", {42}},
                                {"c",    {43}},
                                {"bbb",  {41}},
                             };

        for (auto it=m.begin(); it!=m.end(); ++it)
        {
            cout << it->first << "->" << it->second << endl;
        }


        {
            for (auto it=m.begin(); it!=m.end();
                                          /*it++*/)
            {
                if (!(it->second.value%2))
                {
                    cout << "Found in map!" << endl;
                   it = m.erase(it);
                }
                else
                   ++it;
            }

            cout << m << endl;
        }
    }

    {
        map<string, int> m = {{"a", 1}};

        m["x"] = 77;
        m["x"] = 99;
        cout << m << endl;
        m["X"] = 88;
        cout << m << endl;

        auto it = m.find("yy");
        cout << (it != m.end()) << endl;

        cout << (m["yy"]==42) << endl;

        it = m.find("yy");
        cout << (it != m.end()) << endl;

        cout << m << endl;
    }


    {
        unordered_map<string, int, HashT, EqualT> m = {
                        {"a", 1},
                        {"b", 2},
                        {"c", 3},
                        {"d", 4}
                    };

        cout << m << endl;

        m["a"] = -1;
        cout << m["a"] << endl;

        auto it = m.find("yy");
        cout << (it != m.end()) << endl;
        cout << m["yy"] << endl;
        it = m.find("yy");
        cout << (it != m.end()) << endl;

        cout << m << endl;
    }

    cout << "HASH: " << endl;

    {
        float d = 42.0;
        cout << "hash<int>()(42): "     << hash<int>()(42) << endl;
        cout << "hash<double>()(42): "  << hash<float>()(42.) << endl;
        cout << "hash<double>()(42): "  << hash<float>()(42.1000001) << endl;
        cout << "double->int(42): "     << *(reinterpret_cast<long*>(&d)) << endl;
        cout << "hash<char>()('a'): "   << hash<char>()('a') << endl;
        cout << "hash<string>()(\"42\"): " << hash<string>()("aaaaaaaaaaaaaaaaaaaaaaaaaaa") << endl;
        cout << "hash<string>()(\"42\"): " << hash<string>()("aaaaaaaaaaaaaaaaaaaaaaaaaab") << endl;
    }

    return 0;


    {
        // count words
        unordered_map<string, int> words_count;

        ifstream file("test2.txt");
        string word;

        while (file >> word)
        {
            //int value = words_count[word];
            words_count[word]++;
        }

        for(auto entry : words_count)
        {
            cout << entry.first << ": " << entry.second << endl;
        }

        unordered_set<string/*, CmpLen*/> s = { "aaaa", "c", "bbb" };
        unordered_map<string, int> m = { {"aaaa", 42},
                                           {"c", 3},
                                           {"bbb", 33} };

        for (auto it=s.begin(); it!=s.end(); ++it)
        {
            cout << *it << endl;
        }

        for (auto it=m.begin(); it!=m.end(); ++it)
        {
            cout << it->first << endl;
        }
    }


    {
        set<string/*, CmpLen*/> s1 = { "aaaa", "c", "bbb" };

        set<string/*, CmpLen*/> s2 = { "a", "c", "c", "c", "c", "bb" };

        for (auto it=s2.begin(); it!=s2.end(); ++it)
        {
            cout << *it << endl;
        }

        vector<string> v;
        std::set<string/*, CmpLen*/> result;
        std::set_difference(s1.begin(), s1.end(),
                            s2.begin(), s2.end(),
                            std::back_inserter(v));
//                            v.begin());
//                            std::inserter(result, result.end()));

        for (auto elem : v)
        {
            cout << elem << endl;
        }
    }


    {
        std::vector<int> v1{1,2,3,4,5,6,7,8     };
        std::set<int>    v2{        5,  7,  9,10};
        std::sort(v1.begin(), v1.end());
        //std::sort(v2.begin(), v2.end());

        std::vector<int> xor_v;

        std::set_symmetric_difference(
            v1.begin(), v1.end(),
            v2.begin(), v2.end(),
            std::back_inserter(xor_v));

        cout << xor_v << endl;
     }


    // algorythms = non-modifying + modifying + sorting&relational
    {
        // non-modifying
        std::vector<int> v{1,1,1,1,1,10};
        std::vector<int> v2(v.size());

        cout << v << endl;
        std::for_each(v.begin(),
                      v.end(),
                      [](int &n){ n++; });

        Sum s = std::for_each(v.begin(), v.end(), Sum());

        cout << v << endl;
        cout << s.sum << endl;


        int sum     = accumulate(v.begin(), v.end(), 0);
        int sum2    = accumulate(v.begin(), v.end(), v[0]);
        int subtr   = accumulate(v.begin(), v.end(), 0, minus<int>());
        int product = accumulate(v.begin(), v.end(), 1, multiplies<int>());
        int product2= accumulate(v.begin(), v.end(), 1, [](const int& a, const int& b){ return a*b; });

        int min = accumulate(v.begin(), v.end(), v[0],
                [](const int& a, const int& b){ return std::min(a,b);});

        int max = accumulate(v.begin(), v.end(), v[0],
                [](const int& a, const int& b){ return ::max(a,b);});


        cout << sum << endl;
        cout << sum2 << endl;
        cout << subtr << endl;
        cout << product << endl;
        cout << product2 << endl;
        cout << min << endl;
        cout << max << endl;

        vector<int> vv = v;
        auto it = min_element(vv.begin(), vv.end());

        if (it != vv.end())
        {
            cout << *it << endl;
        }
        else
        {
            cout << "not found!" << endl;
        }

        {
            std::list<int> l = { 0, 2, 5, 6, 3 }; // C++11 initializer lists
            std::vector<int> v = { 0, 2, 5, 6, 3 };  // C++11 initializer lists
            std::array<int, 5> a = { 0, 2, 5, 6, 3 };

            cout << (std::find(l.begin(), l.end(), 1) != l.end()) << endl;
            cout << (std::find(v.begin(), v.end(), 1) != v.end()) << endl;
            cout << (std::find(a.begin(), a.end(), 1) != a.end()) << endl;

            cout << "Count if" << endl;
            cout << std::count_if(v.begin(), v.end(), [](const int& value){return value%2==0;}) << endl;
        }


        // find
        // find_if
        // TBD

        // equal
        // equal_if
        // TBD

        // all_of
        int value = 2;
        vector<Box> v0 = {{1},{2},{3}};
        cout << any_of(v0.begin(), v0.end(),
                       [&](const Box& b){return b.value%value==0;}) << endl;

        cout << count_if(v0.begin(), v0.end(),
                        [&](const Box& b){return b.value>=value;}) << endl;


        list<int> ll = {1,2,3};
        for(auto it = ll.rbegin(); it != ll.rend(); ++it)
        {
            cout << *it << endl;
        }
//        sort(ll.begin(), ll.end());


        // modifying
        std::fill (v.begin(),v.end()-1, 5);
        cout << v << endl;

        vector<int> v3 = v;
        vector<int> v44;

        std::fill (v.begin(),v.end(), 1);

        cout << "BEFORE: " << v << endl;

        std::transform(v.begin()+1, v.end(),
                       v.begin(),
                       v.begin()+1,
                       [](int &n, int &n1){ return n+n1; });

        cout << "AFTER: " << v << endl;

        cout << v2 << endl;
//        cout << v << endl;
        cout << v3 << endl;
        cout << v44 << endl;


        // replace
        // replace_if


        // copy
        // copy_if
        vector<Box> v4 = {{1},{2},{3},{4}};
        vector<Box> v5 = {{42}};
        copy_if(v4.begin(), v4.end(),
                //back_inserter(v5),
                v5.begin(),
                [](const Box& b){return b.value%2==0;});

        cout << v5 << endl;

        // sorting+relational
        {
            std::vector<int> v1{9,2,2,2,3,4,5,6,7,8,1};
            std::vector<Box1> v2{
                {2,'d'},
                {10,'a'},
                {2,'1'},
                {1,'2'},
                {133,'3'},
                {133,'e'},
                {2,'c'},
                {10,'b'},
                {13,'f'},
                {2,'4'},
                {1,'5'},
                {33,'6'},

            };

            std::sort(v2.begin(), v2.end(),
                      [](Box1 &a, Box1 &b){ return a.value<b.value; });

            cout << v2 << endl;


            for_each (v2.begin(), v2.end(),
                      [](Box1 &a){cout << a.value << ": " << a.ch_value << endl;});


            Box1 b1 = {32,'6'};
            bool b = binary_search(
                          v2.begin(),
                          v2.end(),
                          b1,
                          [](const Box1 &a, const Box1 &b)
                          { return a.value<b.value; });


            cout << b << endl;


            auto it = lower_bound(
                                    v2.begin(),
                                    v2.end(),
                                    b1,
                                    Box1Cmp());

            cout << "lower_bound: " << *it << "\n" << endl;
            v2.insert(it, b1);
            cout << v2 << endl;

            {
                ifstream file("/home/db/Downloads/Dropbox/_temp/QuickSort.txt");

                istream_iterator<int> start (file); //input iterator from stream
                istream_iterator<int> end; // end of stream iterator
                vector<int> v(start, end);
                cout << v << endl;

                file.clear();
                file.seekg(0);

                srand(time(0));
                vector<Box1> v2;
                transform(start,
                          end,
                          back_inserter(v2),
                          [](const int &elem){return Box1{elem, static_cast<char>('a'+rand()%('z'-'a'))};});

                ////copy(v2.begin(), v2.end(), ostream_iterator<Box1>(cout, ", "));

                vector<Box1> v_stable_sort = v2;
                stable_sort(v_stable_sort.begin(), v_stable_sort.end(), Box1Cmp());
                sort(v2.begin(), v2.end(), Box1Cmp());

                bool is_equal = equal(v_stable_sort.begin(), v_stable_sort.end(),
                      v2.begin(),
                      [](const Box1 &a, const Box1 &b){ return a.value==b.value && a.ch_value==b.ch_value; });

                cout << "stable equal to unstable?: " << boolalpha << is_equal << endl;
            }

            //nth_element
        }
    }

    // iterator categories
    // iterator creation
    {
        // as simple as pointers
        array<int,3> arr = {1,2,0};
        cout << &arr[0] << endl;
        cout << arr.begin() << endl;

        cout << (&arr[0])+3 << endl;
        cout << arr.end() << endl;

        my::array<int,3> arr2(arr.begin(), arr.size());
        for(auto it = arr2.begin(); it!=arr2.end(); ++it)
        {
            cout << *it << endl;
        }

        sort(arr2.begin(), arr2.end());

        my::array<int,3> arr3 = {1,2,3,4,5};

        for(auto elem: arr2)
        {
            cout << elem << endl;
        }

        test_iter(arr2.begin());
        test_iter(unordered_map<string, int>::iterator());
        test_iter(map<string, int>::iterator());
        test_iter(forward_list<int>::iterator());

        test_iter(my::forward_list<int>::iterator());
        test_iter(my::list<int>::iterator());


        //forward_list<int>::iterator.iterator_category;
        my::forward_list<int> fl;

        fl.push_front(42);
        fl.push_front(43);
        fl.push_front(44);

//        while(!fl.empty())
//        {
//            cout << fl.front() << endl;
//            fl.pop_front();
//        }


        for(auto it = fl.begin(); it!=fl.end(); ++it)
        {
            cout << *it << endl;
        }

        for(auto elem: fl)
        {
            cout << elem << endl;
        }


        my::forward_list<int> fl2(fl);
        cout << "COPY CTOR [fl]:" << fl << endl;
        cout << "COPY CTOR [fl2]:" << fl2 << endl;

        my::forward_list<int> fl3;
        fl3 = fl2;
        cout << "COPY ASSIGN [fl2]:" << fl2 << endl;
        cout << "COPY ASSIGN [fl3]:" << fl3 << endl;

        my::forward_list<int> fl4(move(fl3));
        cout << "MOVE CTOR [fl3 (src)]:" << fl3 << endl;
        cout << "MOVE CTOR [fl4 (dst)]:" << fl4 << endl;

        my::forward_list<int> fl5;
        fl5 = move(fl4);
        cout << "MOVE ASSIGN [fl4 (src)]:" << fl4 << endl;
        cout << "MOVE ASSIGN [fl5 (dst)]:" << fl5 << endl;

        fl2 = move(fl5);
        cout << "MOVE ASSIGN [fl5 (src)]:" << fl5 << endl;
        cout << "MOVE ASSIGN [fl2 (dst)]:" << fl2 << endl;

//        sort(fl.begin(), fl.end());

        cout << "copy_if: " << endl;
        copy_if(fl.begin(), fl.end(),
                arr2.begin(), [](const int& elem){return elem%2==0;});

        for(auto& e: arr2)
        {
            cout << e << endl;
        }


        // double-ended list
        cout << "double-ended list: " << endl;
        {
            my::list<int> dlist;

            dlist.push_back(1);
            dlist.push_back(2);
            dlist.push_back(3);

            // TEST should pass when methods are implemented

////            assert(dlist.empty()==false);

////            for(auto& elem: dlist)
////            {
////                cout << elem << endl;
////            }

////            cout << "dlist: " << dlist << endl;
////            my::list<int> dlist2 = dlist;
////            cout << "dlist2: " << dlist2 << endl;

////            assert(dlist.back() ==dlist2.back());
////            assert(dlist.front()==dlist2.front());

////            cout << "--reversed--" << endl;
////            for(auto it = dlist.rbegin(); it!=dlist.rend(); ++it)
////            {
////                cout << *it << endl;
////            }

////            {
////                my::list<string> list;
////                list.push_back("xxx");
////                list.pop_back();
////                list.push_back("first");
////                list.push_back("second");

////                list.reverse();

////                assert(*(list.begin()) == "second");
////                assert(*(list.end())   == "first");
////                cout << "list: " << list << endl;
////            }
            
////            cout << *(dlist.rbegin()) << endl;
////            assert(dlist.back() ==*(dlist.rbegin()));

////            dlist.pop_back();
////            cout << "popped back: " << dlist << endl;
////            assert(dlist.empty()==false);

////            dlist.pop_front();
////            cout << "popped front: " << dlist << endl;
////            assert(dlist.empty()==false);

////            dlist.pop_front();
////            cout << "popped front: " << dlist << endl;
////            assert(dlist.empty()==true);

////            dlist.push_front(11);
////            auto it = dlist.begin();
////            *(it) = 42;
////            cout << "push & modify: " << dlist << endl;

////            assert(*(  dlist.begin())   ==dlist.front());
////            assert(*(--dlist.end())     ==dlist.back());
////            assert(*(  dlist.rbegin())  ==dlist.back());
////            assert(*(--dlist.rend())    ==dlist.front());
////            assert(42 == dlist.front());

////            dlist.pop_back();
////            cout << "popped back: " << dlist << endl;
////            assert(dlist.empty()==true);

        }

        {
            cout << "moving/copying our container: " << endl;
            my::forward_list<Box> flist;

            flist.push_front(Box(1));
            flist.push_front(Box(2));
            flist.push_front(Box(3));

            my::forward_list<Box> flist2 = flist;
            cout << "flist: " << flist << endl;
            cout << "flist2: " << flist2 << endl;

            cout << "moving/copying std container: " << endl;
            std::forward_list<Box> flist_std;

            flist_std.push_front(Box(1));
            flist_std.push_front(Box(2));
            flist_std.push_front(Box(3));

            std::forward_list<Box> flist_std2 = flist_std;
            cout << "flist_std [COPY]: " << flist_std << endl;
            cout << "flist_std2 [COPY]: " << flist_std2 << endl;

            std::forward_list<Box> flist_std3 = move(flist_std);
            cout << "flist_std [MOVE]: " << flist_std << endl;
            cout << "flist_std3 [MOVE]: " << flist_std3 << endl;


            cout << "moving/copying std container [VECTOR]: " << endl;
            std::vector<Box> vector_std;

            vector_std.push_back(Box(1));
            vector_std.push_back(Box(2));
            vector_std.push_back(Box(3));

            cout << "vector_std [COPY]: " << vector_std << endl;
            std::vector<Box> vector_std2 = vector_std;
            cout << "vector_std2 [COPY]: " << vector_std2 << endl;

            std::vector<Box> vector_std3 = move(vector_std);
            cout << "vector_std [MOVE]: " << vector_std << endl;
            cout << "vector_std3 [MOVE]: " << vector_std3 << endl;



            cout << "moving/copying std container [ARRAY]: " << endl;
            std::array<Box, 5> arr_std;

            arr_std[0] = Box(1);
            arr_std[1] = Box(2);
            arr_std[2] = Box(3);

            cout << "arr_std [COPY]: " << arr_std << endl;
            std::array<Box, 5> arr_std2 = arr_std;
            cout << "arr_std2 [COPY]: " << arr_std2 << endl;

            cout << "arr_std [MOVE]: " << arr_std << endl;
            std::array<Box, 5> arr_std3 = move(arr_std);
            cout << "arr_std3 [MOVE]: " << arr_std3 << endl;

            return 0;
        }

        cout << "advance: " << endl;

        vector<int> v{ 1, 2, 3 };
        auto it = v.begin();
        advance(it, 1);

        cout << (*it == v[1]) << endl;
        it = v.begin();
        advance(it, v.size());
        cout << (it == v.end()) << endl;

        list<int> ll{ 3, 1, 4 };
        auto it2 = ll.begin();
        advance(it2, 2);
        cout << *it2 << endl;
        cout << (distance(ll.begin(), ll.end())==ll.size()) << endl;
    }

    return 0;
}

