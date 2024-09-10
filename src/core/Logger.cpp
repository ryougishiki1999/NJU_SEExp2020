#include "Logger.h"
#include <cstdarg>
#include <cstdio>
#include <sstream>
#include <string>

unsigned int Logger::filter_log_level = Logger::Debug;

void Logger::log(const char *func, unsigned int line, LogLevel lv, const char *fmt, ...)
{
    if (lv > filter_log_level)
        return;
    va_list va;
    va_start(va, fmt);
    std::ostringstream format;
    switch (lv)
    {
    case Error:
        format << "[Error]";
        break;
    case Warning:
        format << "[Warning]";
        break;
    case Info:
        format << "[Info]";
        break;
    case Debug:
        format << "[Debug]";
        break;
    }
    format << '[' << func << ':' << line << "] " << fmt;
    vprintf(format.str().c_str(), va);
    fflush(stdout);
}