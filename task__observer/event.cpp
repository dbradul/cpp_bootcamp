#include "event.h"

Event::Event ()
    : Event("N/A")
{
}

Event::Event (const std::string& description)
    : m_description(description)
{
}

const std::string& Event::getDescription() const
{
    return m_description;
}

