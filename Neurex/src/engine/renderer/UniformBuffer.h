#pragma once

#include "engine/core/Core.h"

namespace Engine {

class UniformBuffer {
public:
	virtual ~UniformBuffer() { }
	virtual void set_data(const void* data, uint32_t size, uint32_t offset = 0) = 0;

	static ref<UniformBuffer> create(uint32_t size, uint32_t binding);
};

}