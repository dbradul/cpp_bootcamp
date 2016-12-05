#include "stack.h"
#include <string>


template<typename T>
stack<T>::stack()
    : m_data(new T[16*8])
    , m_last(0)
{
}

template<typename T>
void stack<T>::push(const T& value)
{
    // TODO: add bounds check + reallocation
    m_data[m_last++] = value;
}

template<typename T>
T stack<T>::pop()
{
    // TODO: add bounds check
    return m_data[m_last--];
}


template class stack<int>;
template class stack<char>;
template class stack<bool>;
template class stack<std::string>;
