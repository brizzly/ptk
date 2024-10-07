
#ifndef KSHADER_H
#define KSHADER_H

#include <OpenGLES/ES2/gl.h>

class KShader {
public:
    
    KShader();

    GLuint compileShader(GLenum type, const char* source);
    GLuint createShaderProgram(const char* vertexSource, const char* fragmentSource);
    
    
//public:
//    
//    const char* vertexShaderSource;
//    const char* fragmentShaderSource;
//    
    
private:
    

};


#endif
