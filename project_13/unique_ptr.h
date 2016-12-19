#ifndef UNIQUE_PTR_H
#define UNIQUE_PTR_H

#include <exception>
#include <stdexcept>
#include <iostream>
#include <memory>

namespace my
{
    template<class T>
    struct default_deleter
    {
        void operator()(T* t)
        {
            std::cout << "...regular delete..." << std::endl;
            delete t;
        }
    };

    template<class T>
    struct default_deleter<T[]>
    {
        void operator()(T* t)
        {
            std::cout << "...array delete[]..." << std::endl;
            delete[] t;
        }
    };


    template <typename T, typename Deleter=default_deleter<T>>
    class unique_ptr
    {
    public:
        unique_ptr(T* value):
            m_data(value)
        {}

        unique_ptr():
            m_data(nullptr)
        {}

        unique_ptr(unique_ptr&& rhs):
            m_data(nullptr)
        {
//            m_data(std::move(rhs.m_data));
//            rhs.m_data = nullptr;
            swap(rhs);
        }

        unique_ptr& operator =(unique_ptr&& rhs)
        {
            unique_ptr(std::move(rhs)).swap(*this);
            return *this;
        }

        void swap(unique_ptr& to)
        {
            std::swap(to.m_data,    m_data);
            std::swap(to.m_deleter, m_deleter);

//            {
//                T* tmp = m_data;
//                m_data = to.m_data;
//                to.m_data = tmp;
//            }

//            {
//                Deleter tmp = m_deleler;
//                m_deleler = to.m_deleler;
//                to.m_deleler = tmp;
//            }

        }

        ~unique_ptr()
        {
            cleanup();
        }


        void cleanup()
        {
            if (m_data)
            {
                m_deleter(m_data);
            }
        }


        T* get()
        {
            return m_data;
        }

        T* operator->() const
        {
            return m_data;
        }

//        operator T*()
//        {
//            return m_data;
//        }

        const T& operator*() const
        {
            return *m_data;
        }


        T& operator*()
        {
            return *m_data;
        }

        T* release()
        {
            T* tmp = m_data;
            m_data = nullptr;
            return tmp;
        }

        void reset(T* ptr)
        {
            cleanup();
            m_data = ptr;
        }

        inline bool operator>(const unique_ptr& rhs) const
        {
            return m_data > rhs.m_data;
        }

        inline bool operator<(const unique_ptr& rhs) const
        {
            return rhs.operator>(*this);
        }

        inline bool operator==(const unique_ptr& rhs) const
        {
            return !operator<(rhs) && !operator>(rhs);
        }

        inline bool operator!=(const unique_ptr& rhs) const
        {
            return !operator==(rhs);
        }

        inline bool operator>=(const unique_ptr& rhs) const
        {
            return operator==(rhs) || operator>(rhs);
        }

        inline bool operator<=(const unique_ptr& rhs) const
        {
            return operator==(rhs) || operator<(rhs);
        }


        unique_ptr(const unique_ptr& rhs) = delete;
        unique_ptr& operator =(const unique_ptr& rhs) = delete;

    private:
        T* m_data;
        Deleter     m_deleter;
    };

}

#endif // UNIQUE_PTR_H
