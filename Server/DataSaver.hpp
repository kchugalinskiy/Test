#pragma once

#include <boost/lockfree/queue.hpp>
#include <condition_variable>
#include <fstream>
#include <string>
#include <thread>
#include <map>
#include <mutex>

namespace Server
{
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class DataSaver
{
public:
    DataSaver(const std::string &binaryFilePath, std::chrono::milliseconds serializerDelay);
    ~DataSaver();

    void QueueSave(int number);

private:
    std::chrono::milliseconds serializerDelay;

    std::thread dataWriter;
    std::fstream serializeFile;
    boost::lockfree::queue<int> requestQueue;
    std::map<int, int> valueCount;

    std::condition_variable quitNotification;
    std::mutex quitMutex;

    void ProcessQueue();

    // disallowed
    DataSaver() = delete;
    DataSaver(DataSaver&&) = delete;
    DataSaver(const DataSaver&) = delete;
    void operator=(const DataSaver&) = delete;

};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace Server