#pragma once

#include <string>
#include <GL/glew.h>
// #include <windows.h>

// GLSL wrapper
class Shader
{
public:
	Shader();
	~Shader();

	//returns what we need for rendering
	[[nodiscard]] GLuint handle() const { return m_programObject; }

	//loads the shader program from two text files
	bool load(const std::string& name, const char* vertexFilename, const char* fragmentFilename);

	[[nodiscard]] GLint getProjLocation() const;
	[[nodiscard]] GLint getModelViewLocation() const;

private:
	std::string m_name;
	GLuint m_vertexShader;       //identifier for the vertex shader
	GLuint m_fragmentShader;     //identifier for the fragment shader
	GLuint m_programObject;      //identifier for the program- this is used when rendering.
	GLint proj_location;
	GLint model_view_location;

	static GLuint loadShader(const char* filename, GLenum type) ;
	static std::string shaderInfoLog(GLuint shader) ;
	static std::string programInfoLog(GLuint program) ;
};