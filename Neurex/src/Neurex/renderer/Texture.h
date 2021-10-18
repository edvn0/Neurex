#pragma once

#include "Neurex/core/Core.h"

namespace Neurex {

class Texture {
public:
	virtual ~Texture() = default;
	virtual uint32_t get_width() const = 0;
	virtual uint32_t get_height() const = 0;

	virtual void bind(uint32_t slot = 0) const = 0;
};

class Texture2D : public Texture {
public:
	static ref<Texture2D> create(const std::string& path);
};

} // namespace Neurex
