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
    return sqrt(1.0 * partialSqrSum / numberOfElements);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace Server