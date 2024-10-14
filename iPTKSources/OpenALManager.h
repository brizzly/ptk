#ifndef OPENAL_MANAGER_H
#define OPENAL_MANAGER_H

#ifdef __ANDROID__
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#else
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#endif
#include <mutex>

class OpenALManager
{
public:
    static OpenALManager& getInstance();

    ALCdevice* getDevice() const;
    ALCcontext* getContext() const;

    // Delete copy constructor and assignment operator
    OpenALManager(const OpenALManager&) = delete;
    OpenALManager& operator=(const OpenALManager&) = delete;

private:
    OpenALManager();
    ~OpenALManager();

    ALCdevice* device;
    ALCcontext* context;

    static OpenALManager* instance;
    static std::mutex instanceMutex;
};

#endif // OPENAL_MANAGER_H
