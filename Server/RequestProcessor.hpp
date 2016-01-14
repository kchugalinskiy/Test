#pragma once

#include <cstdint>
#include "DataSaver.hpp"
#include "Logger/Logger.hpp"
#include <mutex>

namespace Server
{
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class RequestProcessor
{
public:
    RequestProcessor(const std::string &binaryFilePath, std::chrono::milliseconds serializerDelay);
    // Process number somehow
    double ProcessInputNumber(int number);

private:
    // Not thread-safe, returns sum
    void AddNewNumber(int number);
    double CalculateSquareAvg() const;

    // Disallowed
    RequestProcessor() = delete;
    RequestProcessor(RequestProcessor&&) = delete;
    RequestProcessor(const RequestProcessor&) = delete;
    void operator=(const RequestProcessor&) = delete;

    std::mutex requestProcessorMutex;
    uint64_t partialSqrSum;
    int numberOfElements;
    DataSaver serializator;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace Server
