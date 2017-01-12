#include <iostream>
#include <cassert>
#include <vector>
#include <list>
#include <array>
#include <algorithm>
#include <functional>
#include <array>
#include <map>

using namespace std;

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

    Box& operator=(const Box& obj)
    {
        cout << __PRETTY_FUNCTION__ << endl;
        value = obj.value;
        return *this;
    }
};


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
        os << elem << ", ";
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
        os << "(" << elem.first << "->" << elem.second << "), ";
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







#define ARRAY_SIZE(a) sizeof(a)/sizeof(a[0])

int main()
{
    // STL = containers <-> itertors <-> algorithms
    {
        std::vector<int> v1 = {1,2,3,4};

        // [begin, end)

/*vector<Box>::iterator*/
        for(auto i = v1.begin(); i < v1.end(); ++i)
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

        //return 0;

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

//        {
//            for (auto it=v2.begin(); it!=v2.end();
//                                          /*it++*/)
//            {
//                if(*it%2 == 0)
//                   it = v2.erase(it);
//                else
//                   ++it;
//            }

//            cout << v2 << endl;
//        }



        {
//            std::vector<int> v2 = {1, 2, 3, 4, 5, 6, 7, 8, 2};
//            // erase-remove idiom
//            auto it = remove_if( v2.begin(),
//                                 v2.end(),
//                                 [](int& elem)
//            {
//                cout << __PRETTY_FUNCTION__ << ": " << elem << " " << elem%2 << endl;
//                return !elem%2;
//            } );

//            cout << *(it-1) << endl;

//            v2.erase( it, v2.end() );

//            cout << v2 << endl;

            std::vector<int> v(10);
            for (unsigned i = 0; i < v.size(); ++i)
              v[i] = i;
            cout << v << endl;

            // видаляємо всі елементи рівні 5
            v.erase( std::remove( v.begin(), v.end(), 5 ), v.end() );
            cout << v << endl;

            // видаляємо всі непарні елементи
            v.erase( std::remove_if( v.begin(), v.end(),

                                     [](int& elem)
                                                 {
                                                     cout << __PRETTY_FUNCTION__ << ": " << elem << " " << elem%2 << endl;
                                                     return !(elem%2);
                                                 }

                                     ), v.end() );

            cout << v << endl;

        }

        {
            string str = "Текст с несколькими   пробелами";
            str.erase(remove(str.begin(), str.end(), ' '),
                      str.end());
            cout << str << '\n';
        }

    }

    // associative containers: map, mutlimup, set, multiset, unordered_*
    {
//        map<string, Box, CmpLen> m = {  {"aaaa", {42}},
        map<string, Box, CmpLen> m =
                             {
                                {"aaaa", {42}},

                                {"c", {43}},

                                {"bbb", {41}},
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

        cout << m["yy"] << endl;
        auto it = m.find("yy");

        cout << (it != m.end()) << endl;

        cout << m << endl;
    }

    return 0;
}
