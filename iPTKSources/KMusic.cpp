#include "KMusic.h"
#include "OpenALManager.h"
#include <iostream>
#include <vector>
#include <string>


KMusic::KMusic()
{
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

KMusic::~KMusic()
{
    // Stop playback
    stopMusic();

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

const char* KMusic::getOpenALErrorString(ALenum error)
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

bool KMusic::load(const std::string &filename, AAssetManager *assetManager)
{
    this->assetManager_ = assetManager;

    // Load MP3 file into OpenAL buffer
    buffer = loadMP3toBuffer(filename.c_str(), assetManager);
    if (buffer == 0)
    {
        std::cerr << "Failed to load MP3 file: " << filename << std::endl;
        return false;
    }

    // Attach buffer to source
    alSourcei(source, AL_BUFFER, buffer);
    if (alGetError() != AL_NO_ERROR)
    {
        std::cerr << "Failed to attach buffer to source" << std::endl;
        return false;
    }

    // Set looping state
    alSourcei(source, AL_LOOPING, isLooping ? AL_TRUE : AL_FALSE);

    return true;
}

#else

bool KMusic::load(const std::string &filename)
{
    // Load MP3 file into OpenAL buffer
    buffer = loadMP3toBuffer(filename.c_str());
    if (buffer == 0)
    {
        std::cerr << "Failed to load MP3 file: " << filename << std::endl;
        return false;
    }

    // Attach buffer to source
    alSourcei(source, AL_BUFFER, buffer);
    if (alGetError() != AL_NO_ERROR)
    {
        std::cerr << "Failed to attach buffer to source" << std::endl;
        return false;
    }

    // Set looping state
    alSourcei(source, AL_LOOPING, isLooping ? AL_TRUE : AL_FALSE);

    return true;
}

#endif

void KMusic::playMusic()
{
    alSourcePlay(source);
    if (alGetError() != AL_NO_ERROR)
    {
        std::cerr << "Failed to play source" << std::endl;
        // Handle error appropriately
    }
}

void KMusic::stopMusic()
{
    alSourceStop(source);
    if (alGetError() != AL_NO_ERROR)
    {
        std::cerr << "Failed to stop source" << std::endl;
        // Handle error appropriately
    }
}

void KMusic::pauseMusic()
{
    
}

void KMusic::resumeMusic()
{
    
}

void KMusic::setLooping(bool loop)
{
    isLooping = loop;
    alSourcei(source, AL_LOOPING, isLooping ? AL_TRUE : AL_FALSE);
    if (alGetError() != AL_NO_ERROR)
    {
        std::cerr << "Failed to set looping state" << std::endl;
        // Handle error appropriately
    }
}

void KMusic::setVolume(float volume) {
    alSourcef(source, AL_GAIN, volume);
}

#ifndef __ANDROID__

ALuint KMusic::loadMP3toBuffer(const char* filePath)
{
    // Open the audio file
    CFStringRef cfString = CFStringCreateWithCString(kCFAllocatorDefault, filePath, kCFStringEncodingUTF8);
    CFURLRef audioFileURL = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, cfString, kCFURLPOSIXPathStyle, false);
    CFRelease(cfString);

    ExtAudioFileRef audioFile;
    OSStatus result = ExtAudioFileOpenURL(audioFileURL, &audioFile);
    CFRelease(audioFileURL);

    if (result != noErr)
    {
        std::cerr << "Failed to open audio file: " << filePath << std::endl;
        return 0;
    }

    // Get the file's audio format
    AudioStreamBasicDescription fileFormat;
    UInt32 size = sizeof(fileFormat);
    result = ExtAudioFileGetProperty(audioFile, kExtAudioFileProperty_FileDataFormat, &size, &fileFormat);
    if (result != noErr)
    {
        std::cerr << "Failed to get file data format" << std::endl;
        ExtAudioFileDispose(audioFile);
        return 0;
    }

    // Set the client format to 16-bit signed integer (native endian) data
    AudioStreamBasicDescription outputFormat = {0};
    outputFormat.mSampleRate = fileFormat.mSampleRate;
    outputFormat.mFormatID = kAudioFormatLinearPCM;
    outputFormat.mFormatFlags = kLinearPCMFormatFlagIsSignedInteger | kLinearPCMFormatFlagIsPacked;
    outputFormat.mBytesPerPacket = 2 * fileFormat.mChannelsPerFrame;
    outputFormat.mFramesPerPacket = 1;
    outputFormat.mBytesPerFrame = 2 * fileFormat.mChannelsPerFrame;
    outputFormat.mChannelsPerFrame = fileFormat.mChannelsPerFrame;
    outputFormat.mBitsPerChannel = 16;

    result = ExtAudioFileSetProperty(audioFile, kExtAudioFileProperty_ClientDataFormat, sizeof(outputFormat), &outputFormat);
    if (result != noErr)
    {
        std::cerr << "Failed to set client data format" << std::endl;
        ExtAudioFileDispose(audioFile);
        return 0;
    }

    // Get the total frame count
    SInt64 frameCount = 0;
    size = sizeof(frameCount);
    result = ExtAudioFileGetProperty(audioFile, kExtAudioFileProperty_FileLengthFrames, &size, &frameCount);
    if (result != noErr)
    {
        std::cerr << "Failed to get total frame count" << std::endl;
        ExtAudioFileDispose(audioFile);
        return 0;
    }

    // Prepare to read audio data
    UInt32 numFrames = static_cast<UInt32>(frameCount);
    UInt32 dataSize = numFrames * outputFormat.mBytesPerFrame;
    std::vector<char> data(dataSize);

    AudioBufferList bufferList;
    bufferList.mNumberBuffers = 1;
    bufferList.mBuffers[0].mDataByteSize = dataSize;
    bufferList.mBuffers[0].mNumberChannels = outputFormat.mChannelsPerFrame;
    bufferList.mBuffers[0].mData = data.data();

    // Read the audio data
    result = ExtAudioFileRead(audioFile, &numFrames, &bufferList);
    if (result != noErr)
    {
        std::cerr << "Failed to read audio data" << std::endl;
        ExtAudioFileDispose(audioFile);
        return 0;
    }

    ExtAudioFileDispose(audioFile);

    // Generate OpenAL buffer
    ALuint bufferID;
    alGenBuffers(1, &bufferID);
    if (alGetError() != AL_NO_ERROR)
    {
        std::cerr << "Failed to generate OpenAL buffer" << std::endl;
        return 0;
    }

    ALenum format;
    if (outputFormat.mChannelsPerFrame == 1)
    {
        format = AL_FORMAT_MONO16;
    }
    else if (outputFormat.mChannelsPerFrame == 2)
    {
        format = AL_FORMAT_STEREO16;
    }
    else
    {
        std::cerr << "Unsupported number of channels: " << outputFormat.mChannelsPerFrame << std::endl;
        alDeleteBuffers(1, &bufferID);
        return 0;
    }

    // Buffer the audio data into OpenAL
    alBufferData(bufferID, format, data.data(), static_cast<ALsizei>(dataSize), static_cast<ALsizei>(outputFormat.mSampleRate));
    if (alGetError() != AL_NO_ERROR)
    {
        std::cerr << "Failed to buffer audio data into OpenAL" << std::endl;
        alDeleteBuffers(1, &bufferID);
        return 0;
    }

    return bufferID;
}

#else

ALuint KMusic::loadMP3toBuffer(const char* filePath, AAssetManager* assetManager)
{
    // Initialize libmpg123 if not already done
    static bool mpg123_initialized = false;
    if (!mpg123_initialized) {
        if (mpg123_init() != MPG123_OK) {
            std::cerr << "Failed to initialize libmpg123." << std::endl;
            return 0;
        }
        mpg123_initialized = true;
    }

    // Create a new mpg123 handle
    mpg123_handle *mh = mpg123_new(NULL, NULL);
    if (mh == NULL) {
        std::cerr << "Failed to create mpg123 handle." << std::endl;
        return 0;
    }

    // Open the MP3 file from assets
    AAsset* asset = AAssetManager_open(assetManager, filePath, AASSET_MODE_STREAMING);
    if (!asset) {
        std::cerr << "Unable to open asset: " << filePath << std::endl;
        mpg123_delete(mh);
        return 0;
    }

    // Get the size of the asset
    off_t assetSize = AAsset_getLength(asset);
    if (assetSize <= 0) {
        std::cerr << "Asset size is invalid: " << filePath << std::endl;
        AAsset_close(asset);
        mpg123_delete(mh);
        return 0;
    }

    // Read the entire asset into a buffer
    std::vector<unsigned char> mp3Data(assetSize);
    ssize_t bytesRead = AAsset_read(asset, mp3Data.data(), assetSize);
    if (bytesRead != assetSize) {
        std::cerr << "Failed to read entire asset: " << filePath << std::endl;
        AAsset_close(asset);
        mpg123_delete(mh);
        return 0;
    }

    // Close the asset as it's no longer needed
    AAsset_close(asset);

    // Open the feed
    if (mpg123_open_feed(mh) != MPG123_OK) {
        std::cerr << "Failed to open mpg123 feed: " << mpg123_strerror(mh) << std::endl;
        mpg123_delete(mh);
        return 0;
    }

    // Submit the MP3 data to mpg123
    if (mpg123_feed(mh, mp3Data.data(), mp3Data.size()) != MPG123_OK) {
        std::cerr << "Failed to feed data to mpg123: " << mpg123_strerror(mh) << std::endl;
        // Signal EOF before deleting
        mpg123_feed(mh, NULL, 0);
        mpg123_delete(mh);
        return 0;
    }

    // **Mark the end of the data stream by feeding zero bytes**
    if (mpg123_feed(mh, NULL, 0) != MPG123_OK) {
        std::cerr << "Failed to signal EOF to mpg123: " << mpg123_strerror(mh) << std::endl;
        mpg123_delete(mh);
        return 0;
    }

    // Retrieve the output format details
    long rate;
    int channels, encoding;
    int formatres = mpg123_getformat(mh, &rate, &channels, &encoding);
    if (formatres != MPG123_OK) {
        std::cerr << "Failed to get mpg123 format: " << mpg123_strerror(mh) << std::endl;
        mpg123_delete(mh);
        return 0;
    }

    // Determine OpenAL format based on channels and encoding
    ALenum format;
    ALsizei freq = static_cast<ALsizei>(rate);

    if (channels == 1 && encoding == MPG123_ENC_SIGNED_16) {
        format = AL_FORMAT_MONO16;
    }
    else if (channels == 2 && encoding == MPG123_ENC_SIGNED_16) {
        format = AL_FORMAT_STEREO16;
    }
    else {
        std::cerr << "Unsupported MP3 format: channels=" << channels
                  << ", encoding=" << encoding << std::endl;
        mpg123_delete(mh);
        return 0;
    }

    // Decode the MP3 data into PCM
    std::vector<unsigned char> pcmData;
    const size_t bufferSize = 8192;
    std::vector<unsigned char> buffer(bufferSize);
    size_t done = 0;
    int bytesDecoded = 0; // Correct type

    while ((bytesDecoded = mpg123_read(mh, buffer.data(), buffer.size(), &done)) == MPG123_OK) {
        pcmData.insert(pcmData.end(), buffer.begin(), buffer.begin() + done);
    }

    if (bytesDecoded != MPG123_DONE) {
        //std::cerr << "Error decoding MP3: " << mpg123_strerror(mh) << std::endl;
        //mpg123_delete(mh);
        //return 0;
    }

    // Close and delete the mpg123 handle
    mpg123_close(mh);
    mpg123_delete(mh);

    // Generate OpenAL buffer
    ALuint bufferID = 0;
    alGenBuffers(1, &bufferID);
    ALenum alError = alGetError();
    if (alError != AL_NO_ERROR) {
        std::cerr << "Error generating OpenAL buffer: " << getOpenALErrorString(alError) << std::endl;
        return 0;
    }

    if (bufferID == 0) {
        std::cerr << "alGenBuffers failed to generate a valid buffer." << std::endl;
        return 0;
    }

    // Buffer the PCM data into OpenAL
    alBufferData(bufferID, format, pcmData.data(), static_cast<ALsizei>(pcmData.size()), freq);
    alError = alGetError();
    if (alError != AL_NO_ERROR) {
        std::cerr << "Error buffering data into OpenAL buffer: " << getOpenALErrorString(alError) << std::endl;
        alDeleteBuffers(1, &bufferID);
        return 0;
    }

    // Return the generated OpenAL buffer ID
    return bufferID;
}

#endif
