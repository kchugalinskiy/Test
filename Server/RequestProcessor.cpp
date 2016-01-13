#include "RequestProcessor.hpp"

namespace Server
{
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RequestProcessor::RequestProcessor()
    : partialSum(0)
    , numberOfElements(0)
{

}

int RequestProcessor::AddNewNumber( int number )
{
    std::unique_lock<std::mutex> processorLock(requestProcessorMutex);
    ++numberOfElements;
    partialSum += number * number;
    return (partialSum / numberOfElements);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace Server