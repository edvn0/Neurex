#include "OpenGLBuffer.h"
#include "nxpch.h"

#include <glad/glad.h>

namespace Engine {

OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
{
	NX_PROFILE_FUNCTION();

	glGenBuffers(1, &renderer_id);
	glBindBuffer(GL_ARRAY_BUFFER, renderer_id);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
{
	NX_PROFILE_FUNCTION();
	glGenBuffers(1, &renderer_id);
	glBindBuffer(GL_ARRAY_BUFFER, renderer_id);
	glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() { glDeleteBuffers(1, &renderer_id); }

void OpenGLVertexBuffer::bind()
{
	NX_PROFILE_FUNCTION();
	glBindBuffer(GL_ARRAY_BUFFER, renderer_id);
}

void OpenGLVertexBuffer::unbind()
{
	NX_PROFILE_FUNCTION();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLVertexBuffer::set_layout(const BufferLayout& layout_) { layout = layout_; }

void OpenGLVertexBuffer::set_data(const void* data, uint32_t size)
{
	glBindBuffer(GL_ARRAY_BUFFER, renderer_id);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

const Engine::BufferLayout& OpenGLVertexBuffer::get_layout() const { return layout; }

OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count_)
	: count(count_)
{
	NX_PROFILE_FUNCTION();

	auto size = count * sizeof(uint32_t);
	glGenBuffers(1, &renderer_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}

OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t quads)
	: count(quads * 6)
{
	NX_PROFILE_FUNCTION();

	auto vertex_indices = quads * 6;
	uint32_t* quad_indices = new uint32_t[vertex_indices];
	size_t offset = 0;
	for (size_t i = 0; i < quads; i += 6) {
		quad_indices[i + 0] = offset + 0;
		quad_indices[i + 1] = offset + 1;
		quad_indices[i + 2] = offset + 2;

		quad_indices[i + 3] = offset + 2;
		quad_indices[i + 4] = offset + 3;
		quad_indices[i + 5] = offset + 0;

		offset += 4;
	};

	auto size = vertex_indices * sizeof(uint32_t);
	glGenBuffers(1, &renderer_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, quad_indices, GL_STATIC_DRAW);

	delete[] quad_indices;
};

OpenGLIndexBuffer::~OpenGLIndexBuffer() { glDeleteBuffers(1, &renderer_id); }

void OpenGLIndexBuffer::bind()
{
	NX_PROFILE_FUNCTION();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id);
}

void OpenGLIndexBuffer::unbind()
{
	NX_PROFILE_FUNCTION();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
}