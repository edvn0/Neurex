#include "OpenGLVertexArray.h"
#include "nxpch.h"

#include <glad/glad.h>

namespace Neurex {

static GLenum nx_opengl_shader_type(ShaderDataType type)
{
	switch (type) {
	case Neurex::ShaderDataType::Float:
		return GL_FLOAT;
	case Neurex::ShaderDataType::Float2:
		return GL_FLOAT;
	case Neurex::ShaderDataType::Float3:
		return GL_FLOAT;
	case Neurex::ShaderDataType::Float4:
		return GL_FLOAT;
	case Neurex::ShaderDataType::Mat3:
		return GL_FLOAT;
	case Neurex::ShaderDataType::Mat4:
		return GL_FLOAT;
	case Neurex::ShaderDataType::Int:
		return GL_INT;
	case Neurex::ShaderDataType::Int2:
		return GL_INT;
	case Neurex::ShaderDataType::Int3:
		return GL_INT;
	case Neurex::ShaderDataType::Int4:
		return GL_INT;
	case Neurex::ShaderDataType::Bool:
		return GL_BOOL;
	case Neurex::ShaderDataType::None: {
		NX_CORE_ASSERT(false, "Unknown data type.");
		return GL_INT;
	}
	}

	NX_CORE_ASSERT(false, "Unknown data type.");
	return 0;
}

OpenGLVertexArray::OpenGLVertexArray()
{
	glGenVertexArrays(1, &renderer_id);
	// glCreateVertexArrays(1, &renderer_id);
}

OpenGLVertexArray::~OpenGLVertexArray()
{
	glDeleteVertexArrays(1, &renderer_id);
}

void OpenGLVertexArray::bind()
{
	glBindVertexArray(renderer_id);
}

void OpenGLVertexArray::unbind()
{
	glBindVertexArray(0);
}

void OpenGLVertexArray::add_vertex_buffer(std::shared_ptr<VertexBuffer>& vb)
{
	glBindVertexArray(renderer_id);
	vb->bind();

	uint32_t index = 0;
	const auto& layout = vb->get_layout();
	for (const auto& l : layout) {
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index,
			l.get_component_count(),
			nx_opengl_shader_type(l.type),
			l.normalization ? GL_TRUE : GL_FALSE,
			layout.get_stride(),
			(const void*)l.offset);
		index++;
	}

	vertex_buffers.push_back(vb);
}

void OpenGLVertexArray::set_index_buffer(std::shared_ptr<IndexBuffer>& ib)
{
	glBindVertexArray(renderer_id);
	ib->bind();

	index_buffer = ib;
}

const std::vector<std::shared_ptr<Neurex::VertexBuffer>>& OpenGLVertexArray::get_vertex_buffers() const
{
	return vertex_buffers;
}

const std::shared_ptr<Neurex::IndexBuffer>& OpenGLVertexArray::get_index_buffer() const
{
	return index_buffer;
}

}