#ifndef SHARED_PTR_H
#define SHARED_PTR_H

#include <algorithm>
#include <atomic>

#include "unique_ptr.h"

using namespace std;

namespace my {

    template<typename T, typename Deleter=default_deleter<T>>
    class shared_ptr
    {
    public:

        struct ControlBlock {
            ControlBlock(): m_useCount(0)
            { }
            std::atomic<long> m_useCount;
        };

        shared_ptr()
          : m_data(nullptr)
          , m_control(new ControlBlock)
        {
        }

        shared_ptr(T* ptr)
          : m_data(ptr)
          , m_control(new ControlBlock)
        {
            attach();
        }

        shared_ptr(const shared_ptr& rhs)
          : m_data(rhs.m_data)
          , m_control(rhs.m_control)
        {
            attach();
        }

        shared_ptr& operator=(const shared_ptr& rhs)
        {
            if (this != &rhs)
            {
                detach();

                m_data = rhs.m_data;
                m_control = rhs.m_control;

                attach();
            }

            return *this;
        }

        long use_count() const
        {
            return m_control->m_useCount;
        }

        void reset(T* ptr)
        {
            detach();
            m_data = ptr;
            m_control = new ControlBlock;
            attach();
        }

        T* get() const
        {
            return m_data;
        }

        inline bool operator>(const shared_ptr& rhs) const
        {
            return m_data > rhs.m_data;
        }

        inline bool operator<(const shared_ptr& rhs) const
        {
            return rhs.operator>(*this);
        }

        inline bool operator==(const shared_ptr& rhs) const
        {
            return !operator<(rhs) && !operator>(rhs);
        }

        inline bool operator!=(const shared_ptr& rhs) const
        {
            return !operator==(rhs);
        }

        inline bool operator>=(const shared_ptr& rhs) const
        {
            return operator==(rhs) || operator>(rhs);
        }

        inline bool operator<=(const shared_ptr& rhs) const
        {
            return operator==(rhs) || operator<(rhs);
        }

        T* operator->()
        {
            return m_data;
        }


        T& operator*()
        {
//            if (!m_data)
//            {
//                throw std::invalid_argument("");
//            }
            return *m_data;
        }

        operator bool()
        {
//            if (!m_data)
//            {
//                throw std::invalid_argument("");
//            }
            return m_data!=nullptr;
        }

        ~shared_ptr()
        {
            detach();
        }

    private:
        void detach()
        {
            --m_control->m_useCount;
            //m_control->m_useCount = max(0L, m_control->m_useCount);

            if (0 == m_control->m_useCount)
            {
                m_deleter(m_data);
                m_data = nullptr;
                delete m_control;
            }
        }

        void attach()
        {
            m_control->m_useCount++;
        }


        T* m_data;
        ControlBlock* m_control;
        Deleter m_deleter;
    };
}

#endif // SHARED_PTR_H
