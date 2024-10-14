#include "KMusic.h"
#include "OpenALManager.h"
#include <iostream>
#include <vector>
#include <string>


KMusic::KMusic()
{
    isLooping = false;
    
    // Obtain the shared OpenALManager instance
    OpenALManager& alManager = OpenALManager::getInstance();

    alGenSources(1, &source);
    ALenum error = alGetError();
    if (error != AL_NO_ERROR) {
        //std::cerr << "Error generating OpenAL source: " << getOpenALErrorString(error) << std::endl;
    }
}

KMusic::~KMusic()
{
    // Stop playback
    stop();

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

void KMusic::playMusic()
{
    alSourcePlay(source);
    if (alGetError() != AL_NO_ERROR)
    {
        std::cerr << "Failed to play source" << std::endl;
        // Handle error appropriately
    }
}

void KMusic::stop()
{
    alSourceStop(source);
    if (alGetError() != AL_NO_ERROR)
    {
        std::cerr << "Failed to stop source" << std::endl;
        // Handle error appropriately
    }
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

ALuint KMusic::loadMP3toBuffer(const char* filePath)
{
#ifndef __ANDROID__
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
#else
    return 0;
#endif
}
