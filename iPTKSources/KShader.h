
#ifndef KSHADER_H
#define KSHADER_H

#ifdef __ANDROID__
#include <GLES2/gl2.h>
#elif defined(USE_METAL)
// Provide GL typedefs only if OpenGL headers weren't already included (e.g. by glfm.h)
#  if !defined(__gl_es20_h_) && !defined(__gles2_gl2_h_)
typedef unsigned int GLuint;
typedef float GLfloat;
typedef int GLint;
typedef unsigned int GLenum;
typedef unsigned char GLubyte;
#  endif
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
    GLuint createSolidColorShader();
    GLuint createSimpleProgram();

    
private:
    

};


#endif
