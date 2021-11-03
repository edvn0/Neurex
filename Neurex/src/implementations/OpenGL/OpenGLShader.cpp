#include "nxpch.h"

#include "OpenGLShader.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>

namespace Engine {

static GLenum shader_type_from_string(const std::string& type)
{
	if (type == "vertex") {
		return GL_VERTEX_SHADER;
	} else if (type == "fragment" || type == "pixel") {
		return GL_FRAGMENT_SHADER;
	}

	NX_CORE_ASSERT(false, "unknown shader type");
	return 0;
}

OpenGLShader::OpenGLShader(const std::string& path)
{
	NX_PROFILE_FUNCTION();
	auto read = read_file(path);
	auto processed = process_shader_file(read);
	compile_shader(processed);

	std::filesystem::path path_ = path;
	name = path_.stem().string();
}

OpenGLShader::OpenGLShader(const std::string& name_, const std::string& vertex, const std::string& fragment)
	: name(name_)
{
	NX_PROFILE_FUNCTION();
	std::unordered_map<GLenum, std::string> map
		= { { GL_VERTEX_SHADER, vertex }, { GL_FRAGMENT_SHADER, fragment } };
	compile_shader(map);
}

OpenGLShader::~OpenGLShader()
{
	NX_PROFILE_FUNCTION();
	glDeleteProgram(renderer_id);
}

void OpenGLShader::compile_shader(std::unordered_map<GLenum, std::string> sources)
{
	NX_PROFILE_FUNCTION();
	GLuint program = glCreateProgram();

	std::array<GLenum, 2> shader_ids;
	int shader_index = 0;
	for (const auto& [type, src] : sources) {
		GLuint shader = glCreateShader(type);
		const GLchar* source = src.c_str();
		glShaderSource(shader, 1, &source, 0);

		glCompileShader(shader);

		GLint isCompiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

			glDeleteShader(shader);

			NX_CORE_ERROR(
				"Shader Type: {0}. \nInfo: {1}", type == GL_VERTEX_SHADER ? "Vertex" : "Fragment", infoLog.data());
			NX_CORE_ASSERT(false, "OpenGLShader compilation failure.")
			break;
		}

		glAttachShader(program, shader);
		shader_ids[shader_index++] = shader;
	}

	// Vertex and fragment shaders are successfully compiled.
	// Now time to link them together into a program.
	// Get a program object.

	// Link our renderer_id
	glLinkProgram(program);

	// Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE) {
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

		// We don't need the renderer_id anymore.
		glDeleteProgram(program);

		for (auto& val : shader_ids) {
			glDeleteShader(val);
		}

		NX_CORE_ERROR("{0}", infoLog.data());
		NX_CORE_ASSERT(false, "OpenGLShader linking failure.");
	}

	for (auto& val : shader_ids) {
		glDetachShader(program, val);
	}
	renderer_id = program;
}

std::string OpenGLShader::read_file(const std::string& path)
{
	std::string result;
	auto file_stream = std::ifstream(path, std::ios::in | std::ios::binary);
	if (!file_stream) {
		NX_ASSERT(false, "Could not open file.");
		return result;
	}

	file_stream.seekg(0, std::ios::end);
	result.resize(file_stream.tellg());
	file_stream.seekg(0, std::ios::beg);
	file_stream.read(&result[0], result.size());
	file_stream.close();

	return result;
}

std::unordered_map<GLenum, std::string> OpenGLShader::process_shader_file(const std::string& src)
{
	std::unordered_map<GLenum, std::string> sources;

	const char* token = "#type";
	size_t type_token_len = strlen(token);
	size_t pos = src.find(token, 0);
	while (pos != std::string::npos) {
		size_t eol = src.find_first_of("\n", pos);
		NX_CORE_ASSERT(eol != std::string::npos, "Syntax error.");
		size_t begin = pos + type_token_len + 1;
		std::string type = src.substr(begin, eol - begin);
		size_t next_line_pos = src.find_first_not_of("\n", eol);
		pos = src.find(token, next_line_pos);
		sources[shader_type_from_string(type)] = src.substr(
			next_line_pos, pos - (next_line_pos == std::string::npos ? src.size() - 1 : next_line_pos));
	}

	return sources;
}

void OpenGLShader::bind()
{
	NX_PROFILE_FUNCTION();
	glUseProgram(renderer_id);
}

void OpenGLShader::unbind()
{
	NX_PROFILE_FUNCTION();
	glUseProgram(0);
}

void OpenGLShader::upload_uniform(const std::string& name, const glm::mat4& uniform)
{
	NX_PROFILE_FUNCTION();
	GLint location = glGetUniformLocation(renderer_id, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(uniform));
}

void OpenGLShader::upload_uniform(const std::string& name, const glm::vec4& uniform)
{
	NX_PROFILE_FUNCTION();
	GLint location = glGetUniformLocation(renderer_id, name.c_str());
	glUniform4f(location, uniform.x, uniform.y, uniform.z, uniform.w);
}

void OpenGLShader::upload_uniform(const std::string& name, const glm::vec3& uniform)
{
	NX_PROFILE_FUNCTION();
	GLint location = glGetUniformLocation(renderer_id, name.c_str());
	glUniform3f(location, uniform.x, uniform.y, uniform.z);
}

void OpenGLShader::upload_uniform(const std::string& name, const glm::vec2& uniform)
{
	NX_PROFILE_FUNCTION();
	GLint location = glGetUniformLocation(renderer_id, name.c_str());
	glUniform2f(location, uniform.x, uniform.y);
}

void OpenGLShader::upload_uniform(const std::string& name, float uniform)
{
	NX_PROFILE_FUNCTION();
	GLint location = glGetUniformLocation(renderer_id, name.c_str());
	glUniform1f(location, uniform);
}

void OpenGLShader::upload_uniform(const std::string& name, int uniform)
{
	NX_PROFILE_FUNCTION();
	GLint location = glGetUniformLocation(renderer_id, name.c_str());
	glUniform1i(location, uniform);
}

void OpenGLShader::upload_uniform(const std::string& name, int* uniform, uint32_t count)
{
	NX_PROFILE_FUNCTION();
	GLint location = glGetUniformLocation(renderer_id, name.c_str());
	NX_CORE_INFO("Shader Location: {0}", location);
	glUniform1iv(location, count, uniform);
}

void OpenGLShader::set_float4(const std::string& name, const glm::vec4& vec)
{
	NX_PROFILE_FUNCTION();
	upload_uniform(name, vec);
}

void OpenGLShader::set_float3(const std::string& name, const glm::vec3& vec)
{
	NX_PROFILE_FUNCTION();
	upload_uniform(name, vec);
}

void OpenGLShader::set_mat4(const std::string& name, const glm::mat4& mat4)
{
	NX_PROFILE_FUNCTION();
	upload_uniform(name, mat4);
}
}
