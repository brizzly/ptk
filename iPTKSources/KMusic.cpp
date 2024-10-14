#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "KMusic.h"


KMusic::KMusic() {
    
}

KMusic::~KMusic() {

}

bool KMusic::load(const std::string &filename) {

    return true;
}

void KMusic::playMusic(char *filename) {
   
}

void KMusic::stop() {
    
}

void KMusic::setLooping(bool loop) {
    
}

char * KMusic::loadMP3File(const std::string &filename, ALenum &format, ALsizei &freq) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return NULL;
    }

    return NULL;
}
