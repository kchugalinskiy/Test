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
    // Process number somehow
    double ProcessInputNumber(int number);

private:
    // Not thread-safe, returns sum
    void AddNewNumber(int number);
    double CalculateSquareAvg() const;

    // Disallowed
    RequestProcessor(RequestProcessor&&) = delete;
    RequestProcessor(const RequestProcessor&) = delete;
    void operator=(const RequestProcessor&) = delete;

    std::mutex requestProcessorMutex;
    uint64_t partialSqrSum;
    int numberOfElements;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace Server
