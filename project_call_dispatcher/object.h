#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include "calldispatcher.h"

class Object
{
public:
    Object(CallDispatcher::Ptr& dispatcher)
        : m_dispatcher(dispatcher)
    {
    }

    virtual void dispatchMethod(const std::string& methodName){}

protected:
    CallDispatcher::Ptr m_dispatcher;
};


#endif // OBJECT_H
