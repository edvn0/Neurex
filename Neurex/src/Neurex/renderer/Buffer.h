#pragma once

namespace Neurex {

	class VertexBuffer {
	public:
		virtual ~VertexBuffer() = default;

		virtual void bind() = 0;
		virtual void unbind() = 0;

		static VertexBuffer* create(float* vertices, size_t size);
	};

	class IndexBuffer {
	public:
		virtual ~IndexBuffer() = default;

		virtual void bind() = 0;
		virtual void unbind() = 0;
		virtual uint32_t get_count() const = 0;

		static IndexBuffer* create(uint32_t* indices, uint32_t count);
	};

}
