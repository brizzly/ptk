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

	FT_Set_Pixel_Sizes(face, 0, _fontSize);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

	for (unsigned char c = 0; c < 128; c++) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER) != 0) {
			std::cerr << "ERROR::FREETYPE: Failed to load Glyph for character '" << c << "'" << std::endl;
			continue;
		}

		// Generate texture only if bitmap data is valid
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

			// Texture parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		
//		int bbox_ymax = FT_MulFix(face->bbox.yMax, face->size->metrics.y_scale) >> 6;
//		int bbox_ymin = FT_MulFix(face->bbox.yMin, face->size->metrics.y_scale) >> 6;
//		int height = bbox_ymax - bbox_ymin;

		// Calculate the glyph's bitmap height (from the rasterized data)
		int bitmap_height = face->glyph->bitmap.rows;

		
		// Store character metrics regardless of bitmap data
		Character character = {
			texture,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			face->glyph->bitmap_left,
			face->glyph->bitmap_top,
			static_cast<unsigned int>(face->glyph->advance.x),
			static_cast<unsigned int>(bitmap_height)
		};

		characters.insert(std::pair<char, Character>(c, character));
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

void KFont::RenderText(const wchar_t* text, float x, float y, float scale)
{
	if (_fonteShaderProgram == 0) {
		printf("Error: Shader program is invalid.\n");
		return;
	}
	
	// debug
	//int x_copy = x;
    
    y = _gameH - y - scale;
    
    float r = _gameW / _gameH;
	float scaleCopy = scale;
    scale = scale * r * 2.0f / KFONT_SIZE;
    

	setupOrthoProjection(0.0f, _gameW, 0.0f, _gameH);
	

	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	std::string utf8Text = converter.to_bytes(text);

	glUseProgram(_fonteShaderProgram);

	// Set text color
	GLint textColorLoc = glGetUniformLocation(_fonteShaderProgram, "textColor");
	if (textColorLoc != -1) {
        glUniform3f(textColorLoc, text_R, text_G, text_B);
	}
    
    // Set background color
    GLint backgroundColorLoc = glGetUniformLocation(_fonteShaderProgram, "backgroundColor");
    if (backgroundColorLoc != -1) {
        glUniform4f(backgroundColorLoc, back_R, back_G, back_B, back_A);
    }
    
	GLint textUniform = glGetUniformLocation(_fonteShaderProgram, "text");
	if (textUniform != -1) {
		glUniform1i(textUniform, 0);  // Set the sampler to use texture unit 0
	}

	GLint matrixLoc = glGetUniformLocation(_fonteShaderProgram, "u_matrix");
	if (matrixLoc != -1) {
		glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, orthoMatrix);
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float currentTextureID = 0;  // Track currently bound texture
	bool needReturnLine = false;

	std::istringstream stream(utf8Text);
	std::string word;
	float x_copy = x; // Store initial x position for new lines

	while (stream >> word) {
		// Iterate over each character in the word
		float wordWidth = 0.0f;

		// Calculate word width
		for (char c : word) {
			Character ch = characters[c];
			wordWidth += (ch.advance >> 6) * scale;
		}

		// Check if adding this word exceeds the max width
		if (_maxCharsBeforeNewLine > 0 && x + wordWidth - x_copy > _maxCharsBeforeNewLine) {
			
			needReturnLine = true;
		}
		if(needReturnLine == true)
		{
			for (char c : word) {
				Character ch = characters[c];
				if(ch.advance2 > 0)
				{
					x = x_copy;
					y -= (ch.advance2) * scale;
					y -= (ch.advance2) * 0.5f * scale;
					//y -= scaleCopy * scale * 2;
					needReturnLine = false;
					break;
				}
			}
		}

		// Now render the word character by character
		for (char c : word) {
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

				// Update texture only if different
				if (ch.textureID != currentTextureID) {
					glBindTexture(GL_TEXTURE_2D, ch.textureID);
					currentTextureID = ch.textureID;
				}

				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

				// Position attribute (first two floats: x, y)
				GLint posAttrib = glGetAttribLocation(_fonteShaderProgram, "position");
				if (posAttrib != -1) {
					glEnableVertexAttribArray(posAttrib);
					glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
				}

				// Texture coordinates attribute (last two floats: u, v)
				GLint texAttrib = glGetAttribLocation(_fonteShaderProgram, "texCoords");
				if (texAttrib != -1) {
					glEnableVertexAttribArray(texAttrib);
					glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
				}

				glDrawArrays(GL_TRIANGLES, 0, 6);
			}

			x += (ch.advance >> 6) * scale;
		}

		// Add space between words
		Character spaceChar = characters[' '];
		x += (spaceChar.advance >> 6) * scale;
	}


//	glDisableVertexAttribArray(glGetAttribLocation(_fonteShaderProgram, "position"));
//	glDisableVertexAttribArray(glGetAttribLocation(_fonteShaderProgram, "texCoords"));
	glBindTexture(GL_TEXTURE_2D, 0);
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

void KFont::RenderTextCentered(const wchar_t* text, float posy, float offset_x, float scale)
{
    float text_width=0, text_height=0;
    measure_text(text, &text_width, &text_height, scale);
    //printf("text w/h ; %f %f\n", text_width, text_height);
    float posx = (_gameW - text_width) / 2;
    //posx /= _scale;
    if(offset_x != 0) {
        posx += offset_x;
    }
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
