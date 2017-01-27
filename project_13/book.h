#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <memory>

struct BookImpl;

struct Book
{
    Book(const std::string& content, const std::string& title);
    ~Book();
    void print();

private:

    BookImpl* m_impl;
};


#endif // BOOK_H
