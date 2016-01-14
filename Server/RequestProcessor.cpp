#include "RequestProcessor.hpp"

#include <cmath>
#include <limits>

namespace Server
{
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RequestProcessor::RequestProcessor(const std::string &binaryFilePath, std::chrono::milliseconds serializerDelay)
    : partialSqrSum(0)
    , numberOfElements(0)
    , serializator(binaryFilePath, serializerDelay)
{
}

double RequestProcessor::ProcessInputNumber(int number)
{
    std::unique_lock<std::mutex> processorLock(requestProcessorMutex);
    AddNewNumber(number);
    double result = CalculateSquareAvg();
    LOG_INFO(std::string("Current result = ") + std::to_string(result));
    return result;
}

void RequestProcessor::AddNewNumber(int number)
{
    serializator.QueueSave(number);
    static const int maximumAcceptedValue = 1023;
    if (number < 0 || number >= maximumAcceptedValue)
    {
        LOG_ERROR("Wrong number (ignored) : " + std::to_string(number));
        return;
    }

    ++numberOfElements;
    partialSqrSum += number * number;
    if (std::numeric_limits<uint64_t>::max() < 2 * maximumAcceptedValue + partialSqrSum)
    {
        LOG_ERROR("Overflow is nearby! Change algorithm :-)");
    }
}

double RequestProcessor::CalculateSquareAvg() const
{
    // Maximum number of elements : 2^54 - 2^64
    // Maximum square sum : 2^64
    // (Maximum square sum) div (Maximum number of elements) : 0 - 2^10
    // (Maximum square sum) mod (Maximum number of elements) : 0 - 2^64
    double div = (double)(partialSqrSum / numberOfElements); // casted value is 1024 maximum - fine for double
    double mod = (double)(partialSqrSum % numberOfElements) / numberOfElements; // casted value is 1023 maximum, fine for double
    double divided = div + mod;
    return sqrt(1.0 * divided);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace Server