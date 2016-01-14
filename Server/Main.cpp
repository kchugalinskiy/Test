//It seems like Studio 13 doesn't feel good with boost 1_60_0 multiprecision
//#include <boost/multiprecision/cpp_int.hpp>
//#include <boost/multiprecision/cpp_dec_float.hpp> 
#include <boost/program_options.hpp>
#include "Logger/Logger.hpp"
#include "Server.hpp"
#include "SqrAverageContainer.hpp"

int main(int argc, const char *argv[])
{
    short listenPort = 12345;
    std::string serializationPath = "C:\\serverSerializeDefault.txt";
    int serializationDelay = 1000;
    std::string algorithm = "bucket";
    std::string logPath = "C:\\server.txt";

    boost::program_options::options_description optionDesc("General options");
    optionDesc.add_options()
        ("help,h", "Show help")
        ("listen_port,l", boost::program_options::value<short>(&listenPort), "Select server listen port")
        ("serialization_path,p", boost::program_options::value<std::string>(&serializationPath), "Select server serialization path")
        ("serialization_delay,d", boost::program_options::value<int>(&serializationDelay), "Select server serialization delay")
        ("log_path,c", boost::program_options::value<std::string>(&logPath), "Select log path")
        ("algorithm,a", boost::program_options::value<std::string>(&algorithm), "Select sqr avg algorithm : boost, bucket");

    boost::program_options::variables_map variablesMap;
    boost::program_options::parsed_options parsed = boost::program_options::command_line_parser(argc, argv).options(optionDesc).allow_unregistered().run();
    boost::program_options::store(parsed, variablesMap);
    boost::program_options::notify(variablesMap);

    Logger::LoggerConfig logConfig(logPath);
    LOG_INFO("Starting server...");

    try
    {
        std::unique_ptr<Server::SqrAverageAlgorithmConfig> algorithmConfig;
        if (algorithm == "boost")
        {
            LOG_ERROR("MSVC 12 died here. Bless it!");
            //It seems like Studio 13 doesn't feel good with boost 1_60_0 multiprecision
            //typedef Server::SqrAverageNaiveAlgorithm<boost::multiprecision::cpp_int, boost::multiprecision::cpp_dec_float_100> sqrBoost;
            //algorithmConfig = Server::SqrAverageAlgorithmConfig::MakeSqrAverageConfig<sqrBoost>();
        }
        else if (algorithm == "bucket")
        {
            algorithmConfig = Server::SqrAverageAlgorithmConfig::MakeSqrAverageConfig<Server::SqrAverageBucketAlgorithm>();
        }
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