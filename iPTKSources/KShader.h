
#ifndef KSHADER_H
#define KSHADER_H

#ifdef __ANDROID__
#include <GLES2/gl2.h>
#else
#define GLES_SILENCE_DEPRECATION
#include <OpenGLES/ES2/gl.h>
#endif


class KShader {
public:
    
    KShader();

    GLuint compileShader(GLenum type, const char* source);
    GLuint createShaderProgram(const char* vertexSource, const char* fragmentSource);
	GLuint createShader();
	GLuint createLineShader();
	GLuint createFonteShader();

    
private:
    

};


#endif
