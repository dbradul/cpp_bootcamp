#include <iostream>
#include <sstream>
#include <cassert>
#include <algorithm>

using namespace std;


template<typename T>
class range_iterator : public std::iterator<std::input_iterator_tag, T>
{
public:
    range_iterator(size_t B, size_t E, size_t S)
        : m_current(B)
        , m_end(E)
        , m_step(S)
    {
    }

    range_iterator(const range_iterator& rhs)
        : m_current(rhs.m_current)
        , m_end(rhs.m_end)
        , m_step(rhs.m_step)
    {
    }

    range_iterator& operator++()
    {
        m_current = std::min(m_current+m_step, m_end);
        return *this;
    }

    range_iterator operator++(int)
    {
        range_iterator tmp(*this);
        ++(*this);
        return tmp;
    }

    T& operator*()
    {
        return m_current;
    }

    bool operator==(const range_iterator& rhs)
    {
        return m_current == rhs.m_current;
    }

    bool operator!=(const range_iterator& rhs)
    {
        return !(*this==rhs);
    }

    private:
    size_t m_current;
    size_t m_end;
    size_t m_step;
};


template<size_t B, size_t E, size_t S=1>
class Range
{
public:
    typedef range_iterator<size_t> iterator;
    typedef range_iterator<const size_t> const_iterator;

    Range(){}

    iterator begin()
    {
        return iterator(B, E, S);
    }

    iterator end()
    {
        return iterator(E, E, S);
    }

    const_iterator begin() const
    {
        return const_iterator(B, E, S);
    }

    const_iterator end() const
    {
        return const_iterator(E, E, S);
    }

    const_iterator cbegin() const
    {
        return const_iterator(B, E, S);
    }

    const_iterator cend() const
    {
        return const_iterator(E, E, S);
    }
};


template<size_t B, size_t E, size_t S>
ostream& operator<<(ostream& os, const Range<B,E,S>& r)
{
    for (auto& e : r)
    {
        os << e;
    }
    return os;
}


int main(int argc, char *argv[])
{
    {
        stringstream ss;
        ss << Range<1,10>();
        assert (ss.str() == "123456789");
    }

    {
        stringstream ss;
        ss << Range<0,10,2>();
        assert (ss.str() == "02468");
    }

    {
        stringstream ss;
        ss << Range<0,10,20>();
        assert (ss.str() == "0");
    }

    {
        auto r = Range<1,30>();

        assert (count_if(r.begin(), r.end(), [](int e){return e>20;}) == 9);
        assert (find(r.begin(), r.end(), 42) == r.end());
        assert (find(r.begin(), r.end(), 12) != r.end());
        assert (min_element (r.begin(), r.end()) == r.begin());
        assert (is_sorted   (r.begin(), r.end()) == true);
    }

    return 0;
}
