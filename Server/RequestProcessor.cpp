#include "RequestProcessor.hpp"

#include <cmath>
#include <limits>
#include "SqrAverageContainer.hpp"

namespace Server
{
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RequestProcessor::RequestProcessor(const std::string &binaryFilePath, std::chrono::milliseconds serializerDelay)
    : serializator(binaryFilePath, serializerDelay)
{
}

double RequestProcessor::ProcessInputNumber(int number)
{
    std::unique_lock<std::mutex> processorLock(requestProcessorMutex);
    AddNewNumber(number);
    ISqrAverageAlgorithm *algorithm = SqrAverageAlgorithmConfig::GetSqrAverageAlgorithm();
    if (nullptr == algorithm)
    {
        LOG_ERROR("Algorithm uninitialized");
        return 0.0;
    }
    double result = algorithm->AddNumber(number);
    LOG_INFO(std::string("Current result = ") + std::to_string(result));
    return result;
}

void RequestProcessor::AddNewNumber(int number)
{
    serializator.QueueSave(number);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace Server