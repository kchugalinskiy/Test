#pragma once

#include <cstdint>
#include "Logger/Logger.hpp"
#include <mutex>

namespace Server
{
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class RequestProcessor
{
public:
    RequestProcessor();
    // Add new number and return current sum
    int AddNewNumber( int number );

private:
    // Disallowed
    RequestProcessor(RequestProcessor&&);
    RequestProcessor(const RequestProcessor&);
    void operator=(const RequestProcessor&);

    std::mutex requestProcessorMutex;
	uint64_t partialSum;
	int numberOfElements;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace Server
