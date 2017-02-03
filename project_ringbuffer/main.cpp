#include <iostream>
#include <array>


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
        return false;
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
    cout << "Hello World!" << endl;
    return 0;
}
