#include "Client.hpp"

#include "Logger/Logger.hpp"
#include <boost/lexical_cast.hpp>

namespace Client
{
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Client::Client( const std::string &hostString, const std::string &portString )
    : serverSocket(ioService)
    , resolver(ioService)
    , address(boost::asio::ip::address::from_string(hostString))
    , port(boost::lexical_cast<short>( portString ))
{
    LOG_INFO("Client started");
}

void Client::Start()
{
    boost::asio::connect( serverSocket, resolver.resolve(boost::asio::ip::tcp::endpoint(address, port)) );

    int checkResult = 0;
    for ( int i = 0 ; i != 1023 ; ++i )
    {
        checkResult += i*i;
        SendNumber( i );
        int result = ReceiveSum();
        if ( result != checkResult / (i + 1) )
        {
            LOG_ERROR("WRONG RESULT! Expected" + std::to_string(checkResult) + " actual value is " << result );
        }
    }
}

void Client::SendNumber( int request )
{
    boost::asio::write( serverSocket, boost::asio::buffer(&request, sizeof(request)) );
}

int Client::ReceiveSum()
{
    int reply;
    size_t reply_length = boost::asio::read( serverSocket, boost::asio::buffer(&reply, sizeof(reply)) );
    if ( sizeof(reply) != reply_length )
    {
        throw std::logic_error( "Data read error!" );
    }
    return reply;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
