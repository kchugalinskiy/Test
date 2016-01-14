#pragma once

#include <boost/asio.hpp>

namespace Server
{
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class RequestProcessor;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Session : public std::enable_shared_from_this<Session>
{
public:
    explicit Session(boost::asio::ip::tcp::socket socket);

    void Start( RequestProcessor *processor );

private:
    Session() = delete;
    Session(const Session&) = delete;
    Session(Session&&) = delete;
    void operator=(const Session&) = delete;

    void ReadNextNumber();
    void WriteResponse();

    boost::asio::ip::tcp::socket clientSocket;
    int inputData;
    double outputData;
    RequestProcessor *requestProcessor;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace Server