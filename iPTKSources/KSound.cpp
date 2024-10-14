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

#ifdef __ANDROID__

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

#else

bool KSound::loadSample(const char *filename)
{
    ALenum format;
    ALsizei freq;
    size_t dataSize;  // Declare a variable to hold the size of the audio data
    char * data = NULL;
   
    // Open the file as a binary stream
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        //std::cerr << "Unable to open file: " << filename << std::endl;
        return false;
    }

    // Read the "RIFF" chunk descriptor
    char riff[4];
    file.read(riff, 4);
    printf("%s\n", riff);
    if (!strncasecmp(riff, "RIFF", 4)) {
        
        data = this->loadWAVFile(filename, format, freq, dataSize);
    }
    else if (!strncasecmp(riff, "CAFF", 4)) {
        
        data = this->loadCAFFile(filename, format, freq, dataSize);
    }
    else {
        return false;
    }
    
    
    // Check if the data is null (loading failed)
    if (data == NULL) {
        //std::cerr << "Failed to load WAV file: " << filename << std::endl;
        return false;
    }

    // Use alBufferData to load the audio data into OpenAL buffer
    alBufferData(buffer, format, data, static_cast<ALsizei>(dataSize), freq);
    alSourcei(source, AL_BUFFER, buffer);

    // Check for any errors
    int errcode = alGetError();
    if (errcode != AL_NO_ERROR) {
        //std::cerr << "OpenAL error while loading sound data: " << errcode << std::endl;
        delete[] data;  // Don't forget to clean up the allocated memory
        return false;
    }

    // Clean up the audio data buffer after loading into OpenAL
    delete[] data;

    return true;
}

char * KSound::loadWAVFile(const char *filename, ALenum &format, ALsizei &freq, size_t &dataSize)
{
    // Open the file as a binary stream
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        //std::cerr << "Unable to open file: " << filename << std::endl;
        return NULL;
    }

    // Read the "RIFF" chunk descriptor
    char riff[4];
    file.read(riff, 4);
    if (strncmp(riff, "RIFF", 4) != 0) {
        //std::cerr << "Invalid WAV file: " << filename << std::endl;
        file.close();
        return NULL;
    }

    // Skip the next 4 bytes (chunk size) and read the "WAVE" identifier
    file.seekg(4, std::ios::cur);
    char wave[4];
    file.read(wave, 4);
    if (strncmp(wave, "WAVE", 4) != 0) {
        //std::cerr << "Invalid WAV file: " << filename << std::endl;
        file.close();
        return NULL;
    }

    // Read the "fmt " subchunk (first 4 bytes should be "fmt ")
    char fmt[4];
    file.read(fmt, 4);
    if (strncmp(fmt, "fmt ", 4) != 0) {
        //std::cerr << "Invalid WAV file: " << filename << std::endl;
        file.close();
        return NULL;
    }

    // Skip the next 4 bytes (subchunk size) and read the audio format (should be 1 for PCM)
    file.seekg(4, std::ios::cur);
    uint16_t audioFormat;
    file.read(reinterpret_cast<char *>(&audioFormat), sizeof(audioFormat));
    if (audioFormat != 1) { // Only PCM is supported
        //std::cerr << "Unsupported WAV format: " << filename << std::endl;
        file.close();
        return NULL;
    }

    // Read the number of channels
    uint16_t channels;
    file.read(reinterpret_cast<char *>(&channels), sizeof(channels));

    // Read the sample rate
    uint32_t sampleRate;
    file.read(reinterpret_cast<char *>(&sampleRate), sizeof(sampleRate));
    freq = static_cast<ALsizei>(sampleRate);

    // Skip the next 6 bytes (byte rate and block align)
    file.seekg(6, std::ios::cur);

    // Read bits per sample
    uint16_t bitsPerSample;
    file.read(reinterpret_cast<char *>(&bitsPerSample), sizeof(bitsPerSample));

    // Determine the OpenAL format based on channels and bits per sample
    if (channels == 1 && bitsPerSample == 16) {
        format = AL_FORMAT_MONO16;
    } else if (channels == 2 && bitsPerSample == 16) {
        format = AL_FORMAT_STEREO16;
    } else {
        //std::cerr << "Unsupported WAV format: channels=" << channels << ", bitsPerSample=" << bitsPerSample << std::endl;
        file.close();
        return NULL;
    }

    // Find the "data" subchunk (skip other chunks if necessary)
    char dataHeader[4];
    while (file.read(dataHeader, 4)) {
        if (strncmp(dataHeader, "data", 4) == 0) {
            // Read the size of the data chunk
            file.read(reinterpret_cast<char *>(&dataSize), sizeof(dataSize));
            break;
        } else {
            // Skip the chunk size
            uint32_t chunkSize;
            file.read(reinterpret_cast<char *>(&chunkSize), sizeof(chunkSize));
            file.seekg(chunkSize, std::ios::cur);
        }
    }

    if (strncmp(dataHeader, "data", 4) != 0) {
        //std::cerr << "No data chunk found in WAV file: " << filename << std::endl;
        file.close();
        return NULL;
    }

    // Allocate memory for the audio data
    char* buffer = new char[dataSize];
    if (!buffer) {
        //std::cerr << "Memory allocation failed for WAV file data: " << filename << std::endl;
        file.close();
        return NULL;
    }

    // Read the actual audio data into a buffer
    file.read(buffer, dataSize);

    // Close the file
    file.close();

    return buffer;  // Return the allocated buffer
}

uint32_t KSound::be32toh(uint32_t big_endian_32bits) {
    return ((big_endian_32bits << 24) & 0xFF000000) |
           ((big_endian_32bits << 8) & 0x00FF0000) |
           ((big_endian_32bits >> 8) & 0x0000FF00) |
           ((big_endian_32bits >> 24) & 0x000000FF);
}

uint64_t KSound::be64toh(uint64_t big_endian_64bits) {
    return ((uint64_t)be32toh(big_endian_64bits & 0xFFFFFFFF) << 32) |
            be32toh((big_endian_64bits >> 32) & 0xFFFFFFFF);
}

char* KSound::loadCAFFile(const char* filename, ALenum& format, ALsizei& freq, size_t& dataSize)
{
    // Open the file as a binary stream
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Unable to open CAF file: " << filename << std::endl;
        return NULL;
    }

    // Read the CAF file header
    char fileType[4];
    file.read(fileType, 4);
    if (strncmp(fileType, "caff", 4) != 0) {
        std::cerr << "Invalid CAF file: " << filename << std::endl;
        file.close();
        return NULL;
    }

    // Read the file header version and flags
    uint16_t version;
    uint16_t flags;
    file.read(reinterpret_cast<char*>(&version), sizeof(version));
    file.read(reinterpret_cast<char*>(&flags), sizeof(flags));

    // Swap bytes if necessary (CAF files are big-endian)
    version = ntohs(version);
    flags = ntohs(flags);

    // Variables to hold audio format information
    double sampleRate = 0.0;
    uint32_t formatID = 0;
    uint32_t formatFlags = 0;
    uint32_t bytesPerPacket = 0;
    uint32_t framesPerPacket = 0;
    uint32_t channelsPerFrame = 0;
    uint32_t bitsPerChannel = 0;
    uint64_t audioDataOffset = 0;
    uint64_t audioDataSize = 0;

    // Read chunks
    while (!file.eof()) {
        // Read chunk header
        char chunkType[4];
        file.read(chunkType, 4);
        if (file.gcount() < 4) {
            break; // End of file reached
        }

        uint64_t chunkSize = 0;
        file.read(reinterpret_cast<char*>(&chunkSize), sizeof(chunkSize));
        chunkSize = be64toh(chunkSize); // Convert from big-endian to host

        // Remember the current file position
        std::streampos chunkDataStart = file.tellg();

        if (strncmp(chunkType, "desc", 4) == 0) {
            // Read the Audio Description Chunk
            // mSampleRate (Float64)
            uint64_t sampleRateBits = 0;
            file.read(reinterpret_cast<char*>(&sampleRateBits), sizeof(sampleRateBits));
            sampleRateBits = be64toh(sampleRateBits);
            sampleRate = *reinterpret_cast<double*>(&sampleRateBits);

            // mFormatID (UInt32)
            file.read(reinterpret_cast<char*>(&formatID), sizeof(formatID));
            formatID = ntohl(formatID);

            // mFormatFlags (UInt32)
            file.read(reinterpret_cast<char*>(&formatFlags), sizeof(formatFlags));
            formatFlags = ntohl(formatFlags);

            // mBytesPerPacket (UInt32)
            file.read(reinterpret_cast<char*>(&bytesPerPacket), sizeof(bytesPerPacket));
            bytesPerPacket = ntohl(bytesPerPacket);

            // mFramesPerPacket (UInt32)
            file.read(reinterpret_cast<char*>(&framesPerPacket), sizeof(framesPerPacket));
            framesPerPacket = ntohl(framesPerPacket);

            // mChannelsPerFrame (UInt32)
            file.read(reinterpret_cast<char*>(&channelsPerFrame), sizeof(channelsPerFrame));
            channelsPerFrame = ntohl(channelsPerFrame);

            // mBitsPerChannel (UInt32)
            file.read(reinterpret_cast<char*>(&bitsPerChannel), sizeof(bitsPerChannel));
            bitsPerChannel = ntohl(bitsPerChannel);
        }
        else if (strncmp(chunkType, "data", 4) == 0) {
            // Read the Audio Data Chunk
            uint32_t editCount = 0;
            file.read(reinterpret_cast<char*>(&editCount), sizeof(editCount));
            editCount = ntohl(editCount);

            audioDataOffset = file.tellg();
            audioDataSize = chunkSize - sizeof(editCount);

            // We can break here if we assume 'data' chunk comes after 'desc'
            break;
        }
        else {
            // Skip unknown chunk
            file.seekg(chunkSize, std::ios::cur);
        }

        // Move to the next chunk
        file.seekg(chunkDataStart + static_cast<std::streamoff>(chunkSize), std::ios::beg);
    }

    // Check if required chunks were found
    if (sampleRate == 0 || audioDataSize == 0) {
        std::cerr << "Required chunks not found in CAF file: " << filename << std::endl;
        file.close();
        return NULL;
    }

    // Set the frequency
    freq = static_cast<ALsizei>(sampleRate);

    // Determine the OpenAL format based on channels and bits per channel
    if (channelsPerFrame == 1 && bitsPerChannel == 16) {
        format = AL_FORMAT_MONO16;
    }
    else if (channelsPerFrame == 2 && bitsPerChannel == 16) {
        format = AL_FORMAT_STEREO16;
    }
    else {
        std::cerr << "Unsupported CAF format: channels=" << channelsPerFrame
                  << ", bitsPerChannel=" << bitsPerChannel << std::endl;
        file.close();
        return NULL;
    }

    // Allocate memory for the audio data
    dataSize = static_cast<size_t>(audioDataSize);
    char* buffer = new char[dataSize];
    if (!buffer) {
        std::cerr << "Memory allocation failed for CAF file data: " << filename << std::endl;
        file.close();
        return NULL;
    }

    // Read the audio data
    file.seekg(audioDataOffset, std::ios::beg);
    file.read(buffer, dataSize);
    if (file.gcount() != static_cast<std::streamsize>(dataSize)) {
        std::cerr << "Failed to read audio data from CAF file: " << filename << std::endl;
        delete[] buffer;
        file.close();
        return NULL;
    }

    // Close the file
    file.close();

    return buffer;  // Return the allocated buffer
}

#endif

void KSound::playSample() {
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

