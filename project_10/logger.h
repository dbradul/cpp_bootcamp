#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>

class Logger
{
    std::string m_scope;
public:
    Logger(const std::string& scope);

    const std::string& scope() const;

    template<class T>
    void log(T msg, bool end_line=false)
    {
        std::cout << msg;
        if (end_line)
        {
            std::cout << std::endl;
        }
    }
};

Logger& getLogger();
//extern Logger gLogger;

#define LOG(msg) do {\
        getLogger().log(getLogger().scope()); \
        getLogger().log(" (" __FILE__ ": "); \
        getLogger().log(__LINE__); \
        getLogger().log("):\t"); \
        getLogger().log(msg, true); \
    } while(false)


#endif // LOGGER_H
