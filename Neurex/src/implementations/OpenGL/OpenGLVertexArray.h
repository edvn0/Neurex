#pragma once
#include "engine/renderer/VertexArray.h"

namespace Engine {

class OpenGLVertexArray : public VertexArray {
public:
	OpenGLVertexArray();
	~OpenGLVertexArray();

	void bind() override;
	void unbind() override;

	void add_vertex_buffer(ref<VertexBuffer>& vb) override;
	void set_index_buffer(ref<IndexBuffer>& vb) override;

	const std::vector<ref<VertexBuffer>>& get_vertex_buffers() const override;
	const ref<IndexBuffer>& get_index_buffer() const override;

private:
	std::vector<ref<VertexBuffer>> vertex_buffers;
	ref<IndexBuffer> index_buffer;
	unsigned int renderer_id = 0;
	uint32_t vertex_buffer_index = 0;
};

}