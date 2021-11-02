#pragma once

#include "Neurex/core/Core.h"
#include "Neurex/renderer/UniformBuffer.h"

namespace Neurex {

class OpenGLUniformBuffer : public UniformBuffer {
public:
	OpenGLUniformBuffer(uint32_t size, uint32_t binding);
	virtual ~OpenGLUniformBuffer() override;
	virtual void set_data(const void* data, uint32_t size, uint32_t offset = 0) override;

private:
	uint32_t renderer_id;
};
}