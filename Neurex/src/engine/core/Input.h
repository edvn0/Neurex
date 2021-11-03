#pragma once

#include "Core.h"

namespace Engine {

class Input {
public:
	inline static bool is_key_pressed(int keycode) { return instance->is_key_pressed_impl(keycode); };

	inline static bool is_mouse_button_pressed(int button)
	{
		return instance->is_mouse_button_pressed_impl(button);
	};

	inline static float mouse_x() { return instance->mouse_x_impl(); };

	inline static float mouse_y() { return instance->mouse_y_impl(); };

	inline static std::pair<float, float> mouse_pos() { return instance->mouse_pos_impl(); };

protected:
	virtual bool is_key_pressed_impl(int keycode) = 0;
	virtual bool is_mouse_button_pressed_impl(int button) = 0;
	virtual float mouse_x_impl() = 0;
	virtual float mouse_y_impl() = 0;
	virtual std::pair<float, float> mouse_pos_impl() = 0;

private:
	static Input* instance;
};

}