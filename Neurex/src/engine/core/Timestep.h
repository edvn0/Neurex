#pragma once

namespace Engine {

class Timestep {
public:
	using TimeDelta = float;

public:
	Timestep(TimeDelta time_ = 0.0f)
		: time(time_){};

	TimeDelta get_seconds() { return time; };
	TimeDelta get_milli_seconds() { return time * 1e3f; };
	TimeDelta get_nano_seconds() { return time * 1e6f; };

private:
	TimeDelta time;
};

}
