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
	for (const auto& element : layout) {
		switch (element.type) {
		case ShaderDataType::Float:
		case ShaderDataType::Float2:
		case ShaderDataType::Float3:
		case ShaderDataType::Float4: {
			glEnableVertexAttribArray(m_VertexBufferIndex);
			glVertexAttribPointer(m_VertexBufferIndex,
				element.get_component_count(),
				nx_opengl_shader_type(element.type),
				element.normalization ? GL_TRUE : GL_FALSE,
				layout.get_stride(),
				(const void*)element.offset);
			m_VertexBufferIndex++;
			break;
		}
		case ShaderDataType::Int:
		case ShaderDataType::Int2:
		case ShaderDataType::Int3:
		case ShaderDataType::Int4:
		case ShaderDataType::Bool: {
			glEnableVertexAttribArray(m_VertexBufferIndex);
			glVertexAttribIPointer(m_VertexBufferIndex,
				element.get_component_count(),
				nx_opengl_shader_type(element.type),
				layout.get_stride(),
				(const void*)element.offset);
			m_VertexBufferIndex++;
			break;
		}
		case ShaderDataType::Mat3:
		case ShaderDataType::Mat4: {
			uint8_t count = element.get_component_count();
			for (uint8_t i = 0; i < count; i++) {
				glEnableVertexAttribArray(m_VertexBufferIndex);
				glVertexAttribPointer(m_VertexBufferIndex,
					count,
					nx_opengl_shader_type(element.type),
					element.normalization ? GL_TRUE : GL_FALSE,
					layout.get_stride(),
					(const void*)(element.offset + sizeof(float) * count * i));
				glVertexAttribDivisor(m_VertexBufferIndex, 1);
				m_VertexBufferIndex++;
			}
			break;
		}
		default:
			NX_CORE_ASSERT(false, "Unknown ShaderDataType!");
		}
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