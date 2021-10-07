#include "nxpch.h"

#include "OpenGLShader.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Neurex {

OpenGLShader::OpenGLShader(const std::string& vertex, const std::string& fragment)
{
	// Create an empty vertex OpenGLShader handle
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Send the vertex OpenGLShader source code to GL
	// Note that std::string's .c_str is NULL character terminated.
	const GLchar* source = vertex.c_str();
	glShaderSource(vertexShader, 1, &source, 0);

	// Compile the vertex OpenGLShader
	glCompileShader(vertexShader);

	GLint isCompiled = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

		// We don't need the OpenGLShader anymore.
		glDeleteShader(vertexShader);

		NX_CORE_ASSERT(false, "Vertex OpenGLShader compilation failure.")
		NX_CORE_ERROR("{0}", infoLog.data());
	}

	// Create an empty fragment OpenGLShader handle
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Send the fragment OpenGLShader source code to GL
	// Note that std::string's .c_str is NULL character terminated.
	source = fragment.c_str();
	glShaderSource(fragmentShader, 1, &source, 0);

	// Compile the fragment OpenGLShader
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

		// We don't need the OpenGLShader anymore.
		glDeleteShader(fragmentShader);
		// Either of them. Don't leak shaders.
		glDeleteShader(vertexShader);

		NX_CORE_ASSERT(false, "Vertex OpenGLShader compilation failure.");
		NX_CORE_ERROR("{0}", infoLog.data());
	}

	// Vertex and fragment shaders are successfully compiled.
	// Now time to link them together into a program.
	// Get a program object.
	renderer_id = glCreateProgram();

	// Attach our shaders to our program
	glAttachShader(renderer_id, vertexShader);
	glAttachShader(renderer_id, fragmentShader);

	// Link our renderer_id
	glLinkProgram(renderer_id);

	// Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(renderer_id, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE) {
		GLint maxLength = 0;
		glGetProgramiv(renderer_id, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(renderer_id, maxLength, &maxLength, &infoLog[0]);

		// We don't need the renderer_id anymore.
		glDeleteProgram(renderer_id);
		// Don't leak shaders either.
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		NX_CORE_ASSERT(false, "OpenGLShader linking failure.");
		NX_CORE_ERROR("{0}", infoLog.data());
	}

	// Always detach shaders after a successful link.
	glDetachShader(renderer_id, vertexShader);
	glDetachShader(renderer_id, fragmentShader);
}

OpenGLShader::~OpenGLShader()
{
	glDeleteProgram(renderer_id);
}

void OpenGLShader::bind()
{
	glUseProgram(renderer_id);
}

void OpenGLShader::unbind()
{
	glUseProgram(0);
}

void OpenGLShader::upload_uniform(const std::string& name, const glm::mat4& uniform)
{
	GLint location = glGetUniformLocation(renderer_id, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(uniform));
}

void OpenGLShader::upload_uniform(const std::string& name, const glm::vec4& uniform)
{
	GLint location = glGetUniformLocation(renderer_id, name.c_str());
	glUniform4fv(location, 1, glm::value_ptr(uniform));
}
void OpenGLShader::upload_uniform(const std::string& name, const glm::vec3& uniform)
{
	GLint location = glGetUniformLocation(renderer_id, name.c_str());
	glUniform3fv(location, 1, glm::value_ptr(uniform));
}

void OpenGLShader::upload_uniform(const std::string& name, const glm::vec2& uniform)
{
	GLint location = glGetUniformLocation(renderer_id, name.c_str());
	glUniform2fv(location, 1, glm::value_ptr(uniform));
}

void OpenGLShader::upload_uniform(const std::string& name, float uniform)
{
	GLint location = glGetUniformLocation(renderer_id, name.c_str());
	glUniform1f(location, uniform);
}

void OpenGLShader::upload_uniform(const std::string& name, int uniform)
{
	GLint location = glGetUniformLocation(renderer_id, name.c_str());
	glUniform1i(location, uniform);
}
}
