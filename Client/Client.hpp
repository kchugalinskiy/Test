#pragma once

#include <boost/asio.hpp>
#include <chrono>

namespace Client
{
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Client
{
public:
    // RAII
    Client(const std::string &host, short port, int uptimeMs);
    ~Client();

    void Start();

private:
    // Disallowed
    Client() = delete;
    Client(const Client&) = delete;
    Client(Client&&) = delete;
    void operator=(const Client&) = delete;

    void SendNumber( int request );
    double ReceiveSum();

    boost::asio::io_service ioService;
    boost::asio::ip::tcp::socket serverSocket;
    boost::asio::ip::tcp::resolver resolver;
    boost::asio::ip::address address;
    short port;
    std::chrono::milliseconds uptimeMs;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace Client