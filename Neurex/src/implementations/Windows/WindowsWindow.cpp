#include "WindowsWindow.h"
#include "nxpch.h"

#include "engine/events/ApplicationEvent.h"
#include "engine/events/KeyEvent.h"
#include "engine/events/MouseEvent.h"
#include "implementations/OpenGL/OpenGLContext.h"

namespace Engine {
static bool is_glfw_initialized = false;

static void glfw_error_callback(int error, const char* desc)
{
	NX_CORE_ERROR("GLFW Error: ({0}), info: {1}", error, desc);
}

Window* Window::create(const WindowProps& props) { return new WindowsWindow(props); };

WindowsWindow::WindowsWindow(const WindowProps& props) { init(props); };

WindowsWindow::~WindowsWindow() { shutdown(); }

void WindowsWindow::on_update()
{
	glfwPollEvents();
	window_context->swap_buffers();
};

void WindowsWindow::set_vsync(bool enabled)
{
	if (enabled) {
		glfwSwapInterval(1);
	} else {
		glfwSwapInterval(0);
	}
	window_data.vsync = enabled;
}

bool WindowsWindow::is_vsync() { return window_data.vsync; }

void WindowsWindow::resize_window(float w, float h) const { glfwSetWindowSize(win_window, (int)w, (int)h); }

void WindowsWindow::init(const WindowProps& props)
{
	window_data.title = props.title;
	window_data.width = props.width;
	window_data.height = props.height;

	NX_CORE_INFO("Creating a new window {0} ({1}, {2})", props.title, props.width, props.height);

	if (!is_glfw_initialized) {
		int success = glfwInit();

		glfwSetErrorCallback(glfw_error_callback);

		NX_CORE_ASSERT("Initialization works: {0}", success);
		is_glfw_initialized = true;
	}

	win_window
		= glfwCreateWindow((int)props.width, (int)props.height, window_data.title.c_str(), nullptr, nullptr);

	window_context = new OpenGLContext(win_window);
	window_context->init();

	glfwSetWindowUserPointer(win_window, &window_data);
	set_vsync(false);
	setup_events();
};

void WindowsWindow::shutdown() { glfwDestroyWindow(win_window); };

void WindowsWindow::setup_events()
{
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
