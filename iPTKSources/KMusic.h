#ifndef KMUSIC_H
#define KMUSIC_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#ifdef __ANDROID__
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include <mpg123.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
// Define logging macros for Android Logcat
#include <android/log.h>
#define LOG_TAG "KMusic"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#else
#include <AudioToolbox/AudioToolbox.h>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <CoreFoundation/CoreFoundation.h>
#endif
#include "OpenALManager.h"


class KMusic {
public:
    KMusic();
    ~KMusic();

#ifdef __ANDROID__
    bool load(const std::string &filename, AAssetManager *assetManager);
#else
    bool load(const std::string &filename);
#endif
    void playMusic(/*char *filename*/);
    void stop();
    void setLooping(bool loop);
    void setVolume(float volume);

private:
    ALuint buffer;
    ALuint source;
    bool isLooping;
#ifdef __ANDROID__
    AAssetManager *assetManager_;
#endif

#ifdef __ANDROID__
    ALuint loadMP3toBuffer(const char* filePath, AAssetManager* assetManager);
#else
    ALuint loadMP3toBuffer(const char* filePath);
#endif

    const char* getOpenALErrorString(ALenum error);
};


#endif // KMUSIC_H
