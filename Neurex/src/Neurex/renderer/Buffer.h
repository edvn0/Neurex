#pragma once

#include <vector>

namespace Neurex {

enum class ShaderDataType { None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool };

static uint32_t shader_data_type_size(ShaderDataType type)
{
	switch (type) {
	case Neurex::ShaderDataType::None:
		return 0;
	case Neurex::ShaderDataType::Float:
		return 4;
	case Neurex::ShaderDataType::Float2:
		return 4 * 2;
	case Neurex::ShaderDataType::Float3:
		return 4 * 3;
	case Neurex::ShaderDataType::Float4:
		return 4 * 4;
	case Neurex::ShaderDataType::Mat3:
		return 4 * 3 * 3;
	case Neurex::ShaderDataType::Mat4:
		return 4 * 4 * 4;
	case Neurex::ShaderDataType::Int:
		return 4;
	case Neurex::ShaderDataType::Int2:
		return 4 * 2;
	case Neurex::ShaderDataType::Int3:
		return 4 * 3;
	case Neurex::ShaderDataType::Int4:
		return 4 * 4;
	case Neurex::ShaderDataType::Bool:
		return 1;
	}

	NX_CORE_ASSERT(false, "Unknown data type.");
	return 0;
}

struct BufferElement {
	std::string name;
	uint32_t offset;
	uint32_t size;
	bool normalization;
	ShaderDataType type;

	BufferElement(){};

	BufferElement(ShaderDataType type_, const std::string& name_, bool normalize = false)
		: name(name_)
		, type(type_)
		, size(shader_data_type_size(type_))
		, offset(0)
		, normalization(normalize){};

	uint32_t get_component_count() const
	{
		switch (type) {
		case Neurex::ShaderDataType::None:
			return 0;
		case Neurex::ShaderDataType::Float:
			return 1;
		case Neurex::ShaderDataType::Float2:
			return 2;
		case Neurex::ShaderDataType::Float3:
			return 3;
		case Neurex::ShaderDataType::Float4:
			return 4;
		case Neurex::ShaderDataType::Mat3:
			return 9;
		case Neurex::ShaderDataType::Mat4:
			return 16;
		case Neurex::ShaderDataType::Int:
			return 1;
		case Neurex::ShaderDataType::Int2:
			return 2;
		case Neurex::ShaderDataType::Int3:
			return 3;
		case Neurex::ShaderDataType::Int4:
			return 4;
		case Neurex::ShaderDataType::Bool:
			return 1;
		}

		NX_CORE_ASSERT(false, "Unknown data type.");
		return 0;
	}
};

class BufferLayout {
public:
	BufferLayout(){};
	BufferLayout(const std::initializer_list<BufferElement>& elmns)
		: elements(elmns)
	{
		calculate_offsets_stride();
	};

	inline const std::vector<BufferElement>& get_elements() const { return elements; }

	inline uint32_t const get_stride() const { return stride; };

	std::vector<BufferElement>::iterator begin() { return elements.begin(); };
	std::vector<BufferElement>::iterator end() { return elements.end(); };
	std::vector<BufferElement>::const_iterator begin() const { return elements.begin(); };
	std::vector<BufferElement>::const_iterator end() const { return elements.end(); };

private:
	void calculate_offsets_stride()
	{
		uint32_t offset = 0;
		stride = 0;
		for (auto& el : elements) {
			el.offset = offset;
			offset += el.size;
			stride += el.size;
		}
	}

private:
	std::vector<BufferElement> elements;
	uint32_t stride = 0;
};

class VertexBuffer {
public:
	virtual ~VertexBuffer() = default;

	virtual void bind() = 0;
	virtual void unbind() = 0;

	virtual void set_layout(const BufferLayout& layout) = 0;
	virtual void set_data(const void* data, uint32_t size) = 0;
	virtual const BufferLayout& get_layout() const = 0;

	static ref<VertexBuffer> create(float* vertices, uint32_t size);
	static ref<VertexBuffer> create(uint32_t size);
};

class IndexBuffer {
public:
	virtual ~IndexBuffer() = default;

	virtual void bind() = 0;
	virtual void unbind() = 0;
	virtual uint32_t get_count() const = 0;

	static ref<IndexBuffer> create(uint32_t* indices, uint32_t count);
};

}
