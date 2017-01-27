#include "eventmanager.h"
#include <algorithm>

using namespace std;

EventManager& EventManager::getInstance()
{
    static EventManager inst;
    return inst;
}

void EventManager::publishEvent(const Event& ev)
{
    //...
}

void EventManager::addListener(shared_ptr<EventListener>& listener)
{
    //...
}

void EventManager::removeListener(shared_ptr<EventListener>& listener)
{
    //...
}

