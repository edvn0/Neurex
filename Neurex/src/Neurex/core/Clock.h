#pragma once

#include "Timestep.h"

namespace Neurex {
class Clock {
public:
	virtual ~Clock() = default;

	static inline Timestep::TimeDelta get_time_milli() { return clock->get_time_milli_impl(); }

	static inline Timestep::TimeDelta get_time() { return clock->get_time_impl(); }

	static inline Timestep::TimeDelta get_time_nano() { return clock->get_time_nano_impl(); }

protected:
	virtual Timestep::TimeDelta get_time_milli_impl() = 0;
	virtual Timestep::TimeDelta get_time_impl() = 0;
	virtual Timestep::TimeDelta get_time_nano_impl() = 0;

private:
	static Clock* clock;
};
}