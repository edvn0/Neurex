#include "nxpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Neurex {

	OpenGLContext::OpenGLContext(GLFWwindow* handle) : window_handle(handle) 
	{

	}

	void OpenGLContext::init()
	{
		glfwMakeContextCurrent(window_handle);
		int glad_status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		NX_CORE_INFO("Glad initialized with status {0}", glad_status);
		NX_CORE_ASSERT(glad_status, "Failed to initialize glad.");

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