#include "nxpch.h"
#include "Shader.h"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace Neurex {

	Shader::Shader(const std::string& vertex, const std::string& fragment)
	{
		
		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = vertex.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);
			
			NX_CORE_ASSERT(false, "Vertex shader compilation failure.")
			NX_CORE_ERROR("{0}", infoLog.data());
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = fragment.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			NX_CORE_ASSERT(false, "Vertex shader compilation failure.");
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
		if (isLinked == GL_FALSE)
		{
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

			NX_CORE_ASSERT(false, "Shader linking failure.");
			NX_CORE_ERROR("{0}", infoLog.data());
		}

		// Always detach shaders after a successful link.
		glDetachShader(renderer_id, vertexShader);
		glDetachShader(renderer_id, fragmentShader);
	}

	Shader::~Shader() {
		glDeleteProgram(renderer_id);
	}

	void Shader::bind() {
		glUseProgram(renderer_id);
	}

	void Shader::unbind() {
		glUseProgram(0);
	}

	void Shader::upload_uniform_mat4(const std::string& name, const glm::mat4& uniform)
	{
		GLint location = glGetUniformLocation(renderer_id, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(uniform));
	}

}

