#include <iostream>
#include <array>
#include <cassert>
#include <vector>
#include <memory>

using namespace std;

template<typename T, size_t N>
class RingBuffer
{
    array<T, N> m_data;

    typedef unsigned long long tSize;

    unsigned long long m_start = 0;
    unsigned long long m_end = 0;

public:

    size_t size() const
    {
        return N;
    }

    size_t count() const
    {
        return m_start - m_end;
    }

    bool empty() const
    {
        tSize start = m_start % N;
        tSize end   = m_end % N;

        bool result = false;

        if (start == end)
        {
            if (m_start == m_end)
            {
               result = true;
            }
        }

        return result;
    }

    bool full() const
    {
        tSize start = m_start % N;
        tSize end   = m_end % N;

        bool result = false;

        if (start == end)
        {
            if (m_start > m_end)
            {
               result = true;
            }
        }

        return result;
    }

    void push_back(const T& elem)
    {
        if (full())
        {
            throw out_of_range("Buffer is full");
        }

        tSize start = m_start%N;
        m_data[start] = elem;
        ++m_start;
    }

    T& front()
    {
        if (empty())
        {
            throw out_of_range("Buffer is emtpy");
        }

        return m_data[m_end%N];
    }

    const T& front() const
    {
        if (empty())
        {
            throw out_of_range("Buffer is emtpy");
        }

        return m_data[m_end%N];
    }

    void pop_front()
    {
        if (empty())
        {
            throw out_of_range("Buffer is emtpy");
        }

        ++m_end;
    }

};



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

    Box(Box&& obj) noexcept
        : value(move(obj.value))
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }

    Box& operator=(const Box& obj)
    {
        cout << __PRETTY_FUNCTION__ << endl;
        value = obj.value;
        return *this;
    }

    ~Box()
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }
};


int main(int argc, char *argv[])
{
//    cout << sizeof(unsigned long long) << endl;
//    return 0;


    const size_t N = 1024;
    RingBuffer<int, N> ringBuffer;

    {
        // initial state
        assert(ringBuffer.empty()==true);
        assert(ringBuffer.full()==false);
        assert(ringBuffer.count()==0);
    }

    {
        // one push
        ringBuffer.push_back(42);
        assert(ringBuffer.empty()==false);
        assert(ringBuffer.full()==false);
        assert(ringBuffer.count()==1);
    }

    {
        // one pop
        ringBuffer.pop_front();
        assert(ringBuffer.empty()==true);
        assert(ringBuffer.full()==false);
        assert(ringBuffer.count()==0);
    }

    {
        // N push, N pop
        for(size_t i = 0; i < N/2 ; ++i)
        {
            ringBuffer.push_back(42);
        }
        assert(ringBuffer.count()==N/2);

        for(size_t i = 0; i < N/2 ; ++i)
        {
            ringBuffer.pop_front();
        }

        assert(ringBuffer.empty()==true);
        assert(ringBuffer.full()==false);
        assert(ringBuffer.count()==0);
    }

    {
        // cross over buffer boundaries
        for(size_t i = 0; i < N ; ++i)
        {
            ringBuffer.push_back(42);
        }
        assert(ringBuffer.full()==true);
        assert(ringBuffer.count()==N);

        bool exception_thrown = false;
        try
        {
            ringBuffer.push_back(42);
        }
        catch (exception& ex)
        {
            exception_thrown = true;
        }
        assert(exception_thrown==true);
        assert(ringBuffer.count()==N);
    }

    cout << "PASSED" << endl;


    {
        ///buffer = std::make_shared<std::array<char, 64>>();
        auto ptr = make_shared<std::array<Box, 10>>();
        //auto ptr = make_shared<Box>();
    }

    return 0;
}
