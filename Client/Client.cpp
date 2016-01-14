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

Client::~Client()
{
    if (!ioService.stopped())
    {
        LOG_INFO("Service stopped");
        ioService.stop();
    }
}

void Client::Start()
{
    boost::asio::connect( serverSocket, resolver.resolve(boost::asio::ip::tcp::endpoint(address, port)) );
    LOG_INFO("Service started");

    for ( int i = 0 ; i != 1024 ; ++i )
    {
        SendNumber( i );
        double result = ReceiveSum();
        LOG_INFO(std::string("Got response from server:") + std::to_string(result));
    }
}

void Client::SendNumber( int request )
{
    boost::asio::write( serverSocket, boost::asio::buffer(&request, sizeof(request)) );
}

double Client::ReceiveSum()
{
    double reply;
    size_t reply_length = boost::asio::read( serverSocket, boost::asio::buffer(&reply, sizeof(reply)) );
    if ( sizeof(reply) != reply_length )
    {
        throw std::logic_error( "Data read error!" );
    }
    return reply;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
