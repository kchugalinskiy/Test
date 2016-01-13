#include "Client.hpp"

#include <boost/lexical_cast.hpp>

namespace Client
{
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Client::Client( const std::string &hostString, const std::string &portString )
    : serverSocket(ioService)
    , resolver(ioService)
{
    boost::asio::ip::address address = boost::asio::ip::address::from_string(hostString);
    short port = boost::lexical_cast<short>( portString );
    boost::asio::connect( serverSocket, resolver.resolve(boost::asio::ip::tcp::endpoint(address, port)) );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
