#include "calldispatcher.h"

CallDispatcher::CallDispatcher()
{
    //...
}

void CallDispatcher::connect(Object *sender, const std::string& singal, Object *receiver, const std::string& slot)
{
    //...
}

void CallDispatcher::disconnect(Object *sender, const std::string& singal, Object *receiver, const std::string& slot)
{
    //...
}

void CallDispatcher::sendSignal(Object *sender, const std::string& singal)
{
    //...
}
