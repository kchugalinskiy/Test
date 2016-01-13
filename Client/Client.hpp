#pragma once

#include <boost/asio.hpp>

namespace Client
{
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Client
{
public:
    Client( const std::string &host, const std::string &port );

    void Start();

private:
    // Disallowed
    Client();
    Client(const Client&);
    Client(Client&&);
    void operator=(const Client&);

    void SendNumber( int request );
    int ReceiveSum();

    boost::asio::io_service ioService;
    boost::asio::ip::tcp::socket serverSocket;
    boost::asio::ip::tcp::resolver resolver;
    boost::asio::ip::address address;
    short port;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace Client