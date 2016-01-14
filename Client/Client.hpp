#pragma once

#include <boost/asio.hpp>

namespace Client
{
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Client
{
public:
    // RAII
    Client( const std::string &host, const std::string &port );
    ~Client();

    void Start();

private:
    // Disallowed
    Client();
    Client(const Client&);
    Client(Client&&);
    void operator=(const Client&);

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