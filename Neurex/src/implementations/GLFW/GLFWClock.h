#pragma once

#include "engine/core/Clock.h"

namespace Engine {

class GLFWClock : public Clock {
public:
	GLFWClock() = default;
	~GLFWClock() override = default;

protected:
	Timestep::TimeDelta get_time_milli_impl() override;

	Timestep::TimeDelta get_time_impl() override;

	Timestep::TimeDelta get_time_nano_impl() override;
};

}
