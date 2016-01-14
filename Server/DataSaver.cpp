#include "DataSaver.hpp"

#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/map.hpp>
#include "Logger/Logger.hpp"

namespace Server
{
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DataSaver::DataSaver(const std::string &binaryFilePath, std::chrono::milliseconds delay)
    : serializerDelay(delay)
    , serializationPath(binaryFilePath)
{
    std::ofstream serializeFile(serializationPath);
    if (serializeFile.is_open())
    {
        serializeFile.close();
        dataWriter = std::thread(&DataSaver::ProcessQueue, this);
    }
    else
    {
        LOG_ERROR(std::string("Failed to open file path = ") + serializationPath);
    }
}

DataSaver::~DataSaver()
{
    if (dataWriter.joinable())
    {
        LOG_INFO("Sending data saver quit request");
        quitNotification.notify_one();
        dataWriter.join();
    }
}

void DataSaver::QueueSave(int number)
{
    requestQueue.push(number);
}

void DataSaver::ProcessQueue()
{
    bool quitThread = false;
    while (!quitThread)
    {
        std::unique_lock<std::mutex> lock(quitMutex);
        if (std::cv_status::no_timeout == quitNotification.wait_for(lock, serializerDelay))
        {
            LOG_INFO("Data saver thread exited");
            quitThread = true;
        }
        else
        {
            LOG_INFO("Started processing the queue");
            MigrateToBinaryTree();
            LOG_INFO("Finished processing the queue");
            LOG_INFO("Started serialization");
            SerializeBinaryTree();
            LOG_INFO("Finished serialization");
        }
    }
}

void DataSaver::MigrateToBinaryTree()
{
    int number;
    while (requestQueue.pop(number))
    {
        if (valueCount.end() == valueCount.find(number))
        {
            valueCount[number] = 0;
        }
        ++valueCount[number];
    }
}

void DataSaver::SerializeBinaryTree()
{
    std::ofstream serializeFile(serializationPath);
    if (serializeFile.is_open())
    {
        LOG_INFO(std::string("Serializing to path ") + serializationPath);
        boost::archive::text_oarchive output(serializeFile);
        output << valueCount;
    }
    else
    {
        LOG_ERROR(std::string("Failed to open file path = ") + serializationPath);
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace Server