#pragma once

#include <boost/asio.hpp>

namespace Client
{
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Client
{
public:
    // RAII
    Client( const std::string &host, short port );
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
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace Client