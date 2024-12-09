// KFont.h
#ifndef KFONT_H
#define KFONT_H

#include "KShader.h"
#include <iostream>
#include <sstream>
#include <map>
#include <string>
#include <locale>
#include <codecvt>

#ifdef __ANDROID__
#include <android/asset_manager_jni.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
//#include "ft2build.h"
#import "ft2/ft2build.h"
#include FT_FREETYPE_H
#else
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#import <Freetype2/ft2build.h>
#include FT_FREETYPE_H
#endif

#define KFONT_SIZE  360.0

struct Character {
	GLuint textureID;   // ID handle of the glyph texture
	int sizeX;          // Width of glyph
	int sizeY;          // Height of glyph
	int bearingX;       // Offset from baseline to left/top of glyph
	int bearingY;
	unsigned int advance; // Horizontal offset to advance to next glyph
	unsigned int advance2; // Vertical offset to advance to next glyph line
};

class KFont {
	
public:
#ifdef __ANDROID__
    KFont(const char* fontPath, float gameWidth, float gameHeight, AAssetManager * value);
#else
	KFont(const char* fontPath, float gameWidth, float gameHeight);
#endif

    void init(const char* fontPath, float gameWidth, float gameHeight);
	~KFont();
	void SetMaxCharBeforeLine(int value);
	void UnsetMaxCharBeforeLine();
    void SetBackgroundColor(float r, float g, float b, float a);
    void SetTextColor(float r, float g, float b);
	void RenderText(const wchar_t* text, float x, float y, float scale);
	void RenderTextCenteredButton(const wchar_t* text, float posx, float posy, float btn_width, float scale);
    void RenderTextCentered(const wchar_t* text, float posy, float offset_x, float scale);
	void measure_text(const wchar_t* text, float* width, float* height, float scale);
	void printGLError(const char * label);
	void setupOrthoProjection(float left, float right, float bottom, float top);

private:
	KShader *shader;
	GLuint _fonteShaderProgram;
	GLfloat orthoMatrix[16];
	float _gameW, _gameH;
	int _fontSize;
    float back_R, back_G, back_B, back_A;
    float text_R, text_G, text_B;
	FT_Library ft;
	FT_Face face;
	std::map<char, Character> characters;
	GLuint VBO; // Only VBO, no VAO
	int _maxCharsBeforeNewLine;

#ifdef __ANDROID__
    AAssetManager* g_assetManager = nullptr;
#endif
};

#endif // KFONT_H
