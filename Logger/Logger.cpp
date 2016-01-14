#include "Logger.hpp"

#include <boost/format.hpp>
#include <cstdio>
#include <fstream>
#include <mutex>
#include <sstream>
#include <Windows.h>

namespace Logger
{
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static std::mutex loggerMutex;

namespace
{

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class FileLogger : public ILogger
{
public:
    // The only allowed constructor. Class supports RAII
    explicit FileLogger( const std::string &logPath );
    ~FileLogger();

    // Non-thread safe functions! Mutex should be locked outside
    static FileLogger *GetInstance();
    void WriteToFile( const std::string &message );

private:
    static FileLogger *fileloggerInstance;
    std::ofstream logFile;

    // Disallowed ctors
    FileLogger() = delete;
    FileLogger(const FileLogger&) = delete;
    FileLogger(FileLogger&&) = delete;
    void operator=(const FileLogger&) = delete;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} // namespace
static std::string VSConsoleFormatter(
    ELogLevel logLevel,
    const char *file,
    const char *function,
    int line,
    const std::string &message )
{
    std::string resultMessage ((boost::format("%s(%i): in function \"%s\": %s\n") % file % line % function % message).str());
    return std::move( resultMessage );
}

static std::string FileFormatter(
    ELogLevel logLevel,
    const char *file,
    const char *,
    int line,
    const std::string &message )
{
    std::string resultMessage ((boost::format("%s:%i : %s\n") % file % line % message).str());
    return std::move(resultMessage);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FileLogger::FileLogger( const std::string &logPath )
{
    std::lock_guard<std::mutex> loggerLock( loggerMutex );
    if ( nullptr == fileloggerInstance )
    {
        fileloggerInstance = this;
    }
    else
    {
        // Some more adequate actions required including pre-logger
        throw std::logic_error("Trying to create logger twice. Why?");
    }

    logFile.open( logPath );
    if ( !logFile.is_open() )
    {
        throw std::logic_error("File open error");
    }
}

FileLogger::~FileLogger()
{
    std::lock_guard<std::mutex> loggerLock( loggerMutex );
    if ( nullptr == fileloggerInstance )
    {
        // Some more adequate checks required. Didn't you get an exception
        // like this before? ;-)
        throw std::logic_error("Trying to unregister logger twice. Why?");
    }
    else
    {
        fileloggerInstance = nullptr;
    }
}

// static
FileLogger *FileLogger::GetInstance()
{
    return fileloggerInstance;
}

void FileLogger::WriteToFile( const std::string &message )
{
    if ( logFile.is_open() && logFile.good() )
    {
        logFile << message;
        logFile.flush();
    }
}

FileLogger *FileLogger::fileloggerInstance = nullptr;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void LogMessage(
    ELogLevel logLevel,
    const char *file,
    const char *function,
    int line,
    const std::string &message )
{
    const std::string &consoleMessage
        = VSConsoleFormatter( logLevel,
                              file,
                              function,
                              line,
                              message);
    const std::string &fileMessage
        = FileFormatter( logLevel,
                         file,
                         function,
                         line,
                         message);

    std::lock_guard<std::mutex> loggerLock( loggerMutex );

    if ( nullptr != FileLogger::GetInstance() )
    {
        FileLogger::GetInstance()->WriteToFile( fileMessage );
    }
    else
    {
        OutputDebugStringA("Cannot write to file - instance already"
                           "released or not acquired yet");
    }

    printf("%s", consoleMessage.c_str());
#ifdef _DEBUG
    OutputDebugStringA( consoleMessage.c_str() );
#endif
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LoggerConfig
LoggerConfig::LoggerConfig( const std::string &logPath )
    : loggerInstance( new FileLogger(logPath) )
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace Logger
