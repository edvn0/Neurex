#pragma once

#include "engine/renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Engine {

class OpenGLContext : public GraphicsContext {
public:
	OpenGLContext(GLFWwindow* handle);

	virtual void init() override;
	virtual void swap_buffers() override;

private:
	GLFWwindow* window_handle;
};

}
