#include "OpenALManager.h"
#include <iostream>

OpenALManager* OpenALManager::instance = nullptr;
std::mutex OpenALManager::instanceMutex;

OpenALManager& OpenALManager::getInstance()
{
    std::lock_guard<std::mutex> lock(instanceMutex);
    if (!instance)
    {
        instance = new OpenALManager();
    }
    return *instance;
}

OpenALManager::OpenALManager()
{
    device = alcOpenDevice(nullptr);
    if (!device)
    {
        std::cerr << "Failed to open OpenAL device" << std::endl;
        // Handle error appropriately
    }

    context = alcCreateContext(device, nullptr);
    if (!alcMakeContextCurrent(context))
    {
        std::cerr << "Failed to make OpenAL context current" << std::endl;
        // Handle error appropriately
    }
}

OpenALManager::~OpenALManager()
{
    if (context)
    {
        alcMakeContextCurrent(nullptr);
        alcDestroyContext(context);
    }
    if (device)
    {
        alcCloseDevice(device);
    }
}

ALCdevice* OpenALManager::getDevice() const
{
    return device;
}

ALCcontext* OpenALManager::getContext() const
{
    return context;
}
