#include "MacOSWindow.h"
#include "nxpch.h"

#include "Neurex/events/ApplicationEvent.h"
#include "Neurex/events/KeyEvent.h"
#include "Neurex/events/MouseEvent.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Neurex {
static bool is_glfw_initialized = false;

static void glfw_error_callback(int error, const char* desc)
{
	NX_CORE_ERROR("GLFW Error: ({0}), info: {1}", error, desc);
}

Window* Window::create(const WindowProps& props) { return new MacOSWindow(props); };

MacOSWindow::MacOSWindow(const WindowProps& props)
{
	NX_PROFILE_FUNCTION();
	init(props);
};

MacOSWindow::~MacOSWindow()
{
	NX_PROFILE_FUNCTION();
	glfwDestroyWindow(win_window);
}

void MacOSWindow::on_update()
{
	NX_PROFILE_FUNCTION();

	glfwPollEvents();
	window_context->swap_buffers();
};

void MacOSWindow::set_vsync(bool enabled)
{
	NX_PROFILE_FUNCTION();

	if (enabled) {
		glfwSwapInterval(1);
	} else {
		glfwSwapInterval(0);
	}
	window_data.vsync = enabled;
}

bool MacOSWindow::is_vsync() { return window_data.vsync; }

void MacOSWindow::resize_window(float w, float h) const
{
	NX_PROFILE_FUNCTION();

	glfwSetWindowSize(win_window, (int)w, (int)h);
}

void MacOSWindow::resize_framebuffer(int w, int h) const
{
	NX_PROFILE_FUNCTION();
	glViewport(0, 0, w * pixel_size_x, h * pixel_size_y);
}

void MacOSWindow::init(const WindowProps& props)
{
	NX_PROFILE_FUNCTION();

	window_data.title = props.title;
	window_data.width = props.width;
	window_data.height = props.height;

	NX_CORE_INFO("Creating a new MacOSX window {0} ({1}, {2})", props.title, props.width, props.height);

	if (!is_glfw_initialized) {
		int success = glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GL_TRUE);

		glfwSetErrorCallback(glfw_error_callback);

		NX_CORE_ASSERT("Initialization works: {0}", success);
		is_glfw_initialized = true;
	}

	win_window
		= glfwCreateWindow((int)props.width, (int)props.height, window_data.title.c_str(), nullptr, nullptr);
	window_context = new OpenGLContext(win_window);
	window_context->init();

	glfwGetWindowContentScale(win_window, &pixel_size_x, &pixel_size_y);

	NX_CORE_INFO("Pixel density: [{0} X {1}]", pixel_size_x, pixel_size_y);

	glViewport(0, 0, props.width * pixel_size_x, props.height * pixel_size_y);
	glfwSetWindowUserPointer(win_window, &window_data);
	set_vsync(true);
	setup_events();
};

void MacOSWindow::shutdown()
{
	NX_PROFILE_FUNCTION();
	glfwDestroyWindow(win_window);
};

void MacOSWindow::setup_events()
{
	NX_PROFILE_FUNCTION();

	glfwSetFramebufferSizeCallback(win_window, [](GLFWwindow* window, int w, int h) {
		auto user_ptr = *(WindowData*)glfwGetWindowUserPointer(window);

		WindowFramebufferEvent event(w, h);
		user_ptr.width = w;
		user_ptr.height = h;
		user_ptr.callback(event);
	});

	glfwSetWindowSizeCallback(win_window, [](GLFWwindow* window, int width, int height) {
		auto user_ptr = *(WindowData*)glfwGetWindowUserPointer(window);

		WindowResizeEvent event((float)width, (float)height);
		user_ptr.width = width;
		user_ptr.height = height;
		user_ptr.callback(event);
	});

	glfwSetWindowCloseCallback(win_window, [](GLFWwindow* window) {
		auto user_ptr = *(WindowData*)glfwGetWindowUserPointer(window);
		WindowCloseEvent closed;
		user_ptr.callback(closed);
	});

	glfwSetKeyCallback(win_window, [](GLFWwindow* window, int key, int scancode, int action, int modes) {
		auto user_ptr = *(WindowData*)glfwGetWindowUserPointer(window);

		switch (action) {
		case GLFW_PRESS: {
			KeyPressedEvent event(key, 0);
			user_ptr.callback(event);
			break;
		}
		case GLFW_RELEASE: {
			KeyReleasedEvent event(key);
			user_ptr.callback(event);
			break;
		}
		case GLFW_REPEAT: {
			KeyPressedEvent event(key, 1);
			user_ptr.callback(event);
			break;
		}
		}
	});

	glfwSetMouseButtonCallback(win_window, [](GLFWwindow* window, int button, int action, int mods) {
		auto user_ptr = *(WindowData*)glfwGetWindowUserPointer(window);

		double x, y;
		glfwGetCursorPos(window, &x, &y);

		switch (action) {
		case GLFW_PRESS: {
			MouseButtonPressedEvent event(button, x, y);
			user_ptr.callback(event);
			break;
		}
		case GLFW_RELEASE: {
			MouseButtonReleasedEvent event(button, x, y);
			user_ptr.callback(event);
			break;
		}
		}
	});

	glfwSetCharCallback(win_window, [](GLFWwindow* window, unsigned int c) {
		auto user_ptr = *(WindowData*)glfwGetWindowUserPointer(window);
		KeyTypedEvent event(c);
		user_ptr.callback(event);
	});

	glfwSetScrollCallback(win_window, [](GLFWwindow* window, double xo, double yo) {
		auto user_ptr = *(WindowData*)glfwGetWindowUserPointer(window);
		MouseScrolledEvent event((float)xo, (float)yo);
		user_ptr.callback(event);
	});

	glfwSetCursorPosCallback(win_window, [](GLFWwindow* window, double xpos, double ypos) {
		auto user_ptr = *(WindowData*)glfwGetWindowUserPointer(window);
		MouseMovedEvent event((float)xpos, (float)ypos);
		user_ptr.callback(event);
	});
}

}
