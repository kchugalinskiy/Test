#include "Session.hpp"

#include "Logger/Logger.hpp"

namespace Server
{
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Session::Session(boost::asio::ip::tcp::socket socket)
    : clientSocket(std::move(socket))
{
}

void Session::Start()
{
    ReadNextNumber();
}

void Session::ReadNextNumber()
{
    if ( !clientSocket.is_open() )
    {
        LOG_INFO("Session is done");
        return;
    }
    auto self(shared_from_this());
    clientSocket.async_read_some(boost::asio::buffer(&data, sizeof(data)),
        [this, self](boost::system::error_code ec, std::size_t length)
    {
        if ( !ec )
        {
            LOG_INFO( "Number " + std::to_string(data) + " received" );
            WriteResponse();
        }
        else
        {
            LOG_ERROR( ec.message() );
        }
    });
}

void Session::WriteResponse()
{
    auto self(shared_from_this());
    boost::asio::async_write(clientSocket, boost::asio::buffer(&data, sizeof(data)),
        [this, self](boost::system::error_code ec, std::size_t /*length*/)
    {
        if ( !ec )
        {
            ReadNextNumber();
        }
        else
        {
            LOG_ERROR( ec.message() );
        }
    });
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace Server