#include "nxpch.h"
#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace Neurex {

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
	{
		glCreateBuffers(1, &renderer_id);
		glBindBuffer(GL_ARRAY_BUFFER, renderer_id);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &renderer_id);
	}

	void OpenGLVertexBuffer::bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, renderer_id);
	}

	void OpenGLVertexBuffer::unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count_) : count(count_)
	{
		auto size = count * sizeof(uint32_t);
		glCreateBuffers(1, &renderer_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &renderer_id);
	}

	void OpenGLIndexBuffer::bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id);
	}

	void OpenGLIndexBuffer::unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}