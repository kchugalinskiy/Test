#pragma once

#include <string>
#include <memory>

#define LOG_ERROR(msg) Logger::LogMessage(Logger::eLogLevelError, __FILE__, __FUNCTION__, __LINE__, msg)
#define LOG_WARNING(msg) Logger::LogMessage(Logger::eLogLevelWarning, __FILE__, __FUNCTION__, __LINE__, msg)
#define LOG_INFO(msg) Logger::LogMessage(Logger::eLogLevelInfo, __FILE__, __FUNCTION__, __LINE__, msg)

namespace Logger
{

enum ELogLevel
{
    eLogLevelError = 0,
    eLogLevelWarning = 1,
    eLogLevelInfo = 2
};

void LogMessage(
    ELogLevel logLevel,
    const char *file,
    const char *function,
    int line,
    const std::string &message );

class ILogger
{

};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class LoggerConfig
{
public:
    // Supports RAII, should be called once
    explicit LoggerConfig( const std::string &logPath );

private:
    std::unique_ptr<ILogger> loggerInstance;

    // Disallowed
    LoggerConfig() = delete;
    LoggerConfig(const LoggerConfig&) = delete;
    LoggerConfig(LoggerConfig&&) = delete;
    void operator=(const LoggerConfig&) = delete;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace Logger
