#ifndef FORWARD_LIST_H
#define FORWARD_LIST_H

#include <iostream>

namespace my {


template<typename T>
struct fwd_list_Node
{
    T data;
    fwd_list_Node<T>* next;
};


template<typename T, typename RetT=T>
struct fwd_list_iterator : public std::iterator<std::forward_iterator_tag, T>
{
    fwd_list_iterator()
        : m_current (nullptr)
    {
    }

    fwd_list_iterator(my::fwd_list_Node<T>* ptr)
        : m_current (ptr)
    {
    }

    RetT& operator*()
    {
        return m_current->data;
    }

    fwd_list_iterator& operator++()
    {
        m_current = m_current->next;
        return *this;
    }

    fwd_list_iterator operator++(int)
    {
        fwd_list_iterator tmp(m_current);
        ++(*this);
        return tmp;
    }

    bool operator==(const fwd_list_iterator& rhs)
    {
        return m_current==rhs.m_current;
    }

    bool operator!=(const fwd_list_iterator& rhs)
    {
        return !(*this==rhs);
    }

private:
        fwd_list_Node<T>* m_current;
};


template<class T>
class forward_list
{
    fwd_list_Node<T>* head;

public:

    typedef fwd_list_iterator<T> iterator;
    typedef fwd_list_iterator<T, const T> const_iterator;

    forward_list()
        : head(nullptr)
    {
    }

    forward_list(const forward_list& rhs)
        : head(nullptr)
    {
        for(const T& elem : rhs)
        {
            push_front(elem);
        }

        reverse();
    }

    forward_list(forward_list&& rhs)
        : head(nullptr)
    {
        rhs.swap(*this);
    }

    forward_list& operator=(const forward_list& rhs)
    {
        forward_list(rhs).swap(*this);
        return *this;
    }

    forward_list& operator=(forward_list&& rhs)
    {
        forward_list(std::move(rhs)).swap(*this);
        return *this;
    }

    void swap(forward_list& rhs)
    {
        std::swap(rhs.head, head);
    }

    const T& front()
    {
        return head->data;
    }

    T pop_front()
    {
        T data = head->data;
        fwd_list_Node<T>* tmp = head->next;
        delete head;
        head = tmp;
        return data;
    }

    bool empty()
    {
        return head==nullptr;
    }

    void push_front(const T& value)
    {
        fwd_list_Node<T>* tmp = new fwd_list_Node<T>{value, nullptr};
        if (head)
        {
            tmp->next = head;
        }
        head = tmp;
    }

    void push_front(T&& value)
    {
        fwd_list_Node<T>* tmp = new fwd_list_Node<T>{std::move(value), nullptr};
        if (head)
        {
            tmp->next = head;
        }
        head = tmp;
    }

    void reverse()
    {
        // ...
    }

    iterator begin()
    {
        return iterator(head);
    }

    iterator end()
    {
        return iterator(nullptr); // replace with sentinal object?
    }

    const_iterator begin() const
    {
        return const_iterator(head);
    }

    const_iterator end() const
    {
        return const_iterator(nullptr);
    }

    const_iterator cbegin() const
    {
        return const_iterator(head);
    }

    const_iterator cend() const
    {
        return const_iterator(nullptr);
    }

    ~forward_list()
    {
        while(head)
        {
            fwd_list_Node<T>* tmp = head;
            head = head->next;
            delete tmp;
        }
    }

};

}

#endif // FORWARD_LIST_H
