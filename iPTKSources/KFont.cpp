// KFont.cpp
#include "KFont.h"

KFont::KFont(const char* fontPath, float gameWidth, float gameHeight)
{
	_fontSize = KFONT_SIZE;
	_gameW = gameWidth;
	_gameH = gameHeight;
	shader = new KShader();
	_fonteShaderProgram = shader->createFonteShader();
    
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

	if (FT_New_Face(ft, fontPath, 0, &face)) {
		std::cerr << "ERROR::FREETYPE: Failed to load font" << std::endl;
		return;
	}

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

		// Store character metrics regardless of bitmap data
		Character character = {
			texture,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			face->glyph->bitmap_left,
			face->glyph->bitmap_top,
			static_cast<unsigned int>(face->glyph->advance.x)
		};

		characters.insert(std::pair<char, Character>(c, character));
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);
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
    
    y = _gameH - y - scale;
    
    float r = _gameW / _gameH;
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

	GLint matrixLoc = glGetUniformLocation(_fonteShaderProgram, "u_Matrix");
	if (matrixLoc != -1) {
		glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, orthoMatrix);
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (auto c : utf8Text)
	{
		Character ch = characters[c];
		
		float uWidth = static_cast<float>(ch.sizeX) / ch.sizeX;  // This results in 1.0f but validates each glyph’s size
		float vHeight = static_cast<float>(ch.sizeY) / ch.sizeY;
		
		float xpos = x + ch.bearingX * scale;
		float ypos = y - (ch.sizeY - ch.bearingY) * scale;
		float w = ch.sizeX * scale;
		float h = ch.sizeY * scale;

		// Set up vertices to sample only the glyph portion of the texture
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },             // Bottom-left
			{ xpos,     ypos,       0.0f, vHeight },          // Top-left
			{ xpos + w, ypos,       uWidth, vHeight },        // Top-right

			{ xpos,     ypos + h,   0.0f, 0.0f },             // Bottom-left
			{ xpos + w, ypos,       uWidth, vHeight },        // Top-right
			{ xpos + w, ypos + h,   uWidth, 0.0f }            // Bottom-right
		};
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (ch.textureID != 0) {
			
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, ch.textureID);
			
			
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

void KFont::measure_text(const wchar_t* text, float* width, float* height, float scale)
{
	*width = 0;
	*height = 0;
    
    float newScale = 2.0f / KFONT_SIZE;
    
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
