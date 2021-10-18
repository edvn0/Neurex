#pragma once

#include "Neurex/core/Input.h"

namespace Neurex {

class MacOSInput : public Input {
protected:
	virtual bool is_key_pressed_impl(int keycode) override;

	virtual bool is_mouse_button_pressed_impl(int button) override;

	virtual float mouse_x_impl() override;

	virtual float mouse_y_impl() override;

	virtual std::pair<float, float> mouse_pos_impl() override;
};

}
