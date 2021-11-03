#pragma once

#include "engine/core/Core.h"
#include "engine/renderer/UniformBuffer.h"

namespace Engine {

class OpenGLUniformBuffer : public UniformBuffer {
public:
	OpenGLUniformBuffer(uint32_t size, uint32_t binding);
	virtual ~OpenGLUniformBuffer() override;
	virtual void set_data(const void* data, uint32_t size, uint32_t offset = 0) override;

private:
	uint32_t renderer_id;
};
}