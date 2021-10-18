#include "MacOSInput.h"
#include "nxpch.h"

#include "Neurex/core/Application.h"
#include <GLFW/glfw3.h>

namespace Neurex {

#ifdef NX_PT_WIN
Input* Input::instance = nullptr;
#elif defined(NX_PT_OSX)
Input* Input::instance = new MacOSInput();
#endif

bool MacOSInput::is_key_pressed_impl(int keycode)
{
	auto window = static_cast<GLFWwindow*>(Application::the().get_window().get_natively());
	auto state = glfwGetKey(window, keycode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool MacOSInput::is_mouse_button_pressed_impl(int button)
{
	auto window = static_cast<GLFWwindow*>(Application::the().get_window().get_natively());
	auto state = glfwGetMouseButton(window, button);
	return state == GLFW_PRESS;
}

float MacOSInput::mouse_x_impl()
{
	auto [x, y] = mouse_pos_impl();
	return y;
}

float MacOSInput::mouse_y_impl()
{
	auto [x, y] = mouse_pos_impl();
	return y;
}

std::pair<float, float> MacOSInput::mouse_pos_impl()
{
	auto window = static_cast<GLFWwindow*>(Application::the().get_window().get_natively());
	double x;
	double y;
	glfwGetCursorPos(window, &x, &y);
	return { (float)x, float(y) };
}

}
