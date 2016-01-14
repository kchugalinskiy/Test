#pragma once

#include <memory>
#include <vector>

namespace Server
{
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ISqrAverageAlgorithm
{
public:
    virtual ~ISqrAverageAlgorithm();
    virtual double AddNumber(int number) = 0;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SqrAverageAlgorithmConfig
{
public:
    template<class TSqrAverageAlgorithm>
    static std::unique_ptr<SqrAverageAlgorithmConfig> MakeSqrAverageConfig()
    {
        return std::make_unique<SqrAverageAlgorithmConfig>(new TSqrAverageAlgorithm());
    }

    static ISqrAverageAlgorithm *GetSqrAverageAlgorithm();

    // Constructor - registers algorithm
    SqrAverageAlgorithmConfig(ISqrAverageAlgorithm *instance);
    // Dtor deinits algorithm, supports RAII
    ~SqrAverageAlgorithmConfig();

private:
    static SqrAverageAlgorithmConfig *GetInstance();
    
    std::unique_ptr<ISqrAverageAlgorithm> algorithm;
    // disallowed
    SqrAverageAlgorithmConfig() = delete;
    SqrAverageAlgorithmConfig(const SqrAverageAlgorithmConfig&) = delete;
    SqrAverageAlgorithmConfig(SqrAverageAlgorithmConfig&&) = delete;
    void operator=(const SqrAverageAlgorithmConfig&) = delete;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename TIntType, typename TFloatType>
class SqrAverageNaiveAlgorithm : public ISqrAverageAlgorithm
{
public:
    SqrAverageNaiveAlgorithm()
        : partialSum(0)
        , numberOfElements(0)
    {}
    virtual ~SqrAverageNaiveAlgorithm() { }

    virtual double AddNumber(int number)
    {
        if (number < minimumAcceptedValue || number > maximumAcceptedValue)
        {
            LOG_ERROR("Wrong number (ignored) : " + std::to_string(number));
            return 0.0;
        }

        ++numberOfElements;
        partialSum += TIntType(number) * TIntType(number);
        return sqrt(TFloatType(partialSum) / numberOfElements).convert_to<double>();
    }

private:
    // Disallowed
    SqrAverageNaiveAlgorithm(const SqrAverageNaiveAlgorithm&) = delete;
    SqrAverageNaiveAlgorithm(SqrAverageNaiveAlgorithm&&) = delete;
    void operator=(const SqrAverageNaiveAlgorithm&) = delete;

    TIntType partialSum;
    TIntType numberOfElements;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SqrAverageBucketAlgorithm : public ISqrAverageAlgorithm
{
public:
    SqrAverageBucketAlgorithm();
    virtual ~SqrAverageBucketAlgorithm();
    virtual double AddNumber(int number);

private:
    SqrAverageBucketAlgorithm(const SqrAverageBucketAlgorithm&) = delete;
    SqrAverageBucketAlgorithm(SqrAverageBucketAlgorithm&&) = delete;
    void operator=(const SqrAverageBucketAlgorithm&) = delete;

    void AddNewNumberToBuckets(int number);

    std::vector<uint64_t> buckets;
    uint64_t numberOfElements;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace Servers