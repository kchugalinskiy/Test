#include <boost/program_options.hpp>
#include "Client.hpp"
#include "Logger/Logger.hpp"

int main(int argc, const char *argv[])
{
    Logger::LoggerConfig logConfig("D:\\Test\\client.txt");
    LOG_INFO("Starting client...");

    try
    {
        std::string host = "127.0.0.1";
        short port = 12345;
        int clientUptime = 1000;

        boost::program_options::options_description optionDesc("General options");
        optionDesc.add_options()
            ("help,h", "Show help")
            ("port,p", boost::program_options::value<short>(&port), "Select port")
            ("server,s", boost::program_options::value<std::string>(&host), "Select host")
            ("uptime,u", boost::program_options::value<int>(&clientUptime), "Select desired client uptime in milliseconds");

        boost::program_options::variables_map variablesMap;
        boost::program_options::parsed_options parsed = boost::program_options::command_line_parser(argc, argv).options(optionDesc).allow_unregistered().run();
        boost::program_options::store(parsed, variablesMap);
        boost::program_options::notify(variablesMap);

        Client::Client client(host, port, clientUptime);
        client.Start();
    }
    catch (std::exception& e)
    {
        LOG_ERROR( e.what() );
    }

    LOG_INFO("Shutting client down...");
    return 0;
}
