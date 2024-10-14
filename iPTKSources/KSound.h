#ifndef KSOUND_H
#define KSOUND_H


#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#ifdef __ANDROID__
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#else
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#endif
#include "OpenALManager.h"


class KSound {
public:
    KSound();
    ~KSound();
#ifdef __ANDROID__
    bool loadSample(const std::string &filename, AAssetManager *assetManager);
#else
    bool loadSample(const std::string &filename);
#endif
    void playSample();
    void stop();
    void setLooping(bool loop);
    void setVolume(float volume);
    const char* getOpenALErrorString(ALenum error);

private:
    ALuint buffer;
    ALuint source;
    bool isLooping;
#ifdef __ANDROID__
    AAssetManager *assetManager_;
#endif

#ifdef __ANDROID__
    ALuint loadWAVFile(const std::string &filename, ALenum &format, ALsizei &freq/*, AAssetManager *assetManager*/);
#else
    ALuint loadWAVFile(const std::string &filename, ALenum &format, ALsizei &freq);
    ALuint loadCAFFile(const std::string &filename, ALenum& format, ALsizei& freq);
    uint32_t be32toh(uint32_t big_endian_32bits);
    uint64_t be64toh(uint64_t big_endian_64bits);
    //std::vector<char> loadWAVFile(const std::string &filename, ALenum &format, ALsizei &freq);
#endif
    bool loadSampleEx(const std::string &filename);
};

#endif // KSOUND_H
