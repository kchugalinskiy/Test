#include "Session.hpp"

#include "Logger/Logger.hpp"
#include "RequestProcessor.hpp"

namespace Server
{
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Session::Session(boost::asio::ip::tcp::socket socket)
    : clientSocket( std::move(socket) )
    , inputData( 0 )
    , outputData( 0.0 )
    , requestProcessor( nullptr )
{
}

void Session::Start(RequestProcessor *processor)
{
    requestProcessor = processor;
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
    clientSocket.async_read_some(boost::asio::buffer(&inputData, sizeof(inputData)),
        [this, self](boost::system::error_code ec, std::size_t length)
    {
        if ( !ec )
        {
            LOG_INFO("Number " + std::to_string(inputData) + " received");
            if (nullptr != requestProcessor)
            {
                this->outputData = this->requestProcessor->ProcessInputNumber(this->inputData);
                WriteResponse();
            }
            else
            {
                LOG_ERROR("Request processor uninitialized!");
            }
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
    boost::asio::async_write(clientSocket, boost::asio::buffer(&outputData, sizeof(outputData)),
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