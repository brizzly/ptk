#ifndef KMUSIC_H
#define KMUSIC_H

#include <Al/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include <string>
#include <vector>

class KMusic {
public:
    KMusic();
    ~KMusic();

    bool load(const std::string &filename);
    void play();
    void stop();
    void setLooping(bool loop);

private:
    ALuint buffer;
    ALuint source;
    ALCdevice *device;
    ALCcontext *context;

    std::vector<char> loadWAVFile(const std::string &filename, ALenum &format, ALsizei &freq);
};


#endif // KMUSIC_H