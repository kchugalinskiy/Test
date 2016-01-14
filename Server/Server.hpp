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
    // actually serialization path and delay must be read from config file
    Server(short port, const std::string &binaryFilePath, std::chrono::milliseconds serializerDelay);
    void Run();
    ~Server();

private:
    // disallowed functions
    Server() = delete;
    Server(const Server&) = delete;
    Server(Server&&) = delete;
    void operator=(const Server&) = delete;

    // New incoming connection acceptor function
    void AcceptIncoming();

    boost::asio::io_service ioService;
    boost::asio::ip::tcp::acceptor acceptor;
    boost::asio::ip::tcp::socket incomingConnectionSocket;
    RequestProcessor requestProcessor;
    std::vector<std::thread> clientSessions;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace Server