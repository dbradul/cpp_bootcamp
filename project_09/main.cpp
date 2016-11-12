#include <iostream>
#include <bitset>
#include <new>
#include <list>
#include <cstring>
#include <utility>

using namespace std;

typedef unsigned char byte;

// placement new
// user-defined allocator
// overload standard new and delete

namespace trivial_placement
{
    class A
    {
        int m_data;
    public:
        A(int value):
            m_data(value)
        {
        }

        ~A()
        {
            m_data = -1;
        }
    };
}

//namespace
//{

    class Arena
    {
        #define ACTIVE_POOL(n) (((n)==0) ? m_pool : m_pool2)
        #define DEFRAG_POOL(n) (((n)==1) ? m_pool : m_pool2)
        #define SWAP_POOLS(n)  (((n)==1) ? 0 : 1)

        struct AddrInfo
        {
            byte* m_initAddr;
            byte* m_currAddr;
        };

        size_t m_poolSize;
        byte*  m_pool;
        byte*  m_pool2;
        byte   m_activePool;
        size_t m_poolSeek;
        list<pair<AddrInfo, size_t>> m_busySlots;

        public:
            Arena(size_t poolSize = 4096)
              : m_poolSize(poolSize/2)
              , m_pool(new byte[m_poolSize])
              , m_pool2(new byte[m_poolSize])
              , m_activePool(0)
              , m_poolSeek(0)
              , m_busySlots()
            {
                cout << "m_pool: " << m_pool << endl;
                cout << "m_pool2: " << m_pool2 << endl;
            }

            ~Arena()
            {
                if (m_pool)
                {
                    delete[] m_pool;
                }

                if (m_pool2)
                {
                    delete[] m_pool2;
                }
            }

            void* allocate(size_t size)
            {
                cout << __PRETTY_FUNCTION__ << endl;
                byte* new_address = 0;

                // FIXME: concurrent access issue!
                if (m_poolSeek+size > m_poolSize)
                {
                    cout << "Defrag started!" << endl;

                    byte* activePool = ACTIVE_POOL(m_activePool);
                    byte* defragPool = DEFRAG_POOL(m_activePool);

                    // stop-slot
                    m_busySlots.push_back(pair<AddrInfo, size_t>({nullptr,nullptr}, 0));

                    size_t defrag_seek = 0;
                    while(true)
                    {
                        //TODO: exception safety!
                        auto slot = m_busySlots.front();
                        m_busySlots.pop_front();
                        if (slot.first.m_initAddr == nullptr)
                        {
                            m_activePool = SWAP_POOLS(m_activePool);
                            m_poolSeek   = defrag_seek;
                            break;
                        }
                        else
                        {
                            byte* new_pool_address = defragPool + defrag_seek;
                            memcpy(new_pool_address,
                                   slot.first.m_currAddr,
                                   slot.second);

                            m_busySlots.push_back({{slot.first.m_initAddr, new_pool_address},
                                                    slot.second});

                            defrag_seek += slot.second;
                        }
                    }
                }

                byte* activePool = ACTIVE_POOL(m_activePool);
                cout << "activePool: " << activePool << endl;

                if (m_poolSeek+size > m_poolSize)
                {
                    cout << "~~~memory exhausted!~~~" << endl;
                    // throw exception;
                }

                else
                {
                    new_address = activePool + m_poolSeek;
                    m_poolSeek += size;
                    m_busySlots.push_back({{new_address,
                                            new_address}, size});
                    cout << "new_address: " << new_address << endl;
                }

                return new_address;
            }

            void deallocate(void* ptr)
            {
                cout << __PRETTY_FUNCTION__
                     << "m_busySlots.size(): "
                     << m_busySlots.size() << endl;

                // TODO: replace linear scan with constant time lookup
                m_busySlots.remove_if([&](const pair<AddrInfo, size_t>& elem)
                                         {
                                            bool result = (elem.first.m_initAddr == ptr);
                                            if (result)
                                            {
                                                cout << "[Deallocate] "
                                                     << elem.first.m_initAddr
                                                     << ", size=" << elem.second
                                                     << endl;
                                            }
                                            return result;
                                         });

                cout << "m_busySlots.size(): " << m_busySlots.size() << endl;
            }
    };


    void* operator new(size_t sz, Arena& a)
    {
        cout << __PRETTY_FUNCTION__ << ", size requested (bytes): " << sz << endl;
        void* result = a.allocate(sz);
        cout << "result: " << result << endl;
        return result;
    }

    void* operator new[](size_t sz, Arena& a)
    {
        cout << __PRETTY_FUNCTION__ << ", size requested (bytes): " << sz << endl;
        void* result = a.allocate(sz);
        cout << "result: " << result << endl;
        return result;
    }

    template<class T> void destroy(T* ptr, Arena& a)
    {
        if (ptr)
        {
            ptr->~T();// explicit destructor call
            a.deallocate(ptr);
        }
    }

    void* operator new(size_t sz)
    {
        cout << __PRETTY_FUNCTION__ << ", size requested (bytes): " << sz << endl;
        void* ptr = malloc(sz);
        cout << __PRETTY_FUNCTION__ << ", memory allocated: " << ptr << endl;
        return ptr;
    }

    void operator delete(void* ptr)
    {
        cout << __PRETTY_FUNCTION__ << ", freed memory: " << ptr << endl;
        free(ptr);
    }

    void* operator new[](size_t sz)
    {
        cout << __PRETTY_FUNCTION__ << ", size requested (bytes): " << sz << endl;
        void* ptr = malloc(sz);
        cout << __PRETTY_FUNCTION__ << ", memory allocated: " << ptr << endl;
        return ptr;
    }

    void operator delete[](void* ptr)
    {
        cout << __PRETTY_FUNCTION__ << ", freed memory: " << ptr << endl;
        free(ptr);
    }

    class A2
    {
        int m_data;
    public:

        A2(int value=1):
            m_data(value)
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }

        ~A2()
        {
            m_data = -1;
        }
    };
//}





int main(int argc, char *argv[])
{

    {
        using namespace trivial_placement;
        char Buffer[4000] = {0};

        cout << "Buffer[0] = " << bitset<8>(Buffer[0]) << endl;
        cout << "Buffer[1] = " << bitset<8>(Buffer[1]) << endl;
        cout << "Buffer[2] = " << bitset<8>(Buffer[2]) << endl;
        cout << "Buffer[3] = " << bitset<8>(Buffer[3]) << endl;

        A* pA = new(Buffer) A(42);

        cout << "Buffer[0] = " << bitset<8>(Buffer[0]) << endl;
        cout << "Buffer[1] = " << bitset<8>(Buffer[1]) << endl;
        cout << "Buffer[2] = " << bitset<8>(Buffer[2]) << endl;
        cout << "Buffer[3] = " << bitset<8>(Buffer[3]) << endl;

        pA->~A();

        cout << "Buffer[0] = " << bitset<8>(Buffer[0]) << endl;
        cout << "Buffer[1] = " << bitset<8>(Buffer[1]) << endl;
        cout << "Buffer[2] = " << bitset<8>(Buffer[2]) << endl;
        cout << "Buffer[3] = " << bitset<8>(Buffer[3]) << endl;
    }

    {
        using namespace trivial_placement;
        char Buffer[4000] = {0};

        cout << "Buffer[0] = " << bitset<8>(Buffer[0]) << endl;
        cout << "Buffer[1] = " << bitset<8>(Buffer[1]) << endl;
        cout << "Buffer[2] = " << bitset<8>(Buffer[2]) << endl;
        cout << "Buffer[3] = " << bitset<8>(Buffer[3]) << endl;

        A* pA = new(Buffer) A(42);

        cout << "Buffer[0] = " << bitset<8>(Buffer[0]) << endl;
        cout << "Buffer[1] = " << bitset<8>(Buffer[1]) << endl;
        cout << "Buffer[2] = " << bitset<8>(Buffer[2]) << endl;
        cout << "Buffer[3] = " << bitset<8>(Buffer[3]) << endl;

        A* pA2 = new(Buffer) A(1);

        cout << "Buffer[0] = " << bitset<8>(Buffer[0]) << endl;
        cout << "Buffer[1] = " << bitset<8>(Buffer[1]) << endl;
        cout << "Buffer[2] = " << bitset<8>(Buffer[2]) << endl;
        cout << "Buffer[3] = " << bitset<8>(Buffer[3]) << endl;

        pA->~A();

        cout << "Buffer[0] = " << bitset<8>(Buffer[0]) << endl;
        cout << "Buffer[1] = " << bitset<8>(Buffer[1]) << endl;
        cout << "Buffer[2] = " << bitset<8>(Buffer[2]) << endl;
        cout << "Buffer[3] = " << bitset<8>(Buffer[3]) << endl;
    }

    {
        cout << "__global_new_overload__" << endl;

        double* p_int = new double(5);
        delete p_int;
    }

    {
        cout << "__global_new[]_overload__" << endl;

        double* p_int = new double[5];
        delete[] p_int;
    }

    {
        cout << "__allocator__" << endl;

        Arena a;

        double* p_int = new(a) double[128];
        cout << "p_int: " << p_int << endl;
        p_int[0] = 42;

        double* p_int2 = new(a) double[128];
        cout << "p_int2: " << p_int2 << endl;
        p_int2[0] = 42;

        double* p_int3 = new(a) double[128];

        destroy(p_int, a);

        p_int3 = new(a) double[128];
        cout << "p_int3: " << p_int3 << endl;
        if (p_int3)
        {
            p_int3[0] = 42;
        }

        cout << "p_int: "   << p_int << endl;
        cout << "p_int2: "  << p_int2 << endl;
        cout << "*p_int: "  << *p_int << endl;
        cout << "*p_int2: " << *p_int2 << endl;

        destroy(p_int2, a);
        destroy(p_int3, a);
    }

    return 0;
}
