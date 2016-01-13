#include "Logger/Logger.hpp"

int main()
{
    Logger::LoggerConfig logConfig( "D:\\Test\\server.txt" );
    LOG_INFO("Starting server...");
    LOG_INFO("Shutting server down...");
    return 0;
}