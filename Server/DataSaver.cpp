#include "DataSaver.hpp"

#include "Logger/Logger.hpp"

namespace Server
{
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DataSaver::DataSaver(const std::string &binaryFilePath, std::chrono::milliseconds delay)
    : serializerDelay(delay)
{
    dataWriter = std::thread(&DataSaver::ProcessQueue, this);
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
            LOG_INFO("Finished processing the queue");
            LOG_INFO("Started serialization");
            LOG_INFO("Finished serialization");
        }
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace Server