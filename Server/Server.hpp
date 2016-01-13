#pragma once

#include <boost/asio.hpp>

namespace Server
{
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Server
{
public:
    Server( short port );
    void Run();

private:
    void AcceptIncoming();

    boost::asio::io_service ioService;
    boost::asio::ip::tcp::acceptor acceptor;
    boost::asio::ip::tcp::socket incomingConnectionSocket;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace Server