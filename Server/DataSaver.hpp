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
    // Supports RAII
    DataSaver(const std::string &binaryFilePath, std::chrono::milliseconds serializerDelay);
    ~DataSaver();

    // Save number. This function is lockfree and thread-safe
    void QueueSave(int number);

private:
    std::chrono::milliseconds serializerDelay;
    const std::string serializationPath;

    std::thread dataWriter;
    boost::lockfree::queue<int> requestQueue;
    std::map<int, int> valueCount;

    std::condition_variable quitNotification;
    std::mutex quitMutex;

    // Main save thread function
    void ProcessQueue();

    // Move all numbers from queue to map
    void MigrateToBinaryTree();

    // Serialize map
    void SerializeBinaryTree();

    // disallowed
    DataSaver() = delete;
    DataSaver(DataSaver&&) = delete;
    DataSaver(const DataSaver&) = delete;
    void operator=(const DataSaver&) = delete;

};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace Server