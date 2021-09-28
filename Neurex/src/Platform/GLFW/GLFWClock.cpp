#include "nxpch.h"
#include "GLFWClock.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Neurex {

	Timestep::TimeDelta GLFWClock::get_time_milli_impl()
	{
		return glfwGetTime() * 1e3f;
	}

	Timestep::TimeDelta GLFWClock::get_time_impl()
	{
		return glfwGetTime();
	}

	Timestep::TimeDelta GLFWClock::get_time_nano_impl()
	{
		return glfwGetTime() * 1e6f;
	}

}