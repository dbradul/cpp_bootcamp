#ifndef CALLDISPATCHER_H
#define CALLDISPATCHER_H

#include <string>
#include <memory>

class Object;

class CallDispatcher
{
private:

    //...

public:
    typedef std::shared_ptr<CallDispatcher> Ptr;

    CallDispatcher();

    void connect(Object* sender, const std::string& singal, Object* receiver, const std::string& slot);
    void disconnect(Object* sender, const std::string& singal, Object* receiver, const std::string& slot);

    void sendSignal(Object* sender, const std::string& singal);
};

#endif // CALLDISPATCHER_H
