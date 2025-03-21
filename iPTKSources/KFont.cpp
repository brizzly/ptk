// KFont.cpp
#include "KFont.h"

KFont::~KFont()
{
}

#ifdef __ANDROID__
KFont::KFont(const char* fontPath, float gameWidth, float gameHeight, AAssetManager * value)
{
    this->g_assetManager = value;
    this->init(fontPath, gameWidth, gameHeight);
}
#else
KFont::KFont(const char* fontPath, float gameWidth, float gameHeight)
{
    this->init(fontPath, gameWidth, gameHeight);
}
#endif

void KFont::init(const char* fontPath, float gameWidth, float gameHeight)
{
	_fontSize = KFONT_SIZE;
	_gameW = gameWidth;
	_gameH = gameHeight;
	shader = new KShader();
	_fonteShaderProgram = shader->createFonteShader();
	_maxCharsBeforeNewLine = -1;
	
    SetBackgroundColor(0,0,0,0);
    SetTextColor(1,1,1);

	// Generate VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);  // Allocate space for position and texture coordinates
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (FT_Init_FreeType(&ft)) {
		std::cerr << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		return;
	}

#if __ANDROID__
    // Open the font asset
    AAsset* asset = AAssetManager_open(g_assetManager, fontPath, AASSET_MODE_BUFFER);
    if (!asset) {
        std::cerr << "ERROR::ASSET: Could not open font asset" << std::endl;
        return;
    }

    // Get the font data and size
    size_t fontSize = AAsset_getLength(asset);
    const void* fontData = AAsset_getBuffer(asset);
    if (!fontData) {
        std::cerr << "ERROR::ASSET: Could not read font data" << std::endl;
        AAsset_close(asset);
        return;
    }

    // Load the font face from memory
    if (FT_New_Memory_Face(ft, reinterpret_cast<const FT_Byte*>(fontData), fontSize, 0, &face)) {
        std::cerr << "ERROR::FREETYPE: Failed to load font from memory" << std::endl;
        AAsset_close(asset);
        return;
    }
#else
	if (FT_New_Face(ft, fontPath, 0, &face)) {
		std::cerr << "ERROR::FREETYPE: Failed to load font" << std::endl;
		return;
	}
#endif

    
    // Sélection du mode Unicode
    if (FT_Select_Charmap(face, FT_ENCODING_UNICODE)) {
        std::cerr << "ERROR::FREETYPE: Failed to select Unicode charmap" << std::endl;
        return;
    }
    
    /*
    if (FT_Select_Charmap(face, FT_ENCODING_ADOBE_LATIN_1)) {
        std::cerr << "ERROR::FREETYPE: Failed to select Latin-1 charmap" << std::endl;
        return;
    }*/

    // --- Retina scaling update ---
    const float retinaScaleFactor = 2.0f;
    FT_Set_Pixel_Sizes(face, 0, _fontSize * retinaScaleFactor);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Charger les caractères de 32 à 255 (Latin-1 étendu)
    for (unsigned int c = 32; c < 256; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER) != 0) {
            std::cerr << "ERROR::FREETYPE: Failed to load Glyph for character '" << (char)c << "' (code: " << c << ")" << std::endl;
            continue;
        }

        GLuint texture = 0;
        if (face->glyph->bitmap.buffer != nullptr &&
            face->glyph->bitmap.width > 0 &&
            face->glyph->bitmap.rows > 0) {

            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);

            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_ALPHA,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_ALPHA,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );

            // Paramètres texture
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glGenerateMipmap(GL_TEXTURE_2D);
        }


        // Adjust glyph metrics to logical (non-retina) dimensions
        int scaledWidth    = face->glyph->bitmap.width    / retinaScaleFactor;
        int scaledRows     = face->glyph->bitmap.rows     / retinaScaleFactor;
        int scaledBearingX = face->glyph->bitmap_left   / retinaScaleFactor;
        int scaledBearingY = face->glyph->bitmap_top    / retinaScaleFactor;
        // Advance is in 26.6 format; dividing retains fixed-point info for RenderText
        unsigned int scaledAdvance = static_cast<unsigned int>(face->glyph->advance.x / retinaScaleFactor);

        // Optionally, store bitmap height for line spacing adjustment
        int scaledBitmapHeight = scaledRows;

        Character character = {
            texture,
            scaledWidth,
            scaledRows,
            scaledBearingX,
            scaledBearingY,
            scaledAdvance,
            static_cast<unsigned int>(scaledBitmapHeight)
        };

        characters.insert(std::pair<unsigned int, Character>(c, character));
    }

	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void KFont::SetMaxCharBeforeLine(int value)
{
#ifndef __ANDROID__
	//value *= 2;
#endif
	_maxCharsBeforeNewLine = value;
}

void KFont::UnsetMaxCharBeforeLine()
{
	SetMaxCharBeforeLine(1);
}

void KFont::SetBackgroundColor(float r, float g, float b, float a)
{
    back_R = r;
    back_G = g;
    back_B = b;
    back_A = a;
}

void KFont::SetTextColor(float r, float g, float b)
{
    text_R = r;
    text_G = g;
    text_B = b;
}

void KFont::RenderTextCentered(const wchar_t* text, float posy, float offset_x, float scale)
{
    float text_width=0, text_height=0;
    measure_text(text, &text_width, &text_height, scale);
    float posx = (_gameW - text_width) / 2;
    if(offset_x != 0) {
        posx += offset_x;
    }
    RenderText(text, posx, posy, scale);
}

void KFont::RenderText(const wchar_t* text, float x, float y, float scale)
{
    if (_fonteShaderProgram == 0) {
        printf("Error: Shader program is invalid.\n");
        return;
    }

    // Ajustement vertical
    y = _gameH - y - scale;

    float r = _gameW / _gameH;
    float scaleCopy = scale;
    scale = scale * r * 2.0f / KFONT_SIZE;

    setupOrthoProjection(0.0f, _gameW, 0.0f, _gameH);

    glUseProgram(_fonteShaderProgram);

    // Couleur du texte
    GLint textColorLoc = glGetUniformLocation(_fonteShaderProgram, "textColor");
    if (textColorLoc != -1) {
        glUniform3f(textColorLoc, text_R, text_G, text_B);
    }

    // Couleur de fond
    GLint backgroundColorLoc = glGetUniformLocation(_fonteShaderProgram, "backgroundColor");
    if (backgroundColorLoc != -1) {
        glUniform4f(backgroundColorLoc, back_R, back_G, back_B, back_A);
    }

    GLint textUniform = glGetUniformLocation(_fonteShaderProgram, "text");
    if (textUniform != -1) {
        glUniform1i(textUniform, 0);  // utilise l’unité de texture 0
    }

    GLint matrixLoc = glGetUniformLocation(_fonteShaderProgram, "u_matrix");
    if (matrixLoc != -1) {
        glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, orthoMatrix);
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float currentTextureID = 0;
    bool needReturnLine = false;

    // Convertir le texte en wstring
    std::wstring textStr(text);
    std::wistringstream stream(textStr);
    std::wstring word;
    float x_copy = x; // position initiale pour retour à la ligne

    while (stream >> word) {
        float wordWidth = 0.0f;

        // Calcul de la largeur du mot
        for (wchar_t wc : word) {
            unsigned int c = (unsigned int)wc;
            Character ch = characters[c];
            wordWidth += (ch.advance >> 6) * scale;
        }

        // Vérifie si le mot dépasse la largeur max
        if (_maxCharsBeforeNewLine > 0 && x + wordWidth - x_copy > _maxCharsBeforeNewLine) {
            needReturnLine = true;
        }
        if (needReturnLine == true) {
            for (wchar_t wc : word) {
                unsigned int c = (unsigned int)wc;
                Character ch = characters[c];
                if (ch.advance2 > 0) {
                    x = x_copy;
                    y -= (ch.advance2) * scale;
                    y -= (ch.advance2) * 0.5f * scale;
                    needReturnLine = false;
                    break;
                }
            }
        }

        // Rendu du mot caractère par caractère
        for (wchar_t wc : word) {
            unsigned int c = (unsigned int)wc;
            Character ch = characters[c];

            float xpos = x + ch.bearingX * scale;
            float ypos = y - (ch.sizeY - ch.bearingY) * scale;
            float w = ch.sizeX * scale;
            float h = ch.sizeY * scale;

            GLfloat vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }
            };

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            if (ch.textureID != 0) {
                glActiveTexture(GL_TEXTURE0);

                if (ch.textureID != currentTextureID) {
                    glBindTexture(GL_TEXTURE_2D, ch.textureID);
                    currentTextureID = ch.textureID;
                }

                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

                GLint posAttrib = glGetAttribLocation(_fonteShaderProgram, "position");
                if (posAttrib != -1) {
                    glEnableVertexAttribArray(posAttrib);
                    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
                }

                GLint texAttrib = glGetAttribLocation(_fonteShaderProgram, "texCoords");
                if (texAttrib != -1) {
                    glEnableVertexAttribArray(texAttrib);
                    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
                }

                glDrawArrays(GL_TRIANGLES, 0, 6);
            }

            x += (ch.advance >> 6) * scale;
        }

        // Ajoute un espace entre les mots
        Character spaceChar = characters[(unsigned int)' '];
        x += (spaceChar.advance >> 6) * scale;
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

void KFont::RenderTextOffscreen(const wchar_t* text, float x, float y, float scale, GLuint fbo, GLuint texture, int fboWidth, int fboHeight)
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        printf("Error: Offscreen framebuffer is incomplete.\n");
        return;
    }
    
    x *= 2;
    y *= 2;

    glViewport(0, 0, fboWidth, fboHeight);

    //glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (_fonteShaderProgram == 0) {
        printf("Error: Shader program is invalid.\n");
        return;
    }

    y = fboHeight - y - scale;
    float r = (float)fboWidth / (float)fboHeight;
    //scale = scale * r * 2.0f / KFONT_SIZE;
    scale = scale * r / KFONT_SIZE;

    setupOrthoProjection(0.0f, fboWidth, 0.0f, fboHeight);

    glUseProgram(_fonteShaderProgram);

    GLint textColorLoc = glGetUniformLocation(_fonteShaderProgram, "textColor");
    if (textColorLoc != -1) {
        glUniform3f(textColorLoc, text_R, text_G, text_B);
    }

    GLint backgroundColorLoc = glGetUniformLocation(_fonteShaderProgram, "backgroundColor");
    if (backgroundColorLoc != -1) {
        glUniform4f(backgroundColorLoc, back_R, back_G, back_B, back_A);
    }

    GLint textUniform = glGetUniformLocation(_fonteShaderProgram, "text");
    if (textUniform != -1) {
        glUniform1i(textUniform, 0);
    }

    GLint matrixLoc = glGetUniformLocation(_fonteShaderProgram, "u_matrix");
    if (matrixLoc != -1) {
        glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, orthoMatrix);
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float currentTextureID = 0;
    std::wstring textStr(text);
    for (wchar_t wc : textStr) {
        Character ch = characters[(unsigned int)wc];

        float xpos = x + ch.bearingX * scale;
        float ypos = y - (ch.sizeY - ch.bearingY) * scale;
        float w = ch.sizeX * scale;
        float h = ch.sizeY * scale;

        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        glActiveTexture(GL_TEXTURE0);
        if (ch.textureID != currentTextureID) {
            glBindTexture(GL_TEXTURE_2D, ch.textureID);
            currentTextureID = ch.textureID;
        }

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

        GLint posAttrib = glGetAttribLocation(_fonteShaderProgram, "position");
        glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

        GLint texAttrib = glGetAttribLocation(_fonteShaderProgram, "texCoords");
        glEnableVertexAttribArray(texAttrib);
        glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (ch.advance >> 6) * scale;
    }

//    glBindTexture(GL_TEXTURE_2D, 0);
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);

//    glViewport(0, 0, _gameW, _gameH);
}



void KFont::RenderTextCenteredButton(const wchar_t* text, float posx, float posy, float btn_width, float scale)
{
	float text_width = 0.0f, text_height = 0.0f;

	// Measure the text width and height
	measure_text(text, &text_width, &text_height, scale);

	// Adjust position to center the text horizontally within the button
    posx += (btn_width - text_width) / 2;

	// Render the text using drawText
	RenderText(text, posx, posy, scale);
}



void KFont::measure_text(const wchar_t* text, float* width, float* height, float scale)
{
	*width = 0;
	*height = 0;
    
    float r = _gameW / _gameH;
    float newScale = r * 2.0f / KFONT_SIZE;
    
    scale *= newScale;

	for (const wchar_t* c = text; *c; ++c) {
		// Convert wchar_t to char (assuming ASCII or compatible encoding)
		char charCode = static_cast<char>(*c);
		
		// Check if the character is in the loaded glyph map
		if (characters.find(charCode) != characters.end()) {
			Character ch = characters[charCode];
			
			// Accumulate width based on character advance
			*width += (ch.advance >> 6) * scale;  // Convert 26.6 fixed-point to pixels

			// Find the maximum height of the characters
			float charHeight = ch.sizeY * scale;
			if (charHeight > *height) {
				*height = charHeight;
			}
		} else {
			//std::cerr << "Character " << charCode << " not found in font glyph map." << std::endl;
		}
	}
}

void KFont::setupOrthoProjection(float left, float right, float bottom, float top)
{
	orthoMatrix[0] = 2.0f / (right - left);
	orthoMatrix[1] = 0.0f;
	orthoMatrix[2] = 0.0f;
	orthoMatrix[3] = 0.0f;

	orthoMatrix[4] = 0.0f;
	orthoMatrix[5] = 2.0f / (top - bottom);
	orthoMatrix[6] = 0.0f;
	orthoMatrix[7] = 0.0f;

	orthoMatrix[8] = 0.0f;
	orthoMatrix[9] = 0.0f;
	orthoMatrix[10] = -1.0f;
	orthoMatrix[11] = 0.0f;

	orthoMatrix[12] = -(right + left) / (right - left);
	orthoMatrix[13] = -(top + bottom) / (top - bottom);
	orthoMatrix[14] = 0.0f;
	orthoMatrix[15] = 1.0f;
}
