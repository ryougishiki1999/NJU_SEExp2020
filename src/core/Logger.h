#ifndef CORE_LOGGER_H
#define CORE_LOGGER_H
/*! \brief Logger

    Safely output the formatted log
*/
class Logger
{
  public:
    enum LogLevel
    {
        Error,
        Warning,
        Info,
        Debug
    };

  public:
    /*! \brief print log
        \param[in] func the function name
        \param[in] line the line number
        \param[in] lv log level
        \param[in] fmt format, same as printf
    */
    static void log(const char *func, unsigned int line, LogLevel lv, const char *fmt, ...);

  public:
    static unsigned int filter_log_level; //! if log's level greater than filter_log_level, it won't be print
};

// __FUNCSIG__ IS NOT IN STANDARD, IT WORKS IN MSVC
#define ERROR(...) Logger::log(__FUNCTION__, __LINE__, Logger::Error, __VA_ARGS__)
#define WARNING(...) Logger::log(__FUNCTION__, __LINE__, Logger::Warning, __VA_ARGS__)
#define INFO(...) Logger::log(__FUNCTION__, __LINE__, Logger::Info, __VA_ARGS__)
#define DEBUG(...) Logger::log(__FUNCTION__, __LINE__, Logger::Debug, __VA_ARGS__)
#endif