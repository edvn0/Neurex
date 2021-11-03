#pragma once

#include "engine/renderer/GraphicsContext.h"
#include <GLKit/GLKMath.h>
#include <Metal/Metal.h>
#include <QuartzCore/CAMetalLayer.h>

#define AUTOMATICALLY_RESIZE 1
#define CREATE_DEPTH_BUFFER 1

namespace Engine {

class MetalContext : public GraphicsContext {
public:
	MetalContext();
	~MetalContext() = default;

	virtual void init() override;
	virtual void swap_buffers() override;

private:
};

} // namespace Engine
