
#include "KGraphic.h"
#ifndef __ANDROID__
#include "GLTextureHelper.h"
#endif
#include <cmath>
#include <cstring>
#include <cstdio> // printf
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


KGraphic::~KGraphic()
{
    glDeleteBuffers(1, &vertexBuffer);
    vertexBuffer = NULL;

    glDeleteBuffers(1, &indexBuffer);
    indexBuffer = NULL;
    
    if(shader) {
        delete shader;
        shader = NULL;
    }
}


#ifdef __ANDROID__
KGraphic::KGraphic(int game_width, int game_height, int screen_width, int screen_height, AAssetManager * value)
{
    this->g_assetManager = value;
    this->init(game_width, game_height, screen_width, screen_height);
}
#else
KGraphic::KGraphic(int game_width, int game_height, int screen_width, int screen_height)
{
    this->init(game_width, game_height, screen_width, screen_height);
}
#endif

void KGraphic::init(int game_width, int game_height, int screen_width, int screen_height)
{
    _offsetX = 0;
    _offsetY = 0;
    _gameW = (float) game_width;
    _gameH = (float) game_height;
    _screenW = (float) screen_width;
    _screenH = (float) screen_height;
    
    if(_screenW == 0 || _screenH == 0) {
        printf("KGraphic init error.");
        return;
    }
    
    computOffset();
    
    const char * vertexShaderSource =
        "attribute vec4 a_position;"
        "attribute vec2 a_texCoord;"
        "varying vec2 v_texCoord;"
        "uniform mat4 u_matrix;"
        "uniform mat4 u_projectionMatrix;"
        "uniform float u_srcX;"
        "uniform float u_srcY;"
        "uniform float u_sizeW;"
        "uniform float u_sizeH;"
        "uniform float u_texWidth;"
        "uniform float u_texHeight;"
        "uniform float u_epsilon;"  // UV epsilon offset
        "void main() {"
        "    gl_Position = u_projectionMatrix * u_matrix * a_position;"

        // Calculate tex coordinates with added epsilon
        "    float texLeft = (u_srcX / u_texWidth) + u_epsilon;"
        "    float texRight = ((u_srcX + u_sizeW) / u_texWidth) - u_epsilon;"
        "    float texBottom = ((u_srcY + u_sizeH) / u_texHeight) - u_epsilon;"
        "    float texTop = (u_srcY / u_texHeight) + u_epsilon;"

        // Use mix to interpolate between the adjusted UVs based on a_texCoord
        "    v_texCoord = mix(vec2(texLeft, texTop), vec2(texRight, texBottom), a_texCoord);"
        "}";


    const char * fragmentShaderSource =
        "precision mediump float;"
        "varying vec2 v_texCoord;"
        "uniform sampler2D u_texture;"
        "void main() {"
        "    gl_FragColor = texture2D(u_texture, v_texCoord);"
        "}";
    
    shader = new KShader();
    _shaderProgram = shader->createShaderProgram(vertexShaderSource, fragmentShaderSource);
    if (!_shaderProgram) {
        printf("Shader program linking failed!\n");
    }
    printf("Shader program linked successfully! Shader Id: %d\n\n", _shaderProgram);
    
    
    //glUseProgram(_shaderProgram);
    printGLError("glUseProgram");
    
    
    positionAttribLocation = glGetAttribLocation(_shaderProgram, "a_position");
    printf("Position Attribute Location: %d\n", positionAttribLocation);
    printGLError("shader2");
    
    texCoordAttribLocation = glGetAttribLocation(_shaderProgram, "a_texCoord");
    printf("Texture Coord Attribute Location: %d\n", texCoordAttribLocation);
    printGLError("shader3");
    
    matrixUniformLocation = glGetUniformLocation(_shaderProgram, "u_matrix");
    printf("Matrix Uniform Location: %d\n", matrixUniformLocation);
    printGLError("shader4");
    
    matrixUniformProjection = glGetUniformLocation(_shaderProgram, "u_projectionMatrix");
    printf("Matrix Uniform Projection: %d\n", matrixUniformProjection);
    printGLError("shader4bis");
    
//    blendColorLocation = glGetUniformLocation(_shaderProgram, "u_blendColor");
//    printf("Blend Color Uniform Location: %d\n", blendColorLocation);
//    printGLError("shader5");
    
    textureSamplerLoc = glGetUniformLocation(_shaderProgram, "u_texture");
    printf("Texture Location: %d\n", textureSamplerLoc);
    printGLError("shader6");
    
    GLfloat epsilon = 0.00001f; // Small UV offset to avoid bleeding
    glUniform1f(glGetUniformLocation(_shaderProgram, "u_epsilon"), epsilon);

    
    // Vertex data: 2 triangles forming a rectangle (x, y positions + texture coordinates)

    GLfloat UV_EPSILON = 0;//0.001f; // Small offset to avoid sampling adjacent pixels

    GLfloat vertices[] = {
        // Positions        // UV Coordinates (adjusted by UV_EPSILON)
        -1.0f,  1.0f, 0.0f, 1.0f - UV_EPSILON,   // Top-left corner
        -1.0f, -1.0f, 0.0f, 0.0f + UV_EPSILON,   // Bottom-left corner
         1.0f, -1.0f, 1.0f - UV_EPSILON, 0.0f + UV_EPSILON,   // Bottom-right corner
         1.0f,  1.0f, 1.0f - UV_EPSILON, 1.0f - UV_EPSILON    // Top-right corner
    };
    
    GLuint indices[] = { 0, 1, 2, 0, 2, 3 };
    
    
    // Generate and bind the vertex buffer object (VBO)
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    
    printGLError("vertexBuffer");
    

 
    // Upload vertex data to the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    printGLError("GL_STATIC_DRAW");


    
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glViewport(0, 0, _screenW, _screenH);
    //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  // Dark teal background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Dark teal background
}


void KGraphic::printGLError(const char * label)
{
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        printf("OpenGL Error in %s: %x\n", label, error);
        if(error == 501) {
            printf("OpenGL Error: GL_INVALID_VALUE\n");
        }
        if(error == 502) {
            printf("OpenGL Error: GL_INVALID_OPERATION\n");
        }
    }
}


// Initialize matrix to identity
void KGraphic::setIdentityMatrix(mat4 m) {
    for (int i = 0; i < 16; ++i) {
        m[i] = (i % 5 == 0) ? 1.0f : 0.0f;
    }
}

// Translation matrix
void KGraphic::translateMatrix(mat4 m, float x, float y) {
    m[12] = x;
    m[13] = y;
}

// Scaling matrix
void KGraphic::scaleMatrix(mat4 m, float scaleX, float scaleY)
{
    // Multiply the existing scaling values with the new ones
    m[0] = scaleX;  // Scale X
    m[5] = scaleY;  // Scale Y
}

void KGraphic::rotateMatrix(mat4 m, float angle)
{
    float s = sinf(angle);
    float c = cosf(angle);

    // Backup the current translation values
    float translateX = m[12];
    float translateY = m[13];

    // Backup the current scale values
    float scaleX = sqrtf(m[0] * m[0] + m[1] * m[1]);  // Scale in X direction
    float scaleY = sqrtf(m[4] * m[4] + m[5] * m[5]);  // Scale in Y direction

    // Apply rotation while preserving the scale
    m[0] = scaleX * c;  // Cosine with scaling for the X axis
    m[1] = scaleX * -s; // -Sine with scaling for the X axis
    m[4] = scaleY * s;  // Sine with scaling for the Y axis
    m[5] = scaleY * c;  // Cosine with scaling for the Y axis

    // Restore the translation values
    m[12] = translateX;
    m[13] = translateY;
}

void KGraphic::printMatrix(mat4 m)
{
    printf("%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n\n",
           m[0], m[1], m[2], m[3],
           m[4], m[5], m[6], m[7],
           m[8], m[9], m[10], m[11],
           m[12], m[13], m[14], m[15]);
}

void KGraphic::orthographicMatrix(mat4 m, float left, float right, float bottom, float top, float nearVal, float farVal)
{
    setIdentityMatrix(m);  // Set base to identity

    // First row: scaling X by 2 / (right - left)
    m[0] = 2.0f / (right - left);

    // Second row: scaling Y by 2 / (top - bottom)
    m[5] = 2.0f / (top - bottom);

    // Third row: scaling Z by -2 / (far - near) for depth
    m[10] = -2.0f / (farVal - nearVal);

    // Translation X: shifting based on the left and right boundaries
    m[12] = -(right + left) / (right - left);

    // Translation Y: shifting based on the top and bottom boundaries
    m[13] = -(top + bottom) / (top - bottom);

    // Translation Z: shifting based on near and far planes
    m[14] = -(farVal + nearVal) / (farVal - nearVal);

    // Fix the last element for homogeneous coordinates
    m[15] = 1.0f;  // Ensure the last element is 1.0 for homogeneous coordinates
}

void KGraphic::setProjectionMatrix(int window_width, int window_height, float display_width, float display_height)
{
    float scale2 = display_width / display_height;
    float scale3 = 1.0;
    
//    if(scale2 < 1.0) {
//        scale2 = 1.0;
//        scale3 = display_width / display_height;
//    }

    mat4 orthoMatrix;
    orthographicMatrix(orthoMatrix, 0.0, 0.0 + scale2, scale3, 0.0f, -1.0f, 1.0f);
    
    GLuint matrixProjection = glGetUniformLocation(_shaderProgram, "u_projectionMatrix");
    glUniformMatrix4fv(matrixProjection, 1, GL_FALSE, orthoMatrix);
}

void KGraphic::computOffset()
{
     // Determine the orientation
     bool isPortrait = _screenH > _screenW;

     // Calculate scaling factors for width and height
     float scaleX = static_cast<float>(_screenW) / _gameW;
     float scaleY = static_cast<float>(_screenH) / _gameH;

     // Choose the scale factor based on orientation
     float scale_;
     if (isPortrait) {
         // Maximize width in portrait
         scale_ = scaleX;
     } else {
         // Maximize height in landscape
         scale_ = scaleY;
     }

    // Compute the scaled game dimensions
    _scaledGameW = static_cast<int>(_gameW * scale_);
    _scaledGameH = static_cast<int>(_gameH * scale_);

     // Calculate offsets to center the game content
    _offsetX = (_screenW - _scaledGameW) / 2;
    _offsetY = (_screenH - _scaledGameH) / 2;
   
    if(_offsetX == 0 && _offsetY < 0)
    {
         scale_ = (scaleX < scaleY) ? scaleX : scaleY;

         // Compute the scaled game dimensions
        _scaledGameW = static_cast<int>(_gameW * scale_);
        _scaledGameH = static_cast<int>(_gameH * scale_);

         // Calculate offsets to center the game content
        _offsetX = (_screenW - _scaledGameW) / 2;
        _offsetY = (_screenH - _scaledGameH) / 2;
    }
     
     /*
     // Calculate scaling factors for width and height
     float scaleX = static_cast<float>(_screenW) / _gameW;
     float scaleY = static_cast<float>(_screenH) / _gameH;

     // Choose the smaller scale to maintain aspect ratio
     float scale_ = (scaleX < scaleY) ? scaleX : scaleY;

     // Compute the scaled game dimensions
     int scaledGameW = static_cast<int>(_gameW * scale_);
     int scaledGameH = static_cast<int>(_gameH * scale_);

     // Calculate offsets to center the game content
     int offsetX = (_screenW - scaledGameW) / 2;
     int offsetY = (_screenH - scaledGameH) / 2;
     */
}

void KGraphic::render()
{
    if(_shaderProgram == 0) {
        return;
    }
    
    // PROJECTION MATRIX AS PIXELS
    float scale = 1.0f;
    setProjectionMatrix(_screenW, _screenH, _gameW, _gameH);
    
    glDisable(GL_CULL_FACE);
    //glViewport(0, 0, _screenW, _screenH);   // Viewport to cover the entire window
    //glViewport(0, 0, _gameW*2, _gameH*2);
    glViewport(_offsetX, _offsetY, _scaledGameW, _scaledGameH);
    
    /*
    float offsetX = (_screenW - _gameW*2) / 2.0f;
    if(offsetX >= 0) {
        glViewport(offsetX, 0, _gameW*2, _gameH*2);
    }
    */
    
    glUseProgram(_shaderProgram);   // Use the shader

    // Pass the texture cropping information to the shader
    glUniform1f(glGetUniformLocation(_shaderProgram, "u_srcX"), srcX);
    glUniform1f(glGetUniformLocation(_shaderProgram, "u_srcY"), srcY);
    glUniform1f(glGetUniformLocation(_shaderProgram, "u_sizeW"), sizeW);
    glUniform1f(glGetUniformLocation(_shaderProgram, "u_sizeH"), sizeH);
    glUniform1f(glGetUniformLocation(_shaderProgram, "u_texWidth"), _textureSizeW);
    glUniform1f(glGetUniformLocation(_shaderProgram, "u_texHeight"), _textureSizeH);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glEnableVertexAttribArray(positionAttribLocation);
    glEnableVertexAttribArray(texCoordAttribLocation);
    
    glVertexAttribPointer(positionAttribLocation, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glVertexAttribPointer(texCoordAttribLocation, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
    
    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glActiveTexture(GL_TEXTURE0);  // Activate texture unit 0
    glBindTexture(GL_TEXTURE_2D, _texture);  // Bind texture
    glUniform1i(textureSamplerLoc, 0);  // Set the sampler to texture unit 0
    
    
    // Define a 4x4 transformation matrix
    mat4 transform;
    setIdentityMatrix(transform);

    // REAL IMAGE POS PROJECTION
    //destX = (scale / sizeH) * destX;
    //destY = (scale / sizeH) * destY;
 //   destX = (destX / sizeH) / 2.0f;
 //   destY = (destY / sizeH) / 2.0f;
    
    // REAL IMAGE SIZE PROJECTION
    if(zoom < 0) { zoom = 0; }
    float zoomCopy = zoom;
    zoom = sizeH / _gameH;
    zoom *= 0.5;
    float sizeRatio = sizeW / sizeH;
    
    // PREVENT IMAGE TO BE DISTRORDED BY SCREEN RATIO
//    scaleMatrix(transform, sizeRatio, 1);

    // TRANSLATE IMAGE FROM ITS CENTER TO CORNER + POSITION
    float hmX = ((sizeW + destX*2.0f) / _gameH) / 2.0f;
    float hmY = ((sizeH + destY*2.0f) / _gameH) / 2.0f;
    translateMatrix(transform, hmX, hmY);
    
    
    // TRANSLATE
    //translateMatrix(transform, destX, destY);
    

    
    // ZOOM ( project real image size on screen )
    scaleMatrix(transform, zoom*sizeRatio, zoom);
    //scaleMatrix(transform, 0.5*sizeRatio, 0.5);
    
    
    // ALIGN TO CORNER
//    translateMatrix(transform, destX + 1.0*zoom*sizeRatio, destY + 1.0*zoom);
    
    // ZOOM
    scaleMatrix(transform, zoom*sizeRatio*zoomCopy, zoom*zoomCopy);

    // ROTATE
    if (angle != 0.0f) {
        rotateMatrix(transform, angle * M_PI / 180.0f);
    }

    
    // Pass matrix to shader
    GLuint matrixLocation = glGetUniformLocation(_shaderProgram, "u_matrix");
    glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, transform);
    
    // Draw it
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    printGLError("render");
}

void KGraphic::setOrientation(bool isLandscape) {
    // Update the projection matrix based on orientation
    if (isLandscape) {
        // Set landscape projection matrix
    } else {
        // Set portrait projection matrix
    }
}

// Functions added as per user request
void KGraphic::freePicture() {
    if (_imageWidth > 0) {
        _imageWidth = 0;
        glDeleteTextures(1, &_texture);
    }
}

bool KGraphic::loadPicture(const char *filename)
{
    unsigned char *data = NULL;
    int width, height, nrChannels;
    int isBGR = 1;

#ifdef __ANDROID__

    if (this->g_assetManager == nullptr) {
        printf("AssetManager not set\n");
        return false;
    }

    // Ouvrir le fichier à partir du répertoire des assets
    AAsset* asset = AAssetManager_open(g_assetManager, filename, AASSET_MODE_BUFFER);
    if (asset == nullptr) {
        printf("Failed to open asset: %s\n", filename);
        return false;
    }

    // Obtenir la taille du fichier et lire les données
    off_t assetLength = AAsset_getLength(asset);
    unsigned char* assetBuffer = new unsigned char[assetLength];
    AAsset_read(asset, assetBuffer, assetLength);
    AAsset_close(asset);

    // Utiliser stb_image pour charger l'image depuis le buffer
    data = stbi_load_from_memory(assetBuffer, assetLength, &width, &height, &nrChannels, STBI_rgb_alpha);
    delete[] assetBuffer;  // Libérer le buffer d'assets

    isBGR = 0; //detectRGBorBGR(data, width, height);

#else
    GLTextureHelper helper;
    filename = helper.loadFileDatas(filename);
    data = stbi_load(filename, &width, &height, &nrChannels, STBI_rgb_alpha);  // Load as RGBA
#endif

    if (!data) {
        printf("Failed to load texture: %s\n", filename);
        return false;
    }

    // Detect whether the image is RGB or BGR
    if (isBGR) {
        printf("Detected BGR format: Swapping red and blue channels.\n");
        // Perform channel swapping if needed (red and blue)
        for (int i = 0; i < width * height * 4; i += 4) {
            unsigned char temp = data[i];     // Swap red and blue
            data[i] = data[i + 2];
            data[i + 2] = temp;
        }
    } else {
        printf("Detected RGB format: No swap needed.\n");
    }
    
    _blitColorChanged = false;
    _imageWidth = width;
    _imageHeight = height;

    _textureSizeW = width;
    _textureSizeH = height;
    
    glDisable(GL_DEPTH_TEST);
    glGenTextures(1, &_texture);
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glActiveTexture(GL_TEXTURE0);  // Activate texture unit 0
    glBindTexture(GL_TEXTURE_2D, _texture);
    printf("texture binded to: %d\n", _texture);
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // Must use CLAMP_TO_EDGE for NPOT textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);  // Must use CLAMP_TO_EDGE for NPOT textures
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);     // Linear filtering (no mipmapping)
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);     // Magnification filter
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);     //
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);     //

    
    // Use RGBA format for texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    _eyeRetina = true; //helper.getSurfaceRetina();
    return true;
}

float KGraphic::getTextureWidth() {
    return static_cast<float>(_imageWidth);
}

float KGraphic::getTextureHeight() {
    return static_cast<float>(_imageHeight);
}

float KGraphic::getTextureSizeW() {
    return static_cast<float>(_textureSizeW);
}

float KGraphic::getTextureSizeH() {
    return static_cast<float>(_textureSizeH);
}

void KGraphic::setLineWidth(short lineWidth) {
    glLineWidth(lineWidth);
}

void KGraphic::blitAlphaRect(int x1, int y1, int x2, int y2, int destX, int destY , bool flipx , bool flipy )
{
    // Set properties for the rendering
    this->srcX = x1;
    this->srcY = y1;
    this->destX = destX;      // Destination X
    this->destY = destY;      // Destination Y
    this->sizeW = x2 - x1;
    this->sizeH = y2 - y1;
    this->angle = 0;    // Rotation angle in degrees
    this->zoom = 1.0;      // Scale (1.0 = no scaling)
    this->blend = 1.0;     // Alpha blending value (0.0 - 1.0)

    render();
}

void KGraphic::blitAlphaRectFx(int x1, int y1, int x2, int y2, int destX, int destY, float angle, float zoom, float blend , bool flipx , bool flipy )
{
    // Set properties for the rendering
    this->srcX = x1;
    this->srcY = y1;
    this->destX = destX;      // Destination X
    this->destY = destY;      // Destination Y
    this->sizeW = x2 - x1;
    this->sizeH = y2 - y1;
    this->angle = angle;    // Rotation angle in degrees
    this->zoom = zoom;      // Scale (1.0 = no scaling)
    this->blend = blend;     // Alpha blending value (0.0 - 1.0)
    
    render();
}
