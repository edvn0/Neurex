#include "OpenGLContext.h"
#include "nxpch.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Neurex {

OpenGLContext::OpenGLContext(GLFWwindow* handle)
	: window_handle(handle)
{
}

void OpenGLContext::init()
{
	glfwMakeContextCurrent(window_handle);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		NX_CORE_ASSERT(false, "Glad failed.");
	}
	if (!gladLoadGL()) {
		NX_CORE_ASSERT(false, "gladLoadGL failed.");
	}
	NX_CORE_INFO("Glad successfully initialized.");

	auto* vendor = glGetString(GL_VENDOR);
	auto* renderer = glGetString(GL_RENDERER);
	auto* version = glGetString(GL_VERSION);

	NX_CORE_INFO("OpenGL [ Vendor: {0}, Renderer: {1} ]\n\tVersion: {2}", vendor, renderer, version);
}

void OpenGLContext::swap_buffers()
{
	glfwSwapBuffers(window_handle);
}

}