#ifndef KSOUND_H
#define KSOUND_H


#include <string>
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

class KSound {
public:
    KSound();
    ~KSound();
#ifdef __ANDROID__
    bool loadSample(const std::string &filename, AAssetManager *assetManager);
#else
    bool loadSample(const char *filename);
    //bool loadSample(const std::string &filename);
#endif
    void playSample();
    void stop();
    void setLooping(bool loop);
    void setVolume(float volume);

private:
    ALuint buffer;
    ALuint source;
    ALCdevice *device;
    ALCcontext *context;

#ifdef __ANDROID__
    std::vector<char> loadWAVFile(const std::string &filename, ALenum &format, ALsizei &freq, AAssetManager *assetManager);
#else
    char* loadWAVFile(const char *filename, ALenum &format, ALsizei &freq, size_t &dataSize);
    char* loadCAFFile(const char* filename, ALenum& format, ALsizei& freq, size_t& dataSize);
    uint32_t be32toh(uint32_t big_endian_32bits);
    uint64_t be64toh(uint64_t big_endian_64bits);
    //std::vector<char> loadWAVFile(const std::string &filename, ALenum &format, ALsizei &freq);
#endif
};

#endif // KSOUND_H
