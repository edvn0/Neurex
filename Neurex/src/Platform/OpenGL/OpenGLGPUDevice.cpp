#include "Neurex/core/GPUDevice.h"
#include "nxpch.h"

#include <glad/glad.h>

namespace Neurex {

std::pair<int, int> GPUDevice::get_framebuffer_max_size()
{
	GLint dims[2];
	glGetIntegerv(GL_MAX_VIEWPORT_DIMS, &dims[0]);
	return { static_cast<int>(dims[0]), static_cast<int>(dims[1]) };
};

} // namespace Neurex
