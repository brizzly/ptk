#ifndef KSOUND_H
#define KSOUND_H

#include <Al/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include <string>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

class KSound {
public:
    KSound();
    ~KSound();

    bool loadSample(const std::string &filename, AAssetManager *assetManager);
    void play();
    void stop();
    void setLooping(bool loop);
    void setVolume(float volume);

private:
    ALuint buffer;
    ALuint source;
    ALCdevice *device;
    ALCcontext *context;

    std::vector<char> loadWAVFile(const std::string &filename, ALenum &format, ALsizei &freq, AAssetManager *assetManager);
};

#endif // KSOUND_H
