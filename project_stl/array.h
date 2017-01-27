#ifndef ARRAY_H
#define ARRAY_H

#include <algorithm>
#include <iostream>


namespace my {

template<typename T, size_t N>
class array
{
public:
    array (T* arr, size_t n)
    {
        std::copy(arr, arr+std::min(n,N), m_data);
    }

    array (std::initializer_list<T> list)
    {
        std::copy(list.begin(), list.begin()+std::min(N,list.size()), m_data);
    }

    T operator[](size_t idx)
    {
        return m_data[idx];
    }

    T* begin()
    {
        return &m_data[0];
    }

    T* end()
    {
        return m_data+N;
    }

private:
    T m_data[N];
};


}


#endif // ARRAY_H
