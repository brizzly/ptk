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

    int errcode = alGetError();
    printf("err: %d\n", errcode);

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

    // Read the "RIFF" chunk descriptor (First 4 bytes should be "RIFF")
    char riff[4];
    AAsset_read(asset, riff, 4);
    if (strncmp(riff, "RIFF", 4) != 0) {
        std::cerr << "Invalid WAV file: " << filename << std::endl;
        AAsset_close(asset);
        return {};
    }

    // Skip over the next 4 bytes (chunk size) and read "WAVE" identifier
    AAsset_seek(asset, 4, SEEK_CUR);
    char wave[4];
    AAsset_read(asset, wave, 4);
    if (strncmp(wave, "WAVE", 4) != 0) {
        std::cerr << "Invalid WAV file: " << filename << std::endl;
        AAsset_close(asset);
        return {};
    }

    // Read "fmt " subchunk (first 4 bytes should be "fmt ")
    char fmt[4];
    AAsset_read(asset, fmt, 4);
    if (strncmp(fmt, "fmt ", 4) != 0) {
        std::cerr << "Invalid WAV file: " << filename << std::endl;
        AAsset_close(asset);
        return {};
    }

    // Skip the next 4 bytes (subchunk size) and read the audio format (should be 1 for PCM)
    AAsset_seek(asset, 4, SEEK_CUR);
    uint16_t audioFormat;
    AAsset_read(asset, &audioFormat, sizeof(audioFormat));
    if (audioFormat != 1) { // Only PCM is supported
        std::cerr << "Unsupported WAV format: " << filename << std::endl;
        AAsset_close(asset);
        return {};
    }

    // Read the number of channels (Offset 22 in file)
    uint16_t channels;
    AAsset_read(asset, &channels, sizeof(channels));

    // Read the sample rate (Offset 24)
    uint32_t sampleRate;
    AAsset_read(asset, &sampleRate, sizeof(sampleRate));
    freq = sampleRate;

    // Skip next 6 bytes (byte rate and block align)
    AAsset_seek(asset, 6, SEEK_CUR);

    // Read bits per sample (Offset 34)
    uint16_t bitsPerSample;
    AAsset_read(asset, &bitsPerSample, sizeof(bitsPerSample));

    // Determine the OpenAL format based on channels and bits per sample
    if (channels == 1 && bitsPerSample == 16) {
        format = AL_FORMAT_MONO16;
    } else if (channels == 2 && bitsPerSample == 16) {
        format = AL_FORMAT_STEREO16;
    } else {
        std::cerr << "Unsupported WAV format: channels=" << channels << ", bitsPerSample=" << bitsPerSample << std::endl;
        AAsset_close(asset);
        return {};
    }

    // Skip to the "data" chunk
    char dataHeader[4];
    while (AAsset_read(asset, dataHeader, 4) == 4) {
        if (strncmp(dataHeader, "data", 4) == 0) {
            break;
        }
        // Skip the size of the chunk if it's not "data"
        uint32_t chunkSize;
        AAsset_read(asset, &chunkSize, sizeof(chunkSize));
        AAsset_seek(asset, chunkSize, SEEK_CUR);
    }

    // Read the size of the data chunk
    uint32_t dataSize;
    AAsset_read(asset, &dataSize, sizeof(dataSize));

    // Read the actual audio data into a buffer
    std::vector<char> buffer(dataSize);
    AAsset_read(asset, buffer.data(), dataSize);

    // Close the asset file
    AAsset_close(asset);

    return buffer;
}
