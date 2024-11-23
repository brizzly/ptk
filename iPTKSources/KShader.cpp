
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

GLuint KShader::createShader()
{
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
		"uniform float u_opacity;"
		"void main() {"
		"    vec4 texColor = texture2D(u_texture, v_texCoord);"
		"    gl_FragColor = vec4(texColor.rgb, texColor.a * u_opacity);"  // Apply opacity to alpha
		"}";

	// Use createShaderProgram to compile, link, and return the program
	return createShaderProgram(vertexShaderSource, fragmentShaderSource);
}

GLuint KShader::createLineShader()
{
    const char* vertexSource =
        "attribute vec4 a_position;"
        "uniform mat4 u_matrix;"
        "uniform mat4 u_projectionMatrix;"
        "void main() {"
        "    gl_Position = u_projectionMatrix * u_matrix * a_position;"
        "}";

    const char* fragmentSource =
        "precision mediump float;"
        "uniform vec4 color;"
        "void main() {"
        "    gl_FragColor = color;"
        "}";

    return createShaderProgram(vertexSource, fragmentSource);
}

GLuint KShader::createFonteShader()
{
	const char* vertexSource =
		"attribute vec2 position;"         // Separate position attribute
		"attribute vec2 texCoords;"        // Separate texture coordinates attribute
		"uniform mat4 u_matrix;"           // Projection matrix uniform
        
		"varying vec2 TexCoords;"          // Pass texture coordinates to fragment shader
		"void main() {"
		"    gl_Position = u_matrix * vec4(position, 0.0, 1.0);"  // Transform position
		"    TexCoords = texCoords;"       // Pass texture coordinates
		"}";
	
    const char* fragmentSource =
        "precision mediump float;\n"
        "varying vec2 TexCoords;\n"
        "uniform sampler2D text;\n"
        "uniform vec3 textColor;\n"            // Color of the text
        "uniform vec4 backgroundColor;\n"      // RGBA color for background
        "void main() {\n"
        "    float alpha = texture2D(text, TexCoords).a;\n"  // Sample alpha channel
        "    vec3 color = mix(backgroundColor.rgb, textColor, alpha);\n"  // Blend background and text colors
        "    float finalAlpha = mix(backgroundColor.a, 1.0, alpha);\n"    // Blend background and text alpha
        "    gl_FragColor = vec4(color, finalAlpha);\n"  // Set final color and alpha
        "}";
    
	// Compile, link, and return the shader program
	return createShaderProgram(vertexSource, fragmentSource);
}

