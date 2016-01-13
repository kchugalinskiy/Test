#include "Logger.hpp"
#include <mutex>
#include <sstream>
#include <Windows.h>
#include <boost/chrono.hpp>
#include <fstream>

namespace Logger
{
//////////////////////////////////////////////////////////////////////////////
static std::mutex loggerMutex;

namespace
{

//////////////////////////////////////////////////////////////////////////////
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
    FileLogger();
    FileLogger(const FileLogger&);
    FileLogger(FileLogger&&);
};
//////////////////////////////////////////////////////////////////////////////

} // namespace
static std::string VSConsoleFormatter(
    ELogLevel logLevel,
    const char *file,
    const char *,
    int line,
    boost::chrono::system_clock::time_point &currentTime,
    const std::string &message )
{
    std::ostringstream resultMessage;
    resultMessage << file << "(" << line << "): " << currentTime << " : " << message;
    return std::forward<std::string>(resultMessage.str());
}

static std::string FileFormatter(
    ELogLevel logLevel,
    const char *file,
    const char *,
    int line,
    boost::chrono::system_clock::time_point &currentTime,
    const std::string &message )
{
    std::ostringstream resultMessage;
    resultMessage << currentTime << file << ":" << line << " - " << message;
    return std::forward<std::string>(resultMessage.str());
}
//////////////////////////////////////////////////////////////////////////////
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
    if ( logFile.is_open() )
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

}
//////////////////////////////////////////////////////////////////////////////
void LogMessage(
    ELogLevel logLevel,
    const char *file,
    const char *function,
    int line,
    const std::string &message )
{
    boost::chrono::system_clock::time_point currentTime
        = boost::chrono::system_clock::now();

    const std::string &consoleMessage
        = VSConsoleFormatter( logLevel,
                              file,
                              function,
                              line,
                              currentTime,
                              message);
    const std::string &fileMessage
        = FileFormatter( logLevel,
                         file,
                         function,
                         line,
                         currentTime,
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

    OutputDebugStringA( consoleMessage.c_str() );
}
//////////////////////////////////////////////////////////////////////////////
// LoggerConfig
LoggerConfig::LoggerConfig( const std::string &logPath )
    : loggerInstance( new FileLogger(logPath) )
{
}
//////////////////////////////////////////////////////////////////////////////
} // namespace Logger
