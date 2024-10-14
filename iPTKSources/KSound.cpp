#include "KSound.h"
#include <iostream>
#include <fstream>

KSound::KSound() {
    // Initialize OpenAL
    device = alcOpenDevice(nullptr);
    if (!device) {
        std::cerr << "Failed to open OpenAL device" << std::endl;
        return;
    }

    context = alcCreateContext(device, nullptr);
    if (!alcMakeContextCurrent(context)) {
        std::cerr << "Failed to make OpenAL context current" << std::endl;
        return;
    }

    alGenBuffers(1, &buffer);
    alGenSources(1, &source);
}

KSound::~KSound() {
    // Clean up OpenAL resources
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(context);
    alcCloseDevice(device);
}

bool KSound::loadSample(const std::string &filename, AAssetManager *assetManager) {
    ALenum format;
    ALsizei freq;
    std::vector<char> data = loadWAVFile(filename, format, freq, assetManager);

    if (data.empty()) {
        std::cerr << "Failed to load WAV file: " << filename << std::endl;
        return false;
    }

    alBufferData(buffer, format, data.data(), static_cast<ALsizei>(data.size()), freq);
    alSourcei(source, AL_BUFFER, buffer);

    return true;
}

void KSound::play() {
    alSourcePlay(source);
}

void KSound::stop() {
    alSourceStop(source);
}

void KSound::setLooping(bool loop) {
    alSourcei(source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
}

void KSound::setVolume(float volume) {
    alSourcef(source, AL_GAIN, volume);
}

std::vector<char> KSound::loadWAVFile(const std::string &filename, ALenum &format, ALsizei &freq, AAssetManager *assetManager) {
    // Open the WAV file from assets
    AAsset *asset = AAssetManager_open(assetManager, filename.c_str(), AASSET_MODE_UNKNOWN);
    if (!asset) {
        std::cerr << "Unable to open asset: " << filename << std::endl;
        return {};
    }

    // Read the WAV header
    char riff[4];
    AAsset_read(asset, riff, 4);
    if (strncmp(riff, "RIFF", 4) != 0) {
        std::cerr << "Invalid WAV file: " << filename << std::endl;
        AAsset_close(asset);
        return {};
    }

    // Skip over the "WAVE" identifier and go to the "fmt " chunk
    AAsset_seek(asset, 20, SEEK_CUR);
    uint16_t channels;
    AAsset_read(asset, &channels, sizeof(channels));

    AAsset_read(asset, &freq, sizeof(freq));

    // Skip to bits per sample
    AAsset_seek(asset, 6, SEEK_CUR);
    uint16_t bitsPerSample;
    AAsset_read(asset, &bitsPerSample, sizeof(bitsPerSample));

    if (channels == 1) {
        format = (bitsPerSample == 8) ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
    } else {
        format = (bitsPerSample == 8) ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;
    }

    // Skip to the "data" chunk
    AAsset_seek(asset, 4, SEEK_CUR);
    uint32_t dataSize;
    AAsset_read(asset, &dataSize, sizeof(dataSize));

    // Read the actual audio data
    std::vector<char> data(dataSize);
    AAsset_read(asset, data.data(), dataSize);

    AAsset_close(asset);
    return data;
}
