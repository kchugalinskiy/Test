#include "Logger/Logger.hpp"
#include "Server.hpp"
#include <boost/program_options.hpp>

int main(int argc, const char *argv[])
{
    short listenPort = 12345;
    std::string serializationPath = "C:\\serverSerializeDefault.txt";
    int serializationDelay = 1000;

    boost::program_options::options_description optionDesc("General options");
    optionDesc.add_options()
        ("help,h", "Show help")
        ("listen_port,l", boost::program_options::value<short>(&listenPort), "Select server listen port")
        ("serialization_path,p", boost::program_options::value<std::string>(&serializationPath), "Select server serialization path")
        ("serialization_delay,d", boost::program_options::value<int>(&serializationDelay), "Select server serialization delay");

    boost::program_options::variables_map variablesMap;
    boost::program_options::parsed_options parsed = boost::program_options::command_line_parser(argc, argv).options(optionDesc).allow_unregistered().run();
    boost::program_options::store(parsed, variablesMap);
    boost::program_options::notify(variablesMap);

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