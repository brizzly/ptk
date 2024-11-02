
#include "KShader.h"
//#include <OpenGLES/ES2/gl.h>
#include <cmath>
#include <cstring>
#include <cstdio> // printf

KShader::KShader()
{
}

// Function to compile a shader (vertex or fragment)
GLuint KShader::compileShader(GLenum type, const char* source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    
    // Check for compilation errors
    GLint compileStatus;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus != GL_TRUE) {
        char buffer[512];
        glGetShaderInfoLog(shader, sizeof(buffer), nullptr, buffer);
        printf("Shader compilation error: %s\n", buffer);  // Log the error
        glDeleteShader(shader);  // Delete the shader
        return 0;  // Return 0 to indicate failure
    }
    return shader;
}

// Function to link vertex and fragment shaders into a shader program
GLuint KShader::createShaderProgram(const char* vertexSource, const char* fragmentSource)
{
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    if (vertexShader == 0) {
        return 0;  // Return if vertex shader compilation failed
    }
    
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
    if (fragmentShader == 0) {
        glDeleteShader(vertexShader);  // Clean up vertex shader
        return 0;  // Return if fragment shader compilation failed
    }
    
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    // Check for linking errors
    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
        
        // Get the error log
        char buffer[512];
        glGetProgramInfoLog(program, sizeof(buffer), nullptr, buffer);
        
        // Print the error log
        printf("Shader linking error: %s\n", buffer);
        
        // Clean up shaders and program
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(program);
        return 0;  // Return 0 to indicate failure
    }
    
    // Clean up shaders as they are no longer needed after linking
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return program;  // Return the valid shader program
}

GLuint KShader::createLineShader()
{
    const char* vertexSource =
        "attribute vec2 position;"
        "uniform mat4 u_Matrix;"  // Matrix uniform for projection
        "void main() {"
        "    gl_Position = u_Matrix * vec4(position, 0.0, 1.0);"  // Apply matrix transformation
        "}";

    const char* fragmentSource =
        "precision mediump float;"
        "uniform vec4 color;"
        "void main() {"
        "    gl_FragColor = color;"
        "}";

    // Use createShaderProgram to compile, link, and return the program
    return createShaderProgram(vertexSource, fragmentSource);
}

GLuint KShader::createFonteShader()
{
	const char* vertexSource =
		"attribute vec2 position;"         // Separate position attribute
		"attribute vec2 texCoords;"        // Separate texture coordinates attribute
		"uniform mat4 u_Matrix;"           // Projection matrix uniform
		"varying vec2 TexCoords;"          // Pass texture coordinates to fragment shader
		"void main() {"
		"    gl_Position = u_Matrix * vec4(position, 0.0, 1.0);"  // Transform position
		"    TexCoords = texCoords;"       // Pass texture coordinates
		"}";
	
	/*
	const char* fragmentSource =
		"precision mediump float;"
		"uniform vec3 textColor;"

		"void main() {"
		"	gl_FragColor = vec4(textColor, 1.0);"  // Render solid color
		"}";*/

	// Fragment shader source code (Corrected)
	const char* fragmentSource =
		"precision mediump float;\n"
		"varying vec2 TexCoords;\n"
		"uniform sampler2D text;\n"
		"void main() {\n"
		"    float alpha = texture2D(text, TexCoords).a;\n"  // Correctly sample the alpha channel
		"    gl_FragColor = vec4(alpha, alpha, alpha, alpha);\n"  // Set correct output color with alpha
		"}";
	

	
	/*
	const char* fragmentSource =
		"precision mediump float;"
		"varying vec2 TexCoords;"
		"uniform sampler2D text;"
		"void main() {"
		"	float alpha = texture2D(text, TexCoords).r;"  // Sample the alpha channel
		"	gl_FragColor = vec4(alpha, alpha, alpha, 1.0);"  // Output alpha as grayscale
		"}";
	*/
	
	/*
	const char* fragmentSource =
		"precision mediump float;"
		"varying vec2 TexCoords;"          // Interpolated texture coordinates
		"uniform sampler2D text;"          // Texture sampler for glyph
		"uniform vec3 textColor;"          // Text color uniform
		"void main() {"
		"    vec4 sampled = vec4(1.0, 1.0, 1.0, texture2D(text, TexCoords).r);"  // Sample alpha
		"    gl_FragColor = vec4(textColor, 1.0);" // * sampled;"  // Apply color to text
		"}";
	*/
	// Compile, link, and return the shader program
	return createShaderProgram(vertexSource, fragmentSource);
}

