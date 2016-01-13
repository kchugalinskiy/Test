#include "Logger/Logger.hpp"
#include "Server.hpp"
#include <boost/lexical_cast.hpp>

int main(int argc, const char *argv[])
{
    Logger::LoggerConfig logConfig( "D:\\Test\\server.txt" );
    LOG_INFO("Starting server...");

    try
    {
        if (argc != 2)
        {
            throw std::logic_error("Missing or too many command line arguments! Expected usage server <listen port>");
        }

        short port = boost::lexical_cast<short>( std::string(argv[1]) );
        Server::Server server(port);
        server.Run();
    }
    catch ( const boost::bad_lexical_cast &badInput )
    {
        LOG_ERROR( "Bad command line argument! Expected server <listen port>" );
        LOG_ERROR( badInput.what() );
    }
    catch ( const std::exception& e )
    {
        LOG_ERROR( e.what() );
    }

    LOG_INFO("Shutting server down...");
    return 0;
}