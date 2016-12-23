#ifndef STACK_H
#define STACK_H


#include <iostream>

template<typename T, int N=16>
class stack
{
public:
    stack()
        : m_data()
        , m_last(0)
    {

    }

    void push(const T& value)
    {
        m_data[m_last++] = value;
    }

    T pop()
    {
        return m_data[m_last--];
    }

private:
    T m_data[N];
    int m_last;
};



template<int N>
class stack<bool, N>
{
public:
    stack()
        : m_data()
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
    bool m_data[N];
    int m_last;
};


#endif // STACK_H
