#pragma once

#include <boost/asio.hpp>
#include "RequestProcessor.hpp"
#include <thread>
#include <vector>

namespace Server
{
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Server
{
public:
    Server( short port );
    void Run();
	~Server();

private:
    void AcceptIncoming();

    boost::asio::io_service ioService;
    boost::asio::ip::tcp::acceptor acceptor;
    boost::asio::ip::tcp::socket incomingConnectionSocket;
	RequestProcessor requestProcessor;
	std::vector<std::thread> clientSessions;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace Server