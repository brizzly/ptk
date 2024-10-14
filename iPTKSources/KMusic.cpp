#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "KMusic.h"



KMusic::KMusic() {
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

KMusic::~KMusic() {
    // Clean up OpenAL resources
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(context);
    alcCloseDevice(device);
}

bool KMusic::load(const std::string &filename) {
    ALenum format;
    ALsizei freq;
    std::vector<char> data = loadWAVFile(filename, format, freq);

    if (data.empty()) {
        std::cerr << "Failed to load WAV file: " << filename << std::endl;
        return false;
    }

    alBufferData(buffer, format, data.data(), static_cast<ALsizei>(data.size()), freq);
    alSourcei(source, AL_BUFFER, buffer);

    return true;
}

void KMusic::play() {
    alSourcePlay(source);
}

void KMusic::stop() {
    alSourceStop(source);
}

void KMusic::setLooping(bool loop) {
    alSourcei(source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
}

std::vector<char> KMusic::loadWAVFile(const std::string &filename, ALenum &format, ALsizei &freq) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return {};
    }

    // Read WAV header
    char riff[4];
    file.read(riff, 4);
    if (strncmp(riff, "RIFF", 4) != 0) {
        std::cerr << "Invalid WAV file: " << filename << std::endl;
        return {};
    }

    file.seekg(22, std::ios::beg);
    uint16_t channels;
    file.read(reinterpret_cast<char *>(&channels), sizeof(channels));

    file.read(reinterpret_cast<char *>(&freq), sizeof(freq));

    file.seekg(34, std::ios::beg);
    uint16_t bitsPerSample;
    file.read(reinterpret_cast<char *>(&bitsPerSample), sizeof(bitsPerSample));

    if (channels == 1) {
        format = (bitsPerSample == 8) ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
    } else {
        format = (bitsPerSample == 8) ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;
    }

    file.seekg(44, std::ios::beg); // Skip to data
    std::vector<char> data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    return data;
}
