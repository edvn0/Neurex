#include "Clock.h"
#include "nxpch.h"

#include "Platform/GLFW/GLFWClock.h"

namespace Neurex {

Clock* Clock::clock = new GLFWClock();

}