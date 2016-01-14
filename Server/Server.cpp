#include "Server.hpp"

#include "Logger/Logger.hpp"
#include "Session.hpp"

namespace Server
{
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Server::Server( short port )
    : ioService()
    , acceptor(ioService, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port) )
    , incomingConnectionSocket(ioService)
{
    LOG_INFO("Server started");
    LOG_INFO("Waiting for incoming connection(s)");
    AcceptIncoming();
}

void Server::Run()
{
    ioService.run();
}

Server::~Server()
{

}

void Server::AcceptIncoming(  )
{
    acceptor.async_accept(incomingConnectionSocket,
        [this](boost::system::error_code errorCode)
        {
            if (!errorCode)
            {
                LOG_INFO("Starting new client session");
				std::make_shared<Session>(std::move(incomingConnectionSocket))->Start(&requestProcessor);
            }

            AcceptIncoming();
        });
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace Server