#pragma once

#include "Neurex/core/Clock.h"

namespace Neurex {

	class GLFWClock : public Clock
	{
	public:
		GLFWClock() = default;
		~GLFWClock() override = default;

	protected:
		Timestep::TimeDelta get_time_milli_impl() override;

		Timestep::TimeDelta get_time_impl() override;

		Timestep::TimeDelta get_time_nano_impl() override;
	};

}
