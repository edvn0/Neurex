#pragma once

#include "Neurex/renderer/Buffer.h"

namespace Neurex {

class OpenGLVertexBuffer : public VertexBuffer {
public:
	OpenGLVertexBuffer(float* vertices, uint32_t size);
	virtual ~OpenGLVertexBuffer();

	virtual void bind() override;
	virtual void unbind() override;

	void set_layout(const BufferLayout& layout) override;
	const BufferLayout& get_layout() const override;

private:
	uint32_t renderer_id;
	BufferLayout layout;
};

class OpenGLIndexBuffer : public IndexBuffer {
public:
	OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
	virtual ~OpenGLIndexBuffer();

	virtual uint32_t get_count() const override { return count; }

	virtual void bind() override;
	virtual void unbind() override;

private:
	uint32_t renderer_id;
	uint32_t count;
};

}