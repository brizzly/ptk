// KGraphic.cpp
#include "KGraphic.h"
#ifndef __ANDROID__
#include "GLTextureHelper.h"
#endif
#include <cmath>
#include <cstring>
#include <cstdio> // printf
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

KGraphic::~KGraphic()
{
    glDeleteBuffers(1, &vertexBuffer);
    vertexBuffer = 0;
    
    glDeleteBuffers(1, &vertexBuffer_Line);
    vertexBuffer_Line = 0;

    glDeleteBuffers(1, &vertexBuffer_Solid);
    vertexBuffer_Solid = 0;
    
    glDeleteBuffers(1, &indexBuffer);
    indexBuffer = 0;
    
    if(shader) {
        delete shader;
        shader = nullptr;
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
    img_src_x = 0;
    img_src_y = 0;
    img_dst_x = 0;
    img_dst_y = 0;
    
    _fboWidth = 0;
    _fboHeight = 0;
    
    _offsetX = 0;
    _offsetY = 0;
    line_centerX = line_centerY = 0;
    shape_centerX = shape_centerY = 0;
    _gameW = (float) game_width;
    _gameH = (float) game_height;
    _screenW = (float) screen_width;
    _screenH = (float) screen_height;
    _drawBoundings = false;

    if (_screenW == 0 || _screenH == 0) {
        printf("[ERROR] KGraphic init error: Invalid screen dimensions.\n");
        return;
    }

    computOffset();

    shader = new KShader();
    _shaderProgram = shader->createShader();
    if (!_shaderProgram) {
        printf("[ERROR] Shader program linking failed!\n");
        return;
    }
    //printf("[DEBUG] Shader program linked successfully! Shader ID: %d\n", _shaderProgram);
    
    glUseProgram(_shaderProgram);
    printGLError("glUseProgram");

    // Retrieve attribute locations
    positionAttribLocation = glGetAttribLocation(_shaderProgram, "a_position");
    if (positionAttribLocation == -1) {
        printf("[ERROR] Failed to get 'a_position' location!\n");
    }
    printGLError("shader - a_position");

    texCoordAttribLocation = glGetAttribLocation(_shaderProgram, "a_texCoord");
    if (texCoordAttribLocation == -1) {
        printf("[ERROR] Failed to get 'a_texCoord' location!\n");
    }
    printGLError("shader - a_texCoord");

    matrixUniformLocation = glGetUniformLocation(_shaderProgram, "u_matrix");
    if (matrixUniformLocation == -1) {
        printf("[ERROR] Failed to get 'u_matrix' location!\n");
    }
    printGLError("shader - u_matrix");

    matrixUniformProjection = glGetUniformLocation(_shaderProgram, "u_projectionMatrix");
    if (matrixUniformProjection == -1) {
        printf("[ERROR] Failed to get 'u_projectionMatrix' location!\n");
    }
    printGLError("shader - u_projectionMatrix");

    opacityLoc = glGetUniformLocation(_shaderProgram, "u_opacity");
    if (opacityLoc == -1) {
        printf("[ERROR] Failed to get 'u_opacity' location!\n");
    }
    printGLError("shader - u_opacity");

    textureSamplerLoc = glGetUniformLocation(_shaderProgram, "u_texture");
    if (textureSamplerLoc == -1) {
        printf("[ERROR] Failed to get 'u_texture' location!\n");
    }
    printGLError("shader - u_texture");

    // Set default uniforms
    GLfloat epsilon = 0.00001f;
    glUniform1f(glGetUniformLocation(_shaderProgram, "u_epsilon"), epsilon);

    // Define vertex and index data
    GLfloat vertices[] = {
        // Positions          // UV Coordinates
        -0.5f, -0.5f,          0.0f, 0.0f,   // Bottom-left corner
         0.5f, -0.5f,          1.0f, 0.0f,   // Bottom-right corner
         0.5f,  0.5f,          1.0f, 1.0f,   // Top-right corner
        -0.5f,  0.5f,          0.0f, 1.0f    // Top-left corner
    };

    GLuint indices[] = { 0, 1, 2, 2, 3, 0 };

    // Generate and bind vertex buffer
    glGenBuffers(1, &vertexBuffer);
    if (vertexBuffer == 0) {
        printf("[ERROR] Failed to generate vertex buffer!\n");
    }
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    printGLError("vertexBuffer - glBindBuffer");

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    printGLError("vertexBuffer - glBufferData");

    // Generate and bind index buffer
    glGenBuffers(1, &indexBuffer);
    if (indexBuffer == 0) {
        printf("[ERROR] Failed to generate index buffer!\n");
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    printGLError("indexBuffer - glBindBuffer");

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    printGLError("indexBuffer - glBufferData");

    // Set viewport
    glViewport(0, 0, _screenW, _screenH);
    glClearColor(background_r, background_g, background_b, 1.0f);

    // Generate buffers for lines, shapes, and solid rectangles
    glGenBuffers(1, &vertexBuffer_Line);
    if (vertexBuffer_Line == 0) {
        printf("[ERROR] Failed to generate vertexBuffer_Line!\n");
    }
    printGLError("vertexBuffer_Line - glGenBuffers");

    glGenBuffers(1, &vertexBuffer_Solid);
    if (vertexBuffer_Solid == 0) {
        printf("[ERROR] Failed to generate vertexBuffer_Solid!\n");
    }
    printGLError("vertexBuffer_Solid - glGenBuffers");

    // Create shader programs for different rendering types
    _lineShaderProgram = shader->createLineShader();
    if (!_lineShaderProgram) {
        printf("[ERROR] Line Shader program creation failed!\n");
    }
    
    _shapeShaderProgram = shader->createLineShader();
    if (!_shapeShaderProgram) {
        printf("[ERROR] Shape Shader program creation failed!\n");
    }

    _solidShaderProgram = shader->createSolidColorShader();
    if (!_solidShaderProgram) {
        printf("[ERROR] Solid Shader program creation failed!\n");
    }
    
    _simpleProgram = shader->createSimpleProgram();
    if (!_simpleProgram) {
        printf("[ERROR] Simple Shader program creation failed!\n");
    }
    
    quadVBO = createQuadVBO();

    //printf("[DEBUG] Initialization complete!\n");
}

// Creates a vertex buffer object for a fullscreen quad.
GLuint KGraphic::createQuadVBO()
{
    GLuint vbo;
    glGenBuffers(1, &vbo);
    GLfloat quadVertices[] = {
        // Positions   // TexCoords
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f,  1.0f,  1.0f, 1.0f,
    };
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return vbo;
}

void KGraphic::setBackgroundColor(float r, float g, float b)
{
    background_r = r;
    background_g = g;
    background_b = b;
}

void KGraphic::setDrawBounds(bool value)
{
    _drawBoundings = value;
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

void KGraphic::setLineWidth(short lineWidth) {
    glLineWidth(lineWidth);
}

void KGraphic::drawLine(float x1, float y1, float x2, float y2, float r, float g, float b, float a, float linewidth)
{
    if (_lineShaderProgram == 0) {
        printf("Error: Shader program is invalid.\n");
        return;
    }
    
    glUseProgram(_lineShaderProgram);

    // Set Orthographic Projection
    setOrthographicProjection(projectionMatrix, 0.0f, _gameW, 0.0f, _gameH);
    
    GLuint matrixProjection = glGetUniformLocation(_lineShaderProgram, "u_projectionMatrix");
    if (matrixProjection != -1) {
        glUniformMatrix4fv(matrixProjection, 1, GL_FALSE, projectionMatrix);
    }
    
    glDisable(GL_CULL_FACE);
    glViewport(_offsetX, _offsetY, _scaledGameW, _scaledGameH);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
     
    // Create model matrix and apply rotation
    mat4 modelMatrix;
    setIdentityMatrix(modelMatrix);
    
    translateMatrix(modelMatrix, line_centerX, line_centerY);

    // Apply rotation if angle is non-zero
    if (angle != 0.0f) {
        rotateMatrix(modelMatrix, angle * M_PI / 180.0f); // Convert angle to radians
    }

    translateMatrix(modelMatrix, -line_centerX, -line_centerY);

    GLuint matrixLocation = glGetUniformLocation(_lineShaderProgram, "u_matrix");
    if (matrixLocation != -1) {
        glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, modelMatrix);
    }
     
    GLfloat lineCoords[] = {
        x1, y1,
        x2, y2
    };
    
    float color[] = { r, g, b, a };

    glLineWidth(linewidth);
    
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_Line);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lineCoords), lineCoords, GL_DYNAMIC_DRAW);

    GLint posAttrib = glGetAttribLocation(_lineShaderProgram, "a_position");
    if (posAttrib == -1) {
        printf("Error: Position attribute not found in shader.\n");
        return;
    }
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    
    GLint colorUniform = glGetUniformLocation(_lineShaderProgram, "color");
    if (colorUniform == -1) {
        printf("Error: Color uniform not found in shader.\n");
        return;
    }
    glUniform4fv(colorUniform, 1, color);

    glDrawArrays(GL_LINES, 0, 2);
    
    glDisableVertexAttribArray(posAttrib);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);
    
    printGLError("line");
}

void KGraphic::setIdentityMatrix(mat4 m) {
    for (int i = 0; i < 16; ++i) {
        m[i] = (i % 5 == 0) ? 1.0f : 0.0f;
    }
}

void KGraphic::translateMatrix(mat4 m, float x, float y)
{
    mat4 translation;
    setIdentityMatrix(translation);
    translation[12] = x;
    translation[13] = y;

    // Multiply translation * m
    mat4 result;
    multiplyMatrices(translation, m, result);
    copyMatrix(m, result);
}

void KGraphic::scaleMatrix(mat4 m, float scaleX, float scaleY)
{
    mat4 scaling;
    setIdentityMatrix(scaling);
    scaling[0] = scaleX;
    scaling[5] = scaleY;

    // Multiply scaling * m
    mat4 result;
    multiplyMatrices(scaling, m, result);
    copyMatrix(m, result);
}

void KGraphic::scaleMatrix2(mat4 m, float scaleX, float scaleY)
{
    float centerX = 0.0f;
    float centerY = 0.0f;

    mat4 translationToCenter;
    setIdentityMatrix(translationToCenter);
    translationToCenter[12] = -centerX;
    translationToCenter[13] = -centerY;

    mat4 scaling;
    setIdentityMatrix(scaling);
    scaling[0] = scaleX;
    scaling[5] = scaleY;

    mat4 translationBack;
    setIdentityMatrix(translationBack);
    translationBack[12] = centerX;
    translationBack[13] = centerY;

    mat4 temp1, temp2;
    multiplyMatrices(m, translationToCenter, temp1);
    multiplyMatrices(temp1, scaling, temp2);
    multiplyMatrices(temp2, translationBack, temp1);
    copyMatrix(m, temp1);
}

void KGraphic::applyZoom(mat4& modelViewMatrix, float zoom, float modelWidth, float modelHeight)
{
    float centerX = modelWidth / 2.0f;
    float centerY = modelHeight / 2.0f;
    scaleMatrix2(modelViewMatrix, zoom, zoom);
}

void KGraphic::rotateMatrix(mat4 m, float angle)
{
    float s = sinf(angle);
    float c = cosf(angle);

    mat4 rotation;
    setIdentityMatrix(rotation);
    rotation[0] = c;
    rotation[1] = -s;
    rotation[4] = s;
    rotation[5] = c;

    mat4 result;
    multiplyMatrices(rotation, m, result);
    copyMatrix(m, result);
}

void KGraphic::multiplyMatrices(const mat4 a, const mat4 b, mat4 result)
{
    for(int row = 0; row < 4; ++row){
        for(int col = 0; col < 4; ++col){
            result[row * 4 + col] = 0.0f;
            for(int k = 0; k < 4; ++k){
                result[row * 4 + col] += a[row * 4 + k] * b[k * 4 + col];
            }
        }
    }
}

void KGraphic::copyMatrix(mat4 dest, const mat4 src)
{
    for(int i = 0; i < 16; ++i){
        dest[i] = src[i];
    }
}

void KGraphic::setOrthographicProjection(mat4& m, float left, float right, float bottom, float top)
{
    setIdentityMatrix(m);

    m[0] =  2.0f / (right - left);
    m[5] = -2.0f / (top - bottom);
    m[10] = -1.0f;
    m[12] = -(right + left) / (right - left);
    m[13] =  (top + bottom) / (top - bottom);
    m[15] = 1.0f;
}

void KGraphic::computOffset()
{
    //printf("[DEBUG] computOffset - Incoming Values: _gameW=%f, _gameH=%f, _screenW=%f, _screenH=%f\n",
       //    _gameW, _gameH, _screenW, _screenH);

    if (_gameW <= 0 || _gameH <= 0 || _screenW <= 0 || _screenH <= 0) {
        printf("[ERROR] Invalid game or screen dimensions! Skipping computOffset().\n");
        return;
    }

    bool isPortrait = _screenH > _screenW;

    float scaleX = _screenW / _gameW;
    float scaleY = _screenH / _gameH;
    //printf("[DEBUG] computOffset - scaleX=%f, scaleY=%f\n", scaleX, scaleY);

    float scale_;
    if (isPortrait) {
        scale_ = scaleX;
    } else {
        scale_ = scaleY;
    }

    _scaledGameW = static_cast<int>(_gameW * scale_);
    _scaledGameH = static_cast<int>(_gameH * scale_);

    //printf("[DEBUG] computOffset - Computed Scale: scale_=%f, _scaledGameW=%f, _scaledGameH=%f\n",
     //      scale_, _scaledGameW, _scaledGameH);

    if (_scaledGameW == 0 || _scaledGameH == 0) {
        printf("[ERROR] _scaledGameW or _scaledGameH is 0! Check calculations.\n");
        return;
    }

    _offsetX = (_screenW - _scaledGameW) / 2;
    _offsetY = (_screenH - _scaledGameH) / 2;

    //printf("[DEBUG] computOffset - Initial Offset Calculation: OffsetX=%d, OffsetY=%d\n",
      //     _offsetX, _offsetY);

    if (_offsetX == 0 && _offsetY < 0)
    {
        printf("[DEBUG] computOffset - Applying Offset Correction!\n");

        scale_ = (scaleX < scaleY) ? scaleX : scaleY;

        _scaledGameW = static_cast<int>(_gameW * scale_);
        _scaledGameH = static_cast<int>(_gameH * scale_);

        _offsetX = (_screenW - _scaledGameW) / 2;
        _offsetY = (_screenH - _scaledGameH) / 2;

        //printf("[DEBUG] computOffset - Corrected Scale: scale_=%f\n", scale_);
        //printf("[DEBUG] computOffset - Corrected Offset: OffsetX=%d, OffsetY=%d\n",
          //     _offsetX, _offsetY);
    }

    //printf("[DEBUG] computOffset - Final Scaled Game: %dx%d, Offset: (%d, %d)\n",
      //     _scaledGameW, _scaledGameH, _offsetX, _offsetY);
}

void KGraphic::render()
{
    if (_shaderProgram == 0) {
        return;
    }
    
    glUseProgram(_shaderProgram);
    
    setOrthographicProjection(projectionMatrix, 0.0f, _gameW, 0.0f, _gameH);

    GLuint matrixProjection = glGetUniformLocation(_shaderProgram, "u_projectionMatrix");
    if (matrixProjection != -1) {
        glUniformMatrix4fv(matrixProjection, 1, GL_FALSE, projectionMatrix);
    }

    mat4 modelViewMatrix;
    setIdentityMatrix(modelViewMatrix);

    translateMatrix(modelViewMatrix, destX + (sizeW ) / 2.0f, destY + (sizeH ) / 2.0f);
    scaleMatrix(modelViewMatrix, sizeW * zoom, sizeH * zoom);
    
    if (angle != 0.0f) {
        rotateMatrix(modelViewMatrix, angle * M_PI / 180.0f);
    }
    
    GLuint matrixLocation = glGetUniformLocation(_shaderProgram, "u_matrix");
    glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, modelViewMatrix);

    glDisable(GL_CULL_FACE);
    glViewport(_offsetX, _offsetY, _scaledGameW, _scaledGameH);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUniform1f(glGetUniformLocation(_shaderProgram, "u_srcX"), srcX);
    glUniform1f(glGetUniformLocation(_shaderProgram, "u_srcY"), srcY);
    glUniform1f(glGetUniformLocation(_shaderProgram, "u_sizeW"), sizeW);
    glUniform1f(glGetUniformLocation(_shaderProgram, "u_sizeH"), sizeH);
    glUniform1f(glGetUniformLocation(_shaderProgram, "u_texWidth"), _textureSizeW);
    glUniform1f(glGetUniformLocation(_shaderProgram, "u_texHeight"), _textureSizeH);
    glUniform1f(glGetUniformLocation(_shaderProgram, "u_opacity"), blend);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glEnableVertexAttribArray(positionAttribLocation);
    glEnableVertexAttribArray(texCoordAttribLocation);
    glVertexAttribPointer(positionAttribLocation, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glVertexAttribPointer(texCoordAttribLocation, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texture);
    glUniform1i(textureSamplerLoc, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    printGLError("render");
    
    if (_drawBoundings) {
        vec2 vertice[] = {
            {-sizeW * zoom / 2.0f, -sizeH * zoom / 2.0f},
            { sizeW * zoom / 2.0f, -sizeH * zoom / 2.0f},
            { sizeW * zoom / 2.0f,  sizeH * zoom / 2.0f},
            {-sizeW * zoom / 2.0f,  sizeH * zoom / 2.0f}
        };

        float lineW = 4.0;
        shape_centerX = 0;
        shape_centerY = 0;
        drawShape(4, vertice, destX + (sizeW ) / 2.0f, destY + (sizeH ) / 2.0f, lineW, 1, 0, 0, 1);
    }
}

void KGraphic::setOrientation(bool isLandscape) {
}

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
    _eyeRetina = false;

#ifdef __ANDROID__
    if (this->g_assetManager == nullptr) {
        printf("AssetManager not set\n");
        return false;
    }

    bool deviceSupportsRetina = !false;
    const char* retinaSuffix = "@2x";
    std::string filenameStr(filename);
    std::string filenameToLoad = filenameStr;

    if (!deviceSupportsRetina) {
        size_t pos = filenameStr.find(retinaSuffix);
        if (pos != std::string::npos) {
            filenameStr.erase(pos, strlen(retinaSuffix));
        }
        filenameToLoad = filenameStr;
    }

    AAsset* asset = AAssetManager_open(g_assetManager, filenameToLoad.c_str(), AASSET_MODE_BUFFER);
    if (!asset) {
        printf("Failed to open asset: %s\n", filenameToLoad.c_str());
        return false;
    }

    off_t assetLength = AAsset_getLength(asset);
    unsigned char* assetBuffer = new unsigned char[assetLength];
    off_t bytesRead = 0;

    while (bytesRead < assetLength) {
        int result = AAsset_read(asset, assetBuffer + bytesRead, assetLength - bytesRead);
        if (result <= 0) {
            printf("Failed to read asset completely. Bytes read: %ld\n", bytesRead);
            delete[] assetBuffer;
            AAsset_close(asset);
            return false;
        }
        bytesRead += result;
    }
    AAsset_close(asset);

    unsigned char* rawData = stbi_load_from_memory(assetBuffer, assetLength, &width, &height, &nrChannels, 0);

    if (!rawData) {
        printf("Failed to load image: %s\n", filenameToLoad.c_str());
        delete[] assetBuffer;
        return false;
    }

    if (nrChannels == 3) {
        printf("Image has 3 channels (RGB), adding alpha channel.\n");
        unsigned char* rgbaData = new unsigned char[width * height * 4];
        for (int i = 0; i < width * height; i++) {
            rgbaData[i * 4 + 0] = rawData[i * 3 + 0];
            rgbaData[i * 4 + 1] = rawData[i * 3 + 1];
            rgbaData[i * 4 + 2] = rawData[i * 3 + 2];
            rgbaData[i * 4 + 3] = 255;
        }
        stbi_image_free(rawData);
        rawData = rgbaData;
        nrChannels = 4;
    }

    delete[] assetBuffer;

    if (rawData == nullptr) {
        printf("Failed to load image: %s\n", filenameToLoad.c_str());
        return false;
    }

    printf("Image loaded successfully: %dx%d, channels: %d\n", width, height, nrChannels);

    if (!deviceSupportsRetina && filenameStr.find("@2x") != std::string::npos) {
        int newWidth = std::round(width / 2.0);
        int newHeight = std::round(height / 2.0);
        unsigned char* scaledData = new unsigned char[newWidth * newHeight * 4];

        if (stbir_resize_uint8(rawData, width, height, 0, scaledData, newWidth, newHeight, 0, 4)) {
            stbi_image_free(rawData);
            rawData = scaledData;
            width = newWidth;
            height = newHeight;
        } else {
            printf("Failed to resize image.\n");
            stbi_image_free(rawData);
            delete[] scaledData;
            return false;
        }
    }

    if (deviceSupportsRetina && filenameStr.find("@2x") == std::string::npos) {
        int newWidth = std::round(width * 2.0);
        int newHeight = std::round(height * 2.0);
        unsigned char* scaledData = new unsigned char[newWidth * newHeight * 4];

        if (stbir_resize_uint8(rawData, width, height, 0, scaledData, newWidth, newHeight, 0, 4)) {
            stbi_image_free(rawData);
            rawData = scaledData;
            width = newWidth;
            height = newHeight;
        } else {
            printf("Failed to resize image.\n");
            stbi_image_free(rawData);
            delete[] scaledData;
            return false;
        }
    }

    data = rawData;
    isBGR = 0;
    _eyeRetina = deviceSupportsRetina;

#else
    GLTextureHelper helper;
    std::string modifiedFilename(filename);

    size_t dotPosition = modifiedFilename.find_last_of(".");
    if (dotPosition != std::string::npos) {
        _eyeRetina = true;
        modifiedFilename.insert(dotPosition, "@2x");
    }

    const char* retinaFilename = helper.loadFileDatas(modifiedFilename.c_str());
    data = stbi_load(retinaFilename, &width, &height, &nrChannels, STBI_rgb_alpha);

    if (!data) {
        printf("Failed to load @2x texture, trying standard version: %s\n", filename);

        _eyeRetina = false;

        retinaFilename = helper.loadFileDatas(filename);
        data = stbi_load(retinaFilename, &width, &height, &nrChannels, STBI_rgb_alpha);
    }
#endif

    if (!data) {
        printf("Failed to load texture: %s\n", filename);
        return false;
    }

    _textureSizeW = width;
    _textureSizeH = height;

    if (_eyeRetina) {
        printf("Retina texture detected: Adjusting logical dimensions.\n");
        _textureSizeW = width / 2;
        _textureSizeH = height / 2;
    }

    if (isBGR) {
        printf("Detected BGR format: Swapping red and blue channels.\n");
        for (int i = 0; i < width * height * 4; i += 4) {
            unsigned char temp = data[i];
            data[i] = data[i + 2];
            data[i + 2] = temp;
        }
    } else {
        printf("Detected RGB format: No swap needed.\n");
    }

    glDisable(GL_DEPTH_TEST);
    glGenTextures(1, &_texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texture);
    printf("Texture bound to: %d\n", _texture);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);

    return true;
}

void KGraphic::hackForLightfields()
{
#ifdef __ANDROID__
    _textureSizeW /= 2;
    _textureSizeH /= 2;
#endif
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

bool KGraphic::isRetina() {
    return _eyeRetina;
}

void KGraphic::draw(int x, int y, float angle, float zoom, float blend)
{
    img_src_x = 0;
    img_src_y = 0;
    img_dst_x = getTextureSizeW();
    img_dst_y = getTextureSizeH();
    
    drawEx(img_src_x, img_src_y, img_dst_x, img_dst_y, x, y, angle, zoom, blend);
}

void KGraphic::drawEx(int x1, int y1, int x2, int y2, int destX, int destY, float angle, float zoom, float blend)
{
    this->srcX = x1;
    this->srcY = y1;
    this->destX = destX;
    this->destY = destY;
    this->sizeW = x2 - x1;
    this->sizeH = y2 - y1;
    this->angle = angle;
    this->zoom = zoom;
    this->blend = blend;
    
    render();
}

void KGraphic::drawShape(int numvertices, vec2* vertice, int destX, int destY, float linewidth, float r, float g, float b, float a)
{
    if (_shapeShaderProgram == 0) {
        printf("Error: Line shader program is invalid.\n");
        return;
    }

    glUseProgram(_shapeShaderProgram);

    setOrthographicProjection(projectionMatrix, 0.0f, _gameW, 0.0f, _gameH);

    GLuint matrixProjection = glGetUniformLocation(_shapeShaderProgram, "u_projectionMatrix");
    if (matrixProjection != -1) {
        glUniformMatrix4fv(matrixProjection, 1, GL_FALSE, projectionMatrix);
    } else {
        printf("Projection matrix uniform not found in shader!\n");
    }

    mat4 modelViewMatrix;
    setIdentityMatrix(modelViewMatrix);

    translateMatrix(modelViewMatrix, destX, destY);
    translateMatrix(modelViewMatrix, shape_centerX, shape_centerX);

    if (angle != 0.0f) {
        rotateMatrix(modelViewMatrix, angle * M_PI / 180.0f);
    }

    translateMatrix(modelViewMatrix, -shape_centerX, -shape_centerX);
    
    GLuint matrixModelView = glGetUniformLocation(_shapeShaderProgram, "u_matrix");
    if (matrixModelView != -1) {
        glUniformMatrix4fv(matrixModelView, 1, GL_FALSE, modelViewMatrix);
    } else {
        printf("Model-view matrix uniform not found in shader!\n");
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glLineWidth(linewidth);

    for (int i = 0; i < numvertices; ++i) {
        int j = (i + 1) % numvertices;

        GLfloat lineCoords[] = {
            vertice[i][0], vertice[i][1],
            vertice[j][0], vertice[j][1]
        };

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_Line);
        glBufferData(GL_ARRAY_BUFFER, sizeof(lineCoords), lineCoords, GL_DYNAMIC_DRAW);

        GLint posAttrib = glGetAttribLocation(_shapeShaderProgram, "a_position");
        if (posAttrib == -1) {
            printf("Error: Position attribute not found in shader.\n");
            return;
        }
        glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        GLint colorUniform = glGetUniformLocation(_shapeShaderProgram, "color");
        if (colorUniform != -1) {
            glUniform4f(colorUniform, r, g, b, a);
        } else {
            printf("Color uniform not found in shader!\n");
        }

        glDrawArrays(GL_LINES, 0, 2);

        glDisableVertexAttribArray(posAttrib);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);

    printGLError("drawShape");
}

void KGraphic::drawSolidRectangle(float x, float y, float width, float height,
                                  float r, float g, float b, float a)
{
    if (_solidShaderProgram == 0) {
        printf("Error: Solid shader program is invalid.\n");
        return;
    }

    glUseProgram(_solidShaderProgram);

    setOrthographicProjection(projectionMatrix, 0.0f, _gameW, 0.0f, _gameH);

    GLfloat vertices[] = {
        x,        y,         0.0f,
        x + width, y,         0.0f,
        x,        y + height, 0.0f,
        x + width, y + height, 0.0f
    };

    GLushort indices[] = { 0, 1, 2, 1, 3, 2 };

    GLuint matrixProjection = glGetUniformLocation(_solidShaderProgram, "u_projectionMatrix");
    if (matrixProjection != -1) {
        glUniformMatrix4fv(matrixProjection, 1, GL_FALSE, projectionMatrix);
    } else {
        printf("Projection matrix uniform not found in shader!\n");
    }

    mat4 modelViewMatrix;
    setIdentityMatrix(modelViewMatrix);

    translateMatrix(modelViewMatrix, destX, destY);
    translateMatrix(modelViewMatrix, shape_centerX, shape_centerX);

    if (angle != 0.0f) {
        rotateMatrix(modelViewMatrix, angle * M_PI / 180.0f);
    }

    translateMatrix(modelViewMatrix, -shape_centerX, -shape_centerX);

    GLuint matrixModelView = glGetUniformLocation(_solidShaderProgram, "u_matrix");
    if (matrixModelView != -1) {
        glUniformMatrix4fv(matrixModelView, 1, GL_FALSE, modelViewMatrix);
    } else {
        printf("Model-view matrix uniform not found in shader!\n");
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_Solid);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    if (!indexBuffer_Solid) {
        glGenBuffers(1, &indexBuffer_Solid);
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_Solid);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    GLint posAttrib = glGetAttribLocation(_solidShaderProgram, "a_position");
    if (posAttrib == -1) {
        printf("Error: Position attribute not found in shader.\n");
        return;
    }
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    GLint colorUniform = glGetUniformLocation(_solidShaderProgram, "uColor");
    if (colorUniform != -1) {
        glUniform4f(colorUniform, r, g, b, a);
    } else {
        printf("Color uniform not found in shader!\n");
    }

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

    glDisableVertexAttribArray(posAttrib);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glUseProgram(0);
}


// Offscreen -----------------------------------------------------

void KGraphic::createRenderTexture(int width, int height)
{
    if(_fboWidth  > 0 && _fboHeight > 0) {
        return;
    }
    const float retinaScaleFactor = 2.0f; // Added retina scale factor
    _fboWidth = width * retinaScaleFactor;
    _fboHeight = height * retinaScaleFactor;

    glGenFramebuffers(1, &_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

    glGenTextures(1, &_fboTexture);
    glBindTexture(GL_TEXTURE_2D, _fboTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _fboWidth, _fboHeight, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, _fboTexture, 0);
    
    GLenum fbStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void KGraphic::beginRenderToTexture()
{
    if (_fbo == 0) {
        printf("[ERROR] Framebuffer not initialized! Call createRenderTexture first.\n");
        return;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
    glViewport(0, 0, _fboWidth, _fboHeight);
    //printf("[DEBUG] beginRenderToTexture: Set viewport to 0,0,%d,%d\n", _fboWidth, _fboHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //printf("[DEBUG] beginRenderToTexture - Bound FBO: %d\n", _fbo);
    //printf("beginRenderToTexture\n");
}

void KGraphic::endRenderToTexture()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if (_scaledGameW == 0 || _scaledGameH == 0) {
        _scaledGameW = _screenW;
        _scaledGameH = _screenH;
    }

    //printf("[DEBUG] endRenderToTexture - Restoring viewport: offset(%f, %f), size(%f, %f)\n",
     //      _offsetX, _offsetY, _scaledGameW, _scaledGameH);
    
    //printf("[DEBUG] FBO Dimensions: %d x %d\n", _fboWidth, _fboHeight);
    //printf("[DEBUG] Screen Dimensions: %f x %f\n", _screenW, _screenH);
    //printf("[DEBUG] Game Dimensions: %f x %f\n", _gameW, _gameH);
    //printf("[DEBUG] Computed Offset: %f, %f\n", _offsetX, _offsetY);
    //printf("[DEBUG] Computed Scaled Size: %f, %f\n", _scaledGameW, _scaledGameH);

    glViewport(_offsetX, _offsetY, _scaledGameW, _scaledGameH);
    
    printGLError("endRenderToTexture - glViewport");
}

// Updated drawRenderTexture to adjust for retina scaling
void KGraphic::drawRenderTexture(int destX, int destY, float angle, float zoom, float blend)
{
    const float retinaScaleFactor = 2.0f; // Must match createRenderTexture
    glUseProgram(_shaderProgram);
    
    setOrthographicProjection(projectionMatrix, 0.0f, _gameW, 0.0f, _gameH);
    GLuint matrixProjection = glGetUniformLocation(_shaderProgram, "u_projectionMatrix");
    if (matrixProjection != -1) {
        glUniformMatrix4fv(matrixProjection, 1, GL_FALSE, projectionMatrix);
    }
    
    mat4 modelViewMatrix;
    setIdentityMatrix(modelViewMatrix);
    // Adjust position & scale by dividing by retinaScaleFactor
    translateMatrix(modelViewMatrix, destX + ((_fboWidth / retinaScaleFactor) * zoom) / 2.0f,
                                    destY + ((_fboHeight / retinaScaleFactor) * zoom) / 2.0f);
    
    if (angle != 0.0f) {
        rotateMatrix(modelViewMatrix, angle * M_PI / 180.0f);
    }
    scaleMatrix(modelViewMatrix, (_fboWidth / retinaScaleFactor) * zoom,
                                   (_fboHeight / retinaScaleFactor) * zoom);
    
    GLuint matrixLocation = glGetUniformLocation(_shaderProgram, "u_matrix");
    glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, modelViewMatrix);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _fboTexture);
    glUniform1i(textureSamplerLoc, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glEnableVertexAttribArray(positionAttribLocation);
    glEnableVertexAttribArray(texCoordAttribLocation);
    glVertexAttribPointer(positionAttribLocation, 2, GL_FLOAT, GL_FALSE,
                          4 * sizeof(GLfloat), (GLvoid*)0);
    glVertexAttribPointer(texCoordAttribLocation, 2, GL_FLOAT, GL_FALSE,
                          4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
    glDisableVertexAttribArray(positionAttribLocation);
    glDisableVertexAttribArray(texCoordAttribLocation);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glUseProgram(0);
}

void KGraphic::drawOffscreenTexture(float tx, float ty, float scaleX, float scaleY)
{
    setOrthographicProjection(projectionMatrix, 0.0f, _gameW, 0.0f, _gameH);
    
    float recalibrageX = tx / (_screenW/2);
    float recalibrageY = -ty / (_screenH/2);
    
    glUseProgram(_simpleProgram); // basic shader for textured quad

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _fboTexture);
    GLint samplerLoc = glGetUniformLocation(_simpleProgram, "u_texture");
    glUniform1i(samplerLoc, 0);

    // Construct transformation matrix (translate + scale)
    mat4 transform;
    setIdentityMatrix(transform);             // Set matrix to identity
    translateMatrix(transform, recalibrageX, recalibrageY);   // Apply translation
    scaleMatrix(transform, scaleX, scaleY);// Apply scaling (zoom)

    if (angle != 0.0f) {
        rotateMatrix(transform, angle * M_PI / 180.0f);
    }
    
    GLint transformLoc = glGetUniformLocation(_simpleProgram, "u_transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, transform);

    // Fullscreen quad vertices: positions (xy) and texCoords (uv)
    GLfloat quadVertices[] = {
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f, 1.0f,
         1.0f,  1.0f, 1.0f, 1.0f,
    };

    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    GLint posLoc = glGetAttribLocation(_simpleProgram, "a_position");
    GLint texLoc = glGetAttribLocation(_simpleProgram, "a_texCoord");
    glEnableVertexAttribArray(posLoc);
    glVertexAttribPointer(posLoc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(texLoc);
    glVertexAttribPointer(texLoc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

    // Draw the quad
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // Clean up
    glDisableVertexAttribArray(posLoc);
    glDisableVertexAttribArray(texLoc);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);
}

GLuint KGraphic::getFbo()
{
    return _fbo;
}

GLuint KGraphic::getFboTexture()
{
    return _fboTexture;
}

int KGraphic::getFboWidth()
{
    return _fboWidth;
}

int KGraphic::getFboHeight()
{
    return _fboHeight;
}
