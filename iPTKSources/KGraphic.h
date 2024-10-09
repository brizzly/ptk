
#ifndef KGRAPHIC_H
#define KGRAPHIC_H

#include "KShader.h"
#include <OpenGLES/ES2/gl.h>

typedef float mat4[16];

class KGraphic {
public:
    
    KGraphic(int game_width, int game_height, int screen_width, int screen_height);
    ~KGraphic();
    
    void printGLError(const char * label);
    void render();
    void setOrientation(bool isLandscape);  // Call this to switch between landscape and portrait
    
    // New functions as requested
    void freePicture();
    bool loadPicture(const char *filename);
    
    float getTextureWidth();
    float getTextureHeight();
    float getTextureSizeW();
    float getTextureSizeH();
    
    void setLineWidth(short lineWidth);
    
    void blitAlphaRect(int x1, int y1, int x2, int y2, int destX, int destY , bool flipx=false, bool flipy=false);
    void blitAlphaRectFx(int x1, int y1, int x2, int y2, int destX, int destY, float angle, float zoom, float blend , bool flipx=false, bool flipy=false );


public:
    
    float srcX, srcY, destX, destY, sizeW, sizeH, angle, zoom, blend;


private:
    float _imageWidth;
    float _imageHeight;
    float _textureSizeW;
    float _textureSizeH;
    float _screenW;
    float _screenH;
    float _gameW;
    float _gameH;
    bool _blitColorChanged;
    bool _textureWrap;
    bool _eyeRetina;
    GLuint _shaderProgram;
    GLuint positionAttribLocation, texCoordAttribLocation, matrixUniformLocation, matrixUniformProjection;
    GLuint vertexBuffer, indexBuffer, textureSamplerLoc;
    //GLuint textureID;
    GLfloat projectionMatrix[16];
    GLuint blendColorLocation;
    KShader * shader;
    GLuint _texture;
    


    // Matrix helper functions
//    void identityMatrix(float* matrix);
//    void translateMatrix(float* matrix, float x, float y, float z);
//    void rotateMatrix(float* matrix, float angle, float x, float y, float z);
//    void scaleMatrix(float* matrix, float scaleX, float scaleY, float scaleZ);
    
    void printMatrix(mat4 m);
    
    void orthographicMatrix(mat4 m, float left, float right, float bottom, float top, float nearVal, float farVal);

    void setProjectionMatrix(int window_width, int window_height, float display_width, float display_height);
    void setIdentityMatrix(mat4 m);

    // Translation matrix
    void translateMatrix(mat4 m, float x, float y);
 
    // Scaling matrix
    void scaleMatrix(mat4 m, float scaleX, float scaleY);

    // Rotation matrix (angle in radians)
    void rotateMatrix(mat4 m, float angle);
        
        
};


#endif
