// KFont.h
#ifndef KFONT_H
#define KFONT_H

#include "KShader.h"
#include <iostream>
#include <map>
#include <string>
#include <locale>
#include <codecvt>
#include <ft2build.h>
#include FT_FREETYPE_H

#ifdef __ANDROID__
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#endif

struct Character {
	GLuint textureID;   // ID handle of the glyph texture
	int sizeX;          // Width of glyph
	int sizeY;          // Height of glyph
	int bearingX;       // Offset from baseline to left/top of glyph
	int bearingY;
	unsigned int advance; // Horizontal offset to advance to next glyph
};

class KFont {
	
public:
	KFont(const char* fontPath, int fontSize, float gameWidth, float gameHeight);
	~KFont();

	void RenderText(const wchar_t* text, float x, float y, float scale);
	void printGLError(const char * label);
	void setupOrthoProjection(float left, float right, float bottom, float top);

private:
	KShader *shader;
	GLuint _fonteShaderProgram;
	GLfloat orthoMatrix[16];
	float _gameW, _gameH;
	FT_Library ft;
	FT_Face face;
	std::map<char, Character> characters;
	GLuint VBO; // Only VBO, no VAO
};

#endif // KFONT_H
