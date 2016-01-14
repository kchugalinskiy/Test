#include "Logger/Logger.hpp"
#include "Server.hpp"
#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>

int main(int argc, const char *argv[])
{
    short listenPort;
    std::string serializationPath;
    int serializationDelay;

    boost::program_options::options_description optionDesc("General options");
    optionDesc.add_options()
        ("help,h", "Show help")
        ("listen_port,l", boost::program_options::value<short>(&listenPort), "Select server listen port")
        ("serialization_path,p", boost::program_options::value<std::string>(&serializationPath), "Select server serialization path")
        ("serialization_delay,d", boost::program_options::value<int>(&serializationDelay), "Select server serialization delay")
        ;

    Logger::LoggerConfig logConfig( "D:\\Test\\server.txt" );
    LOG_INFO("Starting server...");

    try
    {
        std::chrono::milliseconds serializationDelayMs = std::chrono::milliseconds(serializationDelay);
        Server::Server server(listenPort, serializationPath, serializationDelayMs);
        server.Run();
    }
    catch ( const std::exception& e )
    {
        LOG_ERROR( e.what() );
    }

    LOG_INFO("Shutting server down...");
    return 0;
}