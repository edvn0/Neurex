#include "nxpch.h"
#include "WindowsWindow.h"

namespace Neurex
{
	static bool is_glfw_initialized = false;

	Window* Window::create(const WindowProps& props) {
		return new WindowsWindow(props);
	};

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		init(props);
	};

	WindowsWindow::~WindowsWindow()
	{
		shutdown();
	};

	void WindowsWindow::set_vsync(bool enabled)
	{
		if (enabled) {
			glfwSwapInterval(1);
		}
		else {
			glfwSwapInterval(0);
		}
		window_data.vsync = enabled;
	}

	void WindowsWindow::init(const WindowProps& props)
	{
		window_data.title = props.title;
		window_data.width = props.width;
		window_data.height = props.height;

		NX_CORE_INFO("Creating a new window {0} ({1}, {2})", props.title, props.width, props.height);

		if (!is_glfw_initialized) {
			int success = glfwInit();
			NX_CORE_ERROR("Initialization works: {0}", success);
			is_glfw_initialized = true;
		}

		win_window = glfwCreateWindow((int)props.width, (int)props.height, window_data.title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(win_window);
		glfwSetWindowUserPointer(win_window, &window_data);
		set_vsync(true);
	};

	void WindowsWindow::shutdown() {
		glfwDestroyWindow(win_window);
	};
}
