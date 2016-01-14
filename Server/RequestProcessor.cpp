#include "RequestProcessor.hpp"

#include <cmath>

namespace Server
{
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RequestProcessor::RequestProcessor()
: partialSqrSum(0)
    , numberOfElements(0)
{

}

double RequestProcessor::ProcessInputNumber(int number)
{
    std::unique_lock<std::mutex> processorLock(requestProcessorMutex);
    AddNewNumber(number);
    return CalculateSquareAvg();
}

void RequestProcessor::AddNewNumber(int number)
{
    if (number >= 1024)
    {
        LOG_ERROR("Number overflow : " + std::to_string(number));
        return;
    }

    ++numberOfElements;
    partialSqrSum += number * number;
}

double RequestProcessor::CalculateSquareAvg() const
{
    return double(sqrt(partialSqrSum / numberOfElements));
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace Server