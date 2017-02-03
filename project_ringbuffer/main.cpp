#include <iostream>
#include <array>
#include <cassert>

using namespace std;

template<typename T, size_t N>
class RingBuffer
{
    array<T, N> m_data;

public:

    size_t size() const
    {
        return N;
    }

    bool empty() const
    {
        return true;
    }

    void push(const T& elem)
    {

    }

    T& top()
    {
        return T();
    }

    void pop()
    {

    }
};



int main(int argc, char *argv[])
{
    RingBuffer<int, 1024> ringBuffer;

    assert(ringBuffer.empty()==true);

    return 0;
}
