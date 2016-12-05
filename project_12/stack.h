#ifndef STACK_H
#define STACK_H


#include <iostream>

template<typename T>
class stack
{
public:
    stack();

    void push(const T& value);
    T pop();

private:
    T* m_data;
    int m_last;
};



template<>
class stack<bool>
{
public:
    stack()
        : m_data(new bool[16])
        , m_last(0)
    {
    }

    void push(const bool& value)
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;

        int num_elem = (m_last-1) / 8;
        int offset_elem = (m_last-1) % 8;

        m_data[num_elem] ^= (-value ^ m_data[num_elem])
                            & (1 << (7-offset_elem));

        m_last++;
    }

    bool pop()
    {
        return true;
    }

private:
    bool* m_data;
    int m_last;
};


#endif // STACK_H
