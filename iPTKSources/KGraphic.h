// KGraphic.h
#ifndef KGRAPHIC_H
#define KGRAPHIC_H

#include "KShader.h"
#include <string>
#ifdef __ANDROID__
#include <GLES2/gl2.h>
#include <android/asset_manager_jni.h>
#else
#define GLES_SILENCE_DEPRECATION
#include <OpenGLES/ES2/gl.h>
#endif

typedef float mat4[16];
typedef float vec2[2];

class KGraphic {

private:
    
#ifdef __ANDROID__
    AAssetManager* g_assetManager = nullptr;
#endif

public:


#ifdef __ANDROID__
    KGraphic(int game_width, int game_height, int screen_width, int screen_height, AAssetManager * value);
#else
    KGraphic(int game_width, int game_height, int screen_width, int screen_height);
#endif
    ~KGraphic();

    void init(int game_width, int game_height, int screen_width, int screen_height);
    GLuint createQuadVBO();
    void setBackgroundColor(float r, float g, float b);
    void setDrawBounds(bool value);
    void printGLError(const char * label);
    void setOrientation(bool isLandscape);  // Call this to switch between landscape and portrait

    // New functions as requested
    void freePicture();
    bool loadPicture(const char *filename);

    float getTextureWidth();
    float getTextureHeight();
    float getTextureSizeW();
    float getTextureSizeH();
    bool isRetina();
    void hackForLightfields(); // clean this.

    void draw(int x, int y, float angle, float zoom, float blend);
    void drawEx(int x1, int y1, int x2, int y2, int destX, int destY, float angle = 0.0, float zoom = 1.0, float blend = 1.0);

    void setLineWidth(short lineWidth);
    void drawLine(float x1, float y1, float x2, float y2, float r, float g, float b, float a, float linewidth=1.0);
    void drawLine_Offscreen(float x1, float y1, float x2, float y2, float r, float g, float b, float a, float linewidth=1.0);
    void drawShape(int numvertices, vec2* vertice, int destX, int destY, float linewidth, float r, float g, float b, float a);
    void drawSolidRectangle(float x, float y, float width, float height, float r, float g, float b, float a);
    
    // --- New methods for render-to-texture functionality ---
    void clearRenderTexture(float r, float g, float b, float a);
    void createRenderTexture(int width, int height);
    void beginRenderToTexture();
    void endRenderToTexture();
    void drawRenderTexture(int destX, int destY, float angle, float zoom, float blend);
    void drawOffscreenTexture(float tx, float ty, float scaleX, float scaleY);
    GLuint getFbo();
    GLuint getFboTexture();
    int getFboWidth();
    int getFboHeight();

    
public:

    float srcX, srcY, destX, destY, sizeW, sizeH, angle, zoom, blend;
    float line_centerX, line_centerY;
    float shape_centerX, shape_centerY;
    
    
private:

    float _imageWidth;
    float _imageHeight;
    float _textureSizeW;
    float _textureSizeH;
    float _screenW;
    float _screenH;
    float _gameW;
    float _gameH;
    float _offsetX;
    float _offsetY;
    float _scaledGameW;
    float _scaledGameH;
    bool _blitColorChanged;
    bool _textureWrap;
    bool _eyeRetina;
    GLuint _shaderProgram;
    GLuint _lineShaderProgram;
    GLuint _shapeShaderProgram;
    GLuint _solidShaderProgram;
    GLuint _simpleProgram;
    bool _drawBoundings;
    
    GLuint positionAttribLocation, texCoordAttribLocation, matrixUniformLocation, matrixUniformProjection;
    GLuint vertexBuffer_Solid, vertexBuffer_Line, vertexBuffer, indexBuffer, indexBuffer_Solid, textureSamplerLoc, opacityLoc;
    
    GLfloat projectionMatrix[16];
    GLuint blendColorLocation;
    KShader * shader;
    GLuint _texture;
    
    float img_src_x;
    float img_src_y;
    float img_dst_x;
    float img_dst_y;
    
    float background_r, background_g, background_b = 0.0f;
    
    // --- New member variables ---
    GLuint quadVBO;
    GLuint _fbo;          // Framebuffer object for offscreen rendering
    GLuint _fboTexture;   // Texture attached to the FBO
    int _fboWidth, _fboHeight; // Dimensions of the render texture
    
    

    void render();
    void computOffset();
    
    void printMatrix(mat4 m);
    void setOrthographicProjection(mat4& m, float left, float right, float bottom, float top);
    void setIdentityMatrix(mat4 m);
    void translateMatrix(mat4 m, float x, float y);
    void scaleMatrix(mat4 m, float scaleX, float scaleY);
    void scaleMatrix2(mat4 m, float scaleX, float scaleY);
    void applyZoom(mat4& modelViewMatrix, float zoom, float modelWidth, float modelHeight);
    void rotateMatrix(mat4 m, float angle);
    void copyMatrix(mat4 dest, const mat4 src);
    void multiplyMatrices(const mat4 a, const mat4 b, mat4 result);

};

#endif
