#include "Client.hpp"

#include "Logger/Logger.hpp"
#include <boost/lexical_cast.hpp>
#include <random>
#include <iostream>

namespace Client
{
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Client::Client(const std::string &hostString, short serverPort, int uptime)
    : serverSocket(ioService)
    , resolver(ioService)
    , address(boost::asio::ip::address::from_string(hostString))
    , port(serverPort)
    , uptimeMs(std::chrono::milliseconds(uptime))
{
    LOG_INFO("Client started");
}

Client::~Client()
{
    if (!ioService.stopped())
    {
        LOG_INFO("Service stopped");
        ioService.stop();
    }
}

void Client::Start()
{
    boost::asio::connect( serverSocket, resolver.resolve(boost::asio::ip::tcp::endpoint(address, port)) );
    LOG_INFO("Service started");

    std::random_device randomDevice;
    std::mt19937 generator(randomDevice());
    std::uniform_int_distribution<> distribution(0, 1023);
    std::chrono::system_clock::time_point clientStart = std::chrono::high_resolution_clock::now();

    std::chrono::milliseconds currentProgramUptimeMs;
    int numberOfRequests = 0;
    do
    {
        ++numberOfRequests;
        std::chrono::system_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
        currentProgramUptimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - clientStart);
        SendNumber( distribution(generator) );
        double result = ReceiveSum();
        LOG_INFO(std::string("Got response from server:") + std::to_string(result));
    } while (uptimeMs >= currentProgramUptimeMs);
    LOG_INFO(std::string("Time is gone! Requests = ") + std::to_string(numberOfRequests));
}

void Client::SendNumber( int request )
{
    boost::asio::write( serverSocket, boost::asio::buffer(&request, sizeof(request)) );
}

double Client::ReceiveSum()
{
    double reply;
    size_t reply_length = boost::asio::read( serverSocket, boost::asio::buffer(&reply, sizeof(reply)) );
    if ( sizeof(reply) != reply_length )
    {
        throw std::logic_error( "Data read error!" );
    }
    return reply;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
