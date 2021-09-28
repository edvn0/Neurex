#include "nxpch.h"
#include "Clock.h"

#include "Platform/GLFW/GLFWClock.h"

namespace Neurex {

	Clock* Clock::clock = new GLFWClock();

}