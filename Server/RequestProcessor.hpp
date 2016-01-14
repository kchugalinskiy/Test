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
    RequestProcessor(RequestProcessor&&);
    RequestProcessor(const RequestProcessor&);
    void operator=(const RequestProcessor&);

    std::mutex requestProcessorMutex;
	uint64_t partialSqrSum;
	int numberOfElements;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace Server
