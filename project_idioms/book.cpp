#include <iostream>

#include "book.h"

using namespace std;

//namespace
//{

struct BookImpl
{
    BookImpl(const string& title, const string& content)
        : m_title(title)
        , m_content(content)
    {
    }

    ~BookImpl()
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }

    void print()
    {
        cout << "Title: " << m_title << endl;
        cout << "Content: " << endl
             << m_content   << endl;
    }

private:
    string m_title;
    string m_content;
};

//}


Book::Book(const string& content, const string& title)
    : m_impl(new BookImpl(content, title))
{
}

Book::~Book()
{
    //delete m_impl;
}

void Book::print()
{
    m_impl->print();
}

