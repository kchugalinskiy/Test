#include "SqrAverageContainer.hpp"

#include <limits>
#include "Logger/Logger.hpp"

namespace Server
{
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ISqrAverageAlgorithm::~ISqrAverageAlgorithm()
{}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const int maximumAcceptedValue = 1023;
static const int minimumAcceptedValue = 0;

static SqrAverageAlgorithmConfig *algorithmConfig = nullptr;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SqrAverageAlgorithmConfig::SqrAverageAlgorithmConfig(ISqrAverageAlgorithm *instance)
{
    algorithm.reset(instance);
    algorithmConfig = this;
}

SqrAverageAlgorithmConfig::~SqrAverageAlgorithmConfig()
{
    algorithmConfig = nullptr;
}

// static
ISqrAverageAlgorithm *SqrAverageAlgorithmConfig::GetSqrAverageAlgorithm()
{
    SqrAverageAlgorithmConfig *instance = GetInstance();
    if (nullptr == instance)
    {
        LOG_ERROR("Square avg algo uninitialized!");
        return nullptr;
    }

    return instance->algorithm.get();
}

// static
SqrAverageAlgorithmConfig *SqrAverageAlgorithmConfig::GetInstance()
{
    return algorithmConfig;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SqrAverageBucketAlgorithm::SqrAverageBucketAlgorithm()
    : numberOfElements(0)
{
    buckets.push_back(0);
}

SqrAverageBucketAlgorithm::~SqrAverageBucketAlgorithm()
{
}

double SqrAverageBucketAlgorithm::AddNumber(int number)
{
    if (number < minimumAcceptedValue || number > maximumAcceptedValue)
    {
        LOG_ERROR("Wrong number (ignored) : " + std::to_string(number));
        return 0.0;
    }

    ++numberOfElements;
    AddNewNumberToBuckets(number);

    long double result = 0.0;
    for (auto it = buckets.begin(); it != buckets.end(); ++it)
    {
        result += ((long double)(*it) / numberOfElements);
    }

    result = sqrt(result);
    return (double)result;
}

void SqrAverageBucketAlgorithm::AddNewNumberToBuckets(int number)
{
    uint64_t lastBucketValue = buckets.back();
    if ( std::pow(10, std::numeric_limits<long double>::digits10) < (lastBucketValue + maximumAcceptedValue * maximumAcceptedValue) )
    {
        // Next number may not fit into long double without truncations
        // But the sum after division may still be fine
        // Add new bucket
        buckets.push_back(0);
    }
    buckets.back() += number*number;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace Server
