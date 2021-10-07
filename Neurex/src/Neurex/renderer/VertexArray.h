#pragma once

#include "Buffer.h"
#include <memory>

namespace Neurex {

class VertexArray {
public:
	virtual ~VertexArray() = default;

	virtual void bind() = 0;
	virtual void unbind() = 0;

	virtual void add_vertex_buffer(ref<VertexBuffer>& vb) = 0;
	virtual void set_index_buffer(ref<IndexBuffer>& vb) = 0;

	virtual const std::vector<ref<VertexBuffer>>& get_vertex_buffers() const = 0;
	virtual const ref<IndexBuffer>& get_index_buffer() const = 0;

	static VertexArray* create();
};
};