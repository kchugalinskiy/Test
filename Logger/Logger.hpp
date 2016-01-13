#pragma once

#include <string>

#define LOG_ERROR(msg) LogMessage(eLogLevelError, msg)
#define LOG_WARNING(msg) LogMessage(eLogLevelWarning, msg)
#define LOG_INFO(msg) LogMessage(eLogLevelInfo, msg)

namespace Logger
{

enum ELogLevel
{
	eLogLevelError = 0,
	eLogLevelWarning = 1,
	eLogLevelInfo = 2
};
//////////////////////////////////////////////////////////////////////////////
class Logger
{
public:
	static void LogMessage( ELogLevel logLevel, const std::string &message );

private:
	// The only allowed constructor
	Logger( const std::string &logPath );
	// Disabled
	Logger();
	Logger(const Logger&);
	Logger(Logger&&);
};
//////////////////////////////////////////////////////////////////////////////
} // namespace Logger