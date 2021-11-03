#include "Clock.h"
#include "nxpch.h"

#include "implementations/GLFW/GLFWClock.h"

namespace Engine {

Clock* Clock::clock = new GLFWClock();

}