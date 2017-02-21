#include <iostream>
#include <list>
#include <unordered_map>
#include <cassert>
#include <vector>
#include <algorithm>

using namespace std;

template<typename K, typename V>
class LRUCache
{
    list<K> m_list;
    unordered_map<K,pair<V, typename list<K>::iterator>> m_map;
    size_t m_max_size;

    const V& Value(const K& key)
    {
        return m_map[key].first;
    }

    void Shrink()
    {
        m_map.erase(m_map.find(m_list.back()));
        m_list.pop_back();
    }

    void Store(const K& key, const V& value)
    {
        m_list.push_front(key);
        m_map[key] = {value, m_list.begin()};
    }

    void Rejuvenate(const K& key)
    {
        m_list.erase(m_map[key].second);
        m_list.push_front(key);
        m_map[key] = {Value(key), m_list.begin()};
    }

public:
    LRUCache(size_t max_size)
        : m_list()
        , m_map(max_size)
        , m_max_size(max_size)
    {
    }

    // Put value by key
    void Put(const K& key, const V& value)
    {
        if (Exists(key))
        {
            Rejuvenate(key);
        }
        else
        {
            if (m_max_size == Size())
            {
                Shrink();
            }

            Store(key, value);
        }
    }

    // Retrieves an entry from the cache that has been cached before
    // using Put(). Returns the cached entry. Throws exception,
    // if there is no entry with the key K
    const V& Get(const K& key)
    {
        if (m_map.find(key) == m_map.end())
        {
            throw out_of_range("Missing element");
        }
        Rejuvenate(key);
        return Value(key);
    }

    // Checks presence of the value by the given key
    bool Exists(const K& key)
    {
        return (m_map.find(key) != m_map.end());
    }

    // Clears all the entries in the cache
    void Clear()
    {
        m_map.clear();
        m_list.clear();
    }

    // Returns actual size of the cache: number of currently available elems.
    size_t Size() const
    {
        return m_list.size();
    }
};

int main(int argc, char *argv[])
{
    LRUCache<int, string> cache(3);
    cache.Put(1, "1");
    cache.Put(2, "2");
    cache.Put(3, "3");

    {
        assert ("2" == cache.Get(2));
        assert ("1" == cache.Get(1));
    }

    {
        cache.Put(4, "4");
        assert (0 == cache.Exists(3));
    }

    {
        assert (0 == cache.Exists(42));
        bool thrown = false;
        try
        {
            cache.Get(42);
        }
        catch (exception& ex)
        {
            thrown = true;
        }
        assert(thrown);
    }

    {
        cache.Get(2);
        cache.Get(1);
        cache.Get(4);
        cache.Get(2);
        cache.Get(2);
        cache.Put(4, "4");
        assert (cache.Exists(1));
        assert (cache.Exists(2));
        assert (cache.Exists(4));
    }

    {
        cache.Clear();
        assert (0 == cache.Size());
        assert (0 == cache.Exists(1));
        assert (0 == cache.Exists(2));
    }

    return 0;
}
