#include "Client.hpp"
#include "Logger/Logger.hpp"

int main(int argc, const char *argv[])
{
    Logger::LoggerConfig logConfig( "D:\\Test\\client.txt" );
    LOG_INFO("Starting client...");

    try
    {
        if (argc != 3)
        {
            throw std::logic_error( "Usage: blocking_tcp_echo_client <host> <port>" );
        }

        Client::Client client(argv[1], argv[2]);
        client.Start();
    }
    catch (std::exception& e)
    {
        LOG_ERROR( e.what() );
    }

    LOG_INFO("Shutting client down...");
    return 0;
}
