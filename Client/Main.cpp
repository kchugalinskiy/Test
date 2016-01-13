#include "Logger/Logger.hpp"

int main()
{
    Logger::LoggerConfig logConfig( "D:\\Test\\client.txt" );
    LOG_INFO("Starting client...");
    LOG_INFO("Shutting client down...");
    return 0;
}