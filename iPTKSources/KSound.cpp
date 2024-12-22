#include "KSound.h"



KSound::KSound() {
    
    isLooping = false;
#ifdef __ANDROID__
    assetManager_ = NULL;
#endif
    
    // Obtain the shared OpenALManager instance
    OpenALManager& alManager = OpenALManager::getInstance();

    alGenSources(1, &source);
    ALenum error = alGetError();
    if (error != AL_NO_ERROR) {
        std::cerr << "Error generating OpenAL source: " << getOpenALErrorString(error) << std::endl;
    }
}

KSound::~KSound() {
    
    // Stop playback
    stopSample();

    // Delete OpenAL source and buffer
    if (source)
    {
        alDeleteSources(1, &source);
        source = 0;
    }
    if (buffer)
    {
        alDeleteBuffers(1, &buffer);
        buffer = 0;
    }
}

const char* KSound::getOpenALErrorString(ALenum error)
{
    switch (error)
    {
        case AL_NO_ERROR: return "AL_NO_ERROR";
        case AL_INVALID_NAME: return "AL_INVALID_NAME";
        case AL_INVALID_ENUM: return "AL_INVALID_ENUM";
        case AL_INVALID_VALUE: return "AL_INVALID_VALUE";
        case AL_INVALID_OPERATION: return "AL_INVALID_OPERATION";
        case AL_OUT_OF_MEMORY: return "AL_OUT_OF_MEMORY";
        default: return "Unknown OpenAL Error";
    }
}

#ifdef __ANDROID__

bool KSound::loadSample(const std::string &filename, AAssetManager *assetManager)
{
    ALenum format;
    ALsizei freq;

    this->assetManager_ = assetManager;

    // Open the WAV file from assets
    AAsset *asset = AAssetManager_open(assetManager, filename.c_str(), AASSET_MODE_STREAMING);
    if (!asset) {
        std::cerr << "Unable to open asset: " << filename << std::endl;
        return 0; // 0 is not a valid OpenAL buffer ID
    }

    // Read the "RIFF" chunk descriptor (First 4 bytes should be "RIFF")
    char riff[4];
    ssize_t bytesRead = AAsset_read(asset, riff, 4);
    if (bytesRead == 4 && !strncasecmp(riff, "RIFF", 4)) {

        buffer = this->loadWAVFile(filename, format, freq);
    }
    else {
        std::cerr << "Invalid WAV file (Missing 'RIFF'): " << filename << std::endl;
        AAsset_close(asset);
        return 0;
    }

    alSourcei(source, AL_BUFFER, buffer);

    ALenum error = alGetError();
    if (error != AL_NO_ERROR) {
        std::cerr << "Error: " << getOpenALErrorString(error) << std::endl;
    }

    return true;
}

#else

bool KSound::loadSample(const std::string &filename)
{
    ALenum format;
    ALsizei freq;

    // Open the file as a binary stream
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        //std::cerr << "Unable to open file: " << filename << std::endl;
        return false;
    }

    // Read the "RIFF" chunk descriptor
    char riff[4];
    file.read(riff, 4);
    //printf("%s\n", riff);
    if (!strncasecmp(riff, "RIFF", 4)) {

        buffer = this->loadWAVFile(filename, format, freq);
    }
    else if (!strncasecmp(riff, "CAFF", 4)) {

        buffer = this->loadCAFFile(filename, format, freq);
    }
    else {
        return false;
    }

    alSourcei(source, AL_BUFFER, buffer);

    ALenum error = alGetError();
    if (error != AL_NO_ERROR) {
        std::cerr << "Error: " << getOpenALErrorString(error) << std::endl;
    }

    return true;
}

#endif

#ifdef __ANDROID__

ALuint KSound::loadWAVFile(const std::string &filename, ALenum &format, ALsizei &freq)
{
    AAssetManager *assetManager = this->assetManager_;

    // Open the WAV file from assets in BUFFER mode to prevent compression issues
    AAsset *asset = AAssetManager_open(assetManager, filename.c_str(), AASSET_MODE_BUFFER);
    if (!asset) {
        std::cerr << "Unable to open asset: " << filename << std::endl;
        return 0; // 0 is not a valid OpenAL buffer ID
    }

    // Read the "RIFF" chunk descriptor (First 4 bytes should be "RIFF")
    char riff[4];
    ssize_t bytesRead = AAsset_read(asset, riff, 4);
    if (bytesRead != 4 || strncmp(riff, "RIFF", 4) != 0) {
        std::cerr << "Invalid WAV file (Missing 'RIFF'): " << filename << std::endl;
        AAsset_close(asset);
        return 0;
    }

    // Read the chunk size (4 bytes)
    uint32_t chunkSize = 0;
    bytesRead = AAsset_read(asset, &chunkSize, sizeof(chunkSize));
    if (bytesRead != sizeof(chunkSize)) {
        std::cerr << "Failed to read chunk size." << std::endl;
        AAsset_close(asset);
        return 0;
    }

    // Read the "WAVE" identifier (4 bytes)
    char wave[4];
    bytesRead = AAsset_read(asset, wave, 4);
    if (bytesRead != 4 || strncmp(wave, "WAVE", 4) != 0) {
        std::cerr << "Invalid WAV file (Missing 'WAVE'): " << filename << std::endl;
        AAsset_close(asset);
        return 0;
    }

    // Read the "fmt " subchunk ID (4 bytes)
    char fmt[4];
    bytesRead = AAsset_read(asset, fmt, 4);
    if (bytesRead != 4 || strncmp(fmt, "fmt ", 4) != 0) {
        std::cerr << "Invalid WAV file (Missing 'fmt ' subchunk): " << filename << std::endl;
        AAsset_close(asset);
        return 0;
    }

    // Read the subchunk1 size (4 bytes)
    uint32_t subChunk1Size = 0;
    bytesRead = AAsset_read(asset, &subChunk1Size, sizeof(subChunk1Size));
    if (bytesRead != sizeof(subChunk1Size)) {
        std::cerr << "Failed to read 'fmt ' subchunk size." << std::endl;
        AAsset_close(asset);
        return 0;
    }

    // Read the audio format (2 bytes)
    uint16_t audioFormat = 0;
    bytesRead = AAsset_read(asset, &audioFormat, sizeof(audioFormat));
    if (bytesRead != sizeof(audioFormat)) {
        std::cerr << "Failed to read audio format." << std::endl;
        AAsset_close(asset);
        return 0;
    }
    if (audioFormat != 1) { // Only PCM is supported
        std::cerr << "Unsupported WAV format (Only PCM is supported): " << filename << std::endl;
        AAsset_close(asset);
        return 0;
    }

    // Read the number of channels (2 bytes)
    uint16_t channels = 0;
    bytesRead = AAsset_read(asset, &channels, sizeof(channels));
    if (bytesRead != sizeof(channels)) {
        std::cerr << "Failed to read number of channels." << std::endl;
        AAsset_close(asset);
        return 0;
    }

    // Read the sample rate (4 bytes)
    uint32_t sampleRate = 0;
    bytesRead = AAsset_read(asset, &sampleRate, sizeof(sampleRate));
    if (bytesRead != sizeof(sampleRate)) {
        std::cerr << "Failed to read sample rate." << std::endl;
        AAsset_close(asset);
        return 0;
    }
    freq = static_cast<ALsizei>(sampleRate);

    // Read the byte rate (4 bytes)
    uint32_t byteRate = 0;
    bytesRead = AAsset_read(asset, &byteRate, sizeof(byteRate));
    if (bytesRead != sizeof(byteRate)) {
        std::cerr << "Failed to read byte rate." << std::endl;
        AAsset_close(asset);
        return 0;
    }

    // Read the block align (2 bytes)
    uint16_t blockAlign = 0;
    bytesRead = AAsset_read(asset, &blockAlign, sizeof(blockAlign));
    if (bytesRead != sizeof(blockAlign)) {
        std::cerr << "Failed to read block align." << std::endl;
        AAsset_close(asset);
        return 0;
    }

    // Read bits per sample (2 bytes)
    uint16_t bitsPerSample = 0;
    bytesRead = AAsset_read(asset, &bitsPerSample, sizeof(bitsPerSample));
    if (bytesRead != sizeof(bitsPerSample)) {
        std::cerr << "Failed to read bits per sample." << std::endl;
        AAsset_close(asset);
        return 0;
    }

    // If subChunk1Size > 16, skip the extra bytes
    if (subChunk1Size > 16) {
        uint32_t extraBytes = subChunk1Size - 16;
        AAsset_seek(asset, extraBytes, SEEK_CUR);
    }

    // Determine the OpenAL format based on channels and bits per sample
    if (channels == 1 && bitsPerSample == 8) {
        format = AL_FORMAT_MONO8;
    } else if (channels == 1 && bitsPerSample == 16) {
        format = AL_FORMAT_MONO16;
    } else if (channels == 2 && bitsPerSample == 8) {
        format = AL_FORMAT_STEREO8;
    } else if (channels == 2 && bitsPerSample == 16) {
        format = AL_FORMAT_STEREO16;
    } else {
        std::cerr << "Unsupported WAV format: channels=" << channels
                  << ", bitsPerSample=" << bitsPerSample << std::endl;
        AAsset_close(asset);
        return 0;
    }

    // Find the "data" subchunk
    char dataHeader[4];
    bool dataChunkFound = false;
    uint32_t dataSize = 0;

    while (AAsset_read(asset, dataHeader, 4) == 4) {
        if (strncmp(dataHeader, "data", 4) == 0) {
            // Read the size of the data chunk (4 bytes)
            bytesRead = AAsset_read(asset, &dataSize, sizeof(dataSize));
            if (bytesRead != sizeof(dataSize)) {
                std::cerr << "Failed to read 'data' subchunk size." << std::endl;
                AAsset_close(asset);
                return 0;
            }
            dataChunkFound = true;
            break;
        } else {
            // Read the size of the current chunk to skip it
            uint32_t chunkSize = 0;
            bytesRead = AAsset_read(asset, &chunkSize, sizeof(chunkSize));
            if (bytesRead != sizeof(chunkSize)) {
                std::cerr << "Failed to read chunk size." << std::endl;
                AAsset_close(asset);
                return 0;
            }
            // Skip the current chunk's data
            AAsset_seek(asset, chunkSize, SEEK_CUR);
        }
    }

    if (!dataChunkFound) {
        std::cerr << "No 'data' chunk found in WAV file: " << filename << std::endl;
        AAsset_close(asset);
        return 0;
    }

    // Read the actual audio data into a buffer
    std::vector<char> audioData(dataSize);
    ssize_t totalRead = 0;
    while (totalRead < dataSize) {
        ssize_t bytes = AAsset_read(asset, audioData.data() + totalRead, dataSize - totalRead);
        if (bytes <= 0) {
            std::cerr << "Failed to read audio data from WAV file: " << filename << std::endl;
            AAsset_close(asset);
            return 0;
        }
        totalRead += bytes;
    }

    // Close the asset file
    AAsset_close(asset);

    // Generate OpenAL buffer
    ALuint bufferID = 0;
    alGenBuffers(1, &bufferID);
    ALenum error = alGetError();
    if (error != AL_NO_ERROR) {
        std::cerr << "Error generating OpenAL buffer: " << getOpenALErrorString(error) << std::endl;
        return 0;
    }

    if (bufferID == 0) {
        std::cerr << "alGenBuffers failed to generate a valid buffer." << std::endl;
        return 0;
    }

    // Buffer the audio data into OpenAL
    alBufferData(bufferID, format, audioData.data(), static_cast<ALsizei>(dataSize), freq);
    error = alGetError();
    if (error != AL_NO_ERROR) {
        std::cerr << "Error buffering data into OpenAL buffer: " << getOpenALErrorString(error) << std::endl;
        alDeleteBuffers(1, &bufferID);
        return 0;
    }

    // Return the generated OpenAL buffer ID
    return bufferID;
}


#else

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

ALuint KSound::loadWAVFile(const std::string &filename, ALenum &format, ALsizei &freq)
{
    // Open the WAV file as a binary stream
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return 0; // 0 is not a valid OpenAL buffer ID
    }

    // Read the "RIFF" chunk descriptor (First 4 bytes should be "RIFF")
    char riff[4];
    file.read(riff, 4);
    if (file.gcount() != 4 || strncmp(riff, "RIFF", 4) != 0) {
        std::cerr << "Invalid WAV file (Missing 'RIFF'): " << filename << std::endl;
        file.close();
        return 0;
    }

    // Read the chunk size (4 bytes) and "WAVE" identifier (4 bytes)
    char wave[4];
    file.read(wave, 4);
    if (file.gcount() != 4 || strncmp(wave, "WAVE", 4) != 0) {
        std::cerr << "Invalid WAV file (Missing 'WAVE'): " << filename << std::endl;
        file.close();
        return 0;
    }

    // Read the "fmt " subchunk (4 bytes)
    char fmt[4];
    file.read(fmt, 4);
    if (file.gcount() != 4 || strncmp(fmt, "fmt ", 4) != 0) {
        std::cerr << "Invalid WAV file (Missing 'fmt '): " << filename << std::endl;
        file.close();
        return 0;
    }

    // Read the subchunk size (4 bytes)
    uint32_t subChunk1Size = 0;
    file.read(reinterpret_cast<char*>(&subChunk1Size), sizeof(subChunk1Size));
    if (file.gcount() != sizeof(subChunk1Size)) {
        std::cerr << "Failed to read 'fmt ' subchunk size." << std::endl;
        file.close();
        return 0;
    }

    // Read the audio format (2 bytes)
    uint16_t audioFormat = 0;
    file.read(reinterpret_cast<char*>(&audioFormat), sizeof(audioFormat));
    if (file.gcount() != sizeof(audioFormat)) {
        std::cerr << "Failed to read audio format." << std::endl;
        file.close();
        return 0;
    }
    if (audioFormat != 1) { // Only PCM is supported
        std::cerr << "Unsupported WAV format (Only PCM is supported): " << filename << std::endl;
        file.close();
        return 0;
    }

    // Read the number of channels (2 bytes)
    uint16_t channels = 0;
    file.read(reinterpret_cast<char*>(&channels), sizeof(channels));
    if (file.gcount() != sizeof(channels)) {
        std::cerr << "Failed to read number of channels." << std::endl;
        file.close();
        return 0;
    }

    // Read the sample rate (4 bytes)
    uint32_t sampleRate = 0;
    file.read(reinterpret_cast<char*>(&sampleRate), sizeof(sampleRate));
    if (file.gcount() != sizeof(sampleRate)) {
        std::cerr << "Failed to read sample rate." << std::endl;
        file.close();
        return 0;
    }
    freq = static_cast<ALsizei>(sampleRate);

    // Read the byte rate (4 bytes) and block align (2 bytes) - skipping as they are not needed here
    file.seekg(6, std::ios::cur);

    // Read bits per sample (2 bytes)
    uint16_t bitsPerSample = 0;
    file.read(reinterpret_cast<char*>(&bitsPerSample), sizeof(bitsPerSample));
    if (file.gcount() != sizeof(bitsPerSample)) {
        std::cerr << "Failed to read bits per sample." << std::endl;
        file.close();
        return 0;
    }

    // Determine the OpenAL format based on channels and bits per sample
    if (channels == 1 && bitsPerSample == 8) {
        format = AL_FORMAT_MONO8;
    } else if (channels == 1 && bitsPerSample == 16) {
        format = AL_FORMAT_MONO16;
    } else if (channels == 2 && bitsPerSample == 8) {
        format = AL_FORMAT_STEREO8;
    } else if (channels == 2 && bitsPerSample == 16) {
        format = AL_FORMAT_STEREO16;
    } else {
        std::cerr << "Unsupported WAV format: channels=" << channels
                  << ", bitsPerSample=" << bitsPerSample << std::endl;
        file.close();
        return 0;
    }

    // Find the "data" subchunk
    char dataHeader[4];
    bool dataChunkFound = false;
    uint32_t dataSize = 0;

    while (file.read(dataHeader, 4)) {
        if (strncmp(dataHeader, "data", 4) == 0) {
            // Read the size of the data chunk (4 bytes)
            file.read(reinterpret_cast<char*>(&dataSize), sizeof(dataSize));
            if (file.gcount() != sizeof(dataSize)) {
                std::cerr << "Failed to read 'data' subchunk size." << std::endl;
                file.close();
                return 0;
            }
            dataChunkFound = true;
            break;
        } else {
            // Read the size of the current chunk to skip it
            uint32_t chunkSize = 0;
            file.read(reinterpret_cast<char*>(&chunkSize), sizeof(chunkSize));
            if (file.gcount() != sizeof(chunkSize)) {
                std::cerr << "Failed to read chunk size." << std::endl;
                file.close();
                return 0;
            }
            // Skip the current chunk's data
            file.seekg(chunkSize, std::ios::cur);
        }
    }

    if (!dataChunkFound) {
        std::cerr << "No 'data' chunk found in WAV file: " << filename << std::endl;
        file.close();
        return 0;
    }

    // Read the actual audio data into a buffer
    std::vector<char> audioData(dataSize);
    file.read(audioData.data(), dataSize);
    if (file.gcount() != static_cast<std::streamsize>(dataSize)) {
        std::cerr << "Failed to read audio data from WAV file: " << filename << std::endl;
        file.close();
        return 0;
    }

    // Close the file
    file.close();

    // Generate OpenAL buffer
    ALuint bufferID = 0;
    alGenBuffers(1, &bufferID);
    ALenum error = alGetError();
    if (error != AL_NO_ERROR) {
        std::cerr << "Error generating OpenAL buffer: " << getOpenALErrorString(error) << std::endl;
        return 0;
    }

    if (bufferID == 0) {
        std::cerr << "alGenBuffers failed to generate a valid buffer." << std::endl;
        return 0;
    }

    // Buffer the audio data into OpenAL
    alBufferData(bufferID, format, audioData.data(), static_cast<ALsizei>(dataSize), freq);
    error = alGetError();
    if (error != AL_NO_ERROR) {
        std::cerr << "Error buffering data into OpenAL buffer: " << getOpenALErrorString(error) << std::endl;
        alDeleteBuffers(1, &bufferID);
        return 0;
    }

    // Return the generated OpenAL buffer ID
    return bufferID;
}

ALuint KSound::loadCAFFile(const std::string &filename, ALenum& format, ALsizei& freq)
{
    // Open the file as a binary stream
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Unable to open CAF file: " << filename << std::endl;
        return 0; // Use 0 to indicate failure for ALuint
    }

    // Read the CAF file header
    char fileType[4];
    file.read(fileType, 4);
    if (strncmp(fileType, "caff", 4) != 0) {
        std::cerr << "Invalid CAF file (Missing 'caff'): " << filename << std::endl;
        file.close();
        return 0;
    }

    // Read the file header version and flags
    uint16_t version;
    uint16_t flags;
    file.read(reinterpret_cast<char*>(&version), sizeof(version));
    file.read(reinterpret_cast<char*>(&flags), sizeof(flags));

    // Convert from big-endian to host byte order (16-bit)
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
        chunkSize = be64toh(chunkSize); // Convert from big-endian to host byte order

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
            formatID = be32toh(formatID);

            // mFormatFlags (UInt32)
            file.read(reinterpret_cast<char*>(&formatFlags), sizeof(formatFlags));
            formatFlags = be32toh(formatFlags);

            // mBytesPerPacket (UInt32)
            file.read(reinterpret_cast<char*>(&bytesPerPacket), sizeof(bytesPerPacket));
            bytesPerPacket = be32toh(bytesPerPacket);

            // mFramesPerPacket (UInt32)
            file.read(reinterpret_cast<char*>(&framesPerPacket), sizeof(framesPerPacket));
            framesPerPacket = be32toh(framesPerPacket);

            // mChannelsPerFrame (UInt32)
            file.read(reinterpret_cast<char*>(&channelsPerFrame), sizeof(channelsPerFrame));
            channelsPerFrame = be32toh(channelsPerFrame);

            // mBitsPerChannel (UInt32)
            file.read(reinterpret_cast<char*>(&bitsPerChannel), sizeof(bitsPerChannel));
            bitsPerChannel = be32toh(bitsPerChannel);
        }
        else if (strncmp(chunkType, "data", 4) == 0) {
            // Read the Audio Data Chunk
            uint32_t editCount = 0;
            file.read(reinterpret_cast<char*>(&editCount), sizeof(editCount));
            editCount = be32toh(editCount);

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
        return 0;
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
        return 0;
    }

    // Allocate memory for the audio data

    uint32_t dataSize = static_cast<size_t>(audioDataSize);
    //dataSize = static_cast<size_t>(audioDataSize);
    char* data = new(std::nothrow) char[dataSize];
    if (!data) {
        std::cerr << "Memory allocation failed for CAF file data: " << filename << std::endl;
        file.close();
        return 0;
    }

    // Read the audio data
    file.seekg(audioDataOffset, std::ios::beg);
    file.read(data, dataSize);
    if (file.gcount() != static_cast<std::streamsize>(dataSize)) {
        std::cerr << "Failed to read audio data from CAF file: " << filename << std::endl;
        delete[] data;
        file.close();
        return 0;
    }

    // Close the file
    file.close();

    // Generate OpenAL buffer
    ALuint bufferID = 0;
    alGenBuffers(1, &bufferID);
    ALenum error = alGetError();
    if (error != AL_NO_ERROR) {
        std::cerr << "Error generating buffer: " << getOpenALErrorString(error) << std::endl;
        delete[] data;
        return 0;
    }

    if (bufferID == 0) {
        std::cerr << "alGenBuffers failed to generate a valid buffer." << std::endl;
        delete[] data;
        return 0;
    }

    // Buffer the audio data into OpenAL
    alBufferData(bufferID, format, data, static_cast<ALsizei>(dataSize), freq);
    error = alGetError();
    if (error != AL_NO_ERROR) {
        std::cerr << "Error buffering data: " << getOpenALErrorString(error) << std::endl;
        alDeleteBuffers(1, &bufferID);
        delete[] data;
        return 0;
    }

    // Free the audio data buffer as OpenAL has copied the data
    delete[] data;

    return bufferID;
}

#endif

void KSound::playSample() {
    alSourcePlay(source);
}

void KSound::stopSample() {
    alSourceStop(source);
}

void KSound::setLooping(bool loop) {
    alSourcei(source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
}

void KSound::setVolume(float volume) {
    alSourcef(source, AL_GAIN, volume);
}

