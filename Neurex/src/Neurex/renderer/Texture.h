#pragma once

#include "Neurex/core/Core.h"
#include <iostream>

namespace Neurex {

class Texture {
public:
	virtual ~Texture() = default;
	virtual uint32_t get_width() const = 0;
	virtual uint32_t get_height() const = 0;

	virtual void set_data(void* data, uint32_t size) = 0;

	virtual void bind(uint32_t slot = 0) const = 0;
	virtual void unbind() const = 0;

	virtual uint32_t get_renderer_id() const = 0;

	virtual bool compare(const Texture& other) const = 0;
	virtual bool operator==(const Texture& other) const = 0;
};

class Texture2D : public Texture {
public:
	static ref<Texture2D> create(const std::string& path);
	static ref<Texture2D> create(uint32_t width, uint32_t height);
};

} // namespace Neurex
