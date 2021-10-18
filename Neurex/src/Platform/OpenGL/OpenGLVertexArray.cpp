#include "OpenGLVertexArray.h"
#include "nxpch.h"

#include <glad/glad.h>

namespace Neurex {

static GLenum nx_opengl_shader_type(ShaderDataType type)
{
	switch (type) {
	case Neurex::ShaderDataType::Float:
	case Neurex::ShaderDataType::Float2:
	case Neurex::ShaderDataType::Float3:
	case Neurex::ShaderDataType::Float4:
	case Neurex::ShaderDataType::Mat3:
	case Neurex::ShaderDataType::Mat4:
		return GL_FLOAT;
	case Neurex::ShaderDataType::Int:
	case Neurex::ShaderDataType::Int2:
	case Neurex::ShaderDataType::Int3:
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
	glBindVertexArray(renderer_id);
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

void OpenGLVertexArray::add_vertex_buffer(ref<VertexBuffer>& vb)
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
			(void*)l.offset);
		index++;
	}

	vertex_buffers.push_back(vb);
}

void OpenGLVertexArray::set_index_buffer(ref<IndexBuffer>& ib)
{
	glBindVertexArray(renderer_id);
	ib->bind();

	index_buffer = ib;
}

const std::vector<ref<Neurex::VertexBuffer>>& OpenGLVertexArray::get_vertex_buffers() const
{
	return vertex_buffers;
}

const ref<Neurex::IndexBuffer>& OpenGLVertexArray::get_index_buffer() const
{
	return index_buffer;
}

}