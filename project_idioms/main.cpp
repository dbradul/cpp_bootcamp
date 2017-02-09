#include <iostream>
#include <vector>
#include <memory>
#include <bits/stdc++.h>

#include "book.h"

using namespace std;

// SFINAE
// CRTP
// RAII
// Tag dispatch
// Pimpl
// Copy-n-swap
// Type erasure


// H/W:
// - implements Observer
// - dispatch tag


namespace RAII
{
    class File_handle
    {
        FILE* p;

    public:
        File_handle(const char* n, const char* a)
        {
            cout << __PRETTY_FUNCTION__ << endl;
            p = fopen(n,a);
            if (p==0) throw -1;//std::exception(-1);
        }

        File_handle(FILE* pp)
        {
            p = pp;
            if (p==0) throw -1;//std::exception(-1);
        }

        ~File_handle()
        {
            cout << __PRETTY_FUNCTION__ << endl;
            fclose(p);
        }

        operator FILE*() { return p; }
        // ...
    };

}



namespace PImpl
{

struct Book
{
    void print()
    {
    }

private:
    string m_content;
    string m_title;
    string m_edition;
};

}












namespace PImpl_v1
{

struct Book
{
    Book()
    {
    }

    ~Book()
    {
        delete m_impl;
    }

    void print(){}

private:
    struct BookImpl;

    BookImpl* m_impl;
};


struct BookImpl
{
    BookImpl()
    {
    }

    ~BookImpl()
    {
    }

    void print(){}

private:
    string m_content;
    string m_title;
};

}


namespace type_erasuse
{
    struct Cacheable
    {
        virtual ~Cacheable(){}
        virtual Cacheable* clone() const {return nullptr;}
    };

    template<typename T>
    struct Value : Cacheable
    {
        T m_data;

        Value(const T& val)
            : m_data(val)
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }

        Value(const Value& rhs)
            : Cacheable(*(rhs.clone()))
            , m_data(rhs.m_data)
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }

        Cacheable* clone() const override
        {
            cout << __PRETTY_FUNCTION__ << endl;
            return new Value(*this);
        }

        T& data()
        {
            return m_data;
        }
    };

    struct Box
    {
        int value;
        char ch_value;
    };

}

namespace dispatcher_tag
{
    template<typename T>
    size_t distance_impl(T iter1, T iter2, random_access_iterator_tag)
    {
        cout << __PRETTY_FUNCTION__ << endl;

        return iter2 - iter1;
    }

    template<typename T>
    size_t distance_impl(T iter1, T iter2, bidirectional_iterator_tag )
    {
        cout << __PRETTY_FUNCTION__ << endl;

        size_t dist = 0;

        for(; iter1 != iter2; ++iter1, ++dist);

        return dist;
    }

    template<typename T>
    size_t distance(T iter1, T iter2)
    {
        using iter_category = typename std::iterator_traits<T>::iterator_category;
        return distance_impl(iter1, iter2, iter_category());
    }
}



namespace copy_n_swap
{

class String
{
private:
    char* m_buffer;
    size_t* m_refCounter;

    void attach()
    {
        (*m_refCounter)++;
    }

    void detach()
    {
        (*m_refCounter)--;
        if(*m_refCounter == 0)
        {
            delete []m_buffer;
            delete []m_refCounter;
        }
    }

public:

    String() : m_buffer(nullptr), m_refCounter(new size_t(1))
    {
        cout << "String : "<< this <<endl;
    }

    String(const char* value)
        : m_buffer(new char[strlen(value)+1])
        , m_refCounter(new size_t(0))
    {
        strcpy(m_buffer, value);
        attach();
    }

    String(const String& rhs)
        : m_buffer(rhs.m_buffer)
        , m_refCounter(rhs.m_refCounter)
    {
        attach();
    }

    ~String()
    {
        detach();
    }

    void swap(String& other)
    {
        std::swap(m_buffer, other.m_buffer);
        std::swap(m_refCounter, other.m_refCounter);
    }

    String& operator= (const String& rhs)
    {
        if (&rhs != this)
        {
            String tmp(rhs);
            swap(tmp);
        }
        return *this;
    }

    String& operator= (const char* value)
    {
        String(value).swap(*this);
        return *this;
    }

    const char& operator[](size_t index)
    {
        return m_buffer[index];
    }

    size_t count() const
    {
        return *m_refCounter;
    }

    void set_elem(size_t index, char value)
    {
        if (count()>1)
        {
            String tmp(m_buffer);
            std::swap(*this, tmp);
        }
        m_buffer[index] = value;
    }

    const char *data()const
    {
        return m_buffer;
    }
};

}



int main(int argc, char *argv[])
{
    try
    {
        using namespace RAII;
        ////http://prnt.sc/dzp3f2

        File_handle a("/tmp/null", "a+");

        throw "failure";
    }
    catch(...)
    {
        cout << "terminated!" << endl;
    }


    {
        using namespace PImpl;
        PImpl::Book b;
    }


    Book b("B. Stroustrup. C++ language", "Long time ago...");
    b.print();


    {
        using namespace dispatcher_tag;

        int arr[] = {1,2,3};
        vector<int> v = {1,2,3,4,5};
        map<double, string> m = {{1,"a"}, {2, "b"}, {3, "c"}};

        cout << dispatcher_tag::distance(&arr[0], arr+3) << endl;
        cout << dispatcher_tag::distance(v.begin(), v.end()) << endl;
        cout << dispatcher_tag::distance(m.begin(), m.end()) << endl;
    }


    {
        using namespace copy_n_swap;
        String s("abc");
        assert(s.count()==1);
        {
            String s2 = s;
            assert(s.count()==2);
            assert(s2.count()==2);
        }
        assert(s.count()==1);
        String s3 = s;
        String s4 = "ass";
        assert(strcmp(s4.data(), "ass")==0);
        assert(s.count()==2);
        s3.set_elem(0, 'X');
        assert(s.count()==1);
        assert(s3.count()==1);
        assert(strcmp(s.data(), "abc")==0);
        assert(strcmp(s3.data(), "Xbc")==0);
    }

    return 0;
}
