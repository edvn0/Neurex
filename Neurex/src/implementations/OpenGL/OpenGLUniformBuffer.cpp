#include "OpenGLUniformBuffer.h"
#include "nxpch.h"

#include <glad/glad.h>

namespace Engine {

OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size, uint32_t binding)
{
	glGenBuffers(1, &renderer_id);
	glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW); // TODO: investigate usage hint
	glBindBufferBase(GL_UNIFORM_BUFFER, binding, renderer_id);
}

OpenGLUniformBuffer::~OpenGLUniformBuffer() { glDeleteBuffers(1, &renderer_id); }

void OpenGLUniformBuffer::set_data(const void* data, uint32_t size, uint32_t offset)
{
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}

} // namespace Engine
