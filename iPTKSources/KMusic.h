#ifndef KMUSIC_H
#define KMUSIC_H


#ifdef __ANDROID__
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#else
#include <AudioToolbox/AudioToolbox.h>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <CoreFoundation/CoreFoundation.h>
#endif


#include <string>
#include <vector>

class KMusic {
public:
    KMusic();
    ~KMusic();

    bool load(const std::string &filename);
    void playMusic(/*char *filename*/);
    void stop();
    void setLooping(bool loop);

private:
    ALuint buffer;
    ALuint source;
    ALCdevice *device;
    ALCcontext *context;
    
    bool isLooping;

    ALuint loadMP3toBuffer(const char* filePath);
};


#endif // KMUSIC_H
