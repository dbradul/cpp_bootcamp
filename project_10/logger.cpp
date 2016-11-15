#include <string>
#include "logger.h"

using namespace std;

Logger& getLogger()
{
    static Logger* logger = new Logger("Project_10");
    return *logger;
}

//Logger gLogger = Logger("Project_10");

Logger::Logger(const string& scope)
    : m_scope(scope)
{

}

const string& Logger::scope() const
{
    return m_scope;
}
