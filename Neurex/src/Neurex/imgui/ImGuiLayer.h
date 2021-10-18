#pragma once

#include "Neurex/core/Layer.h"

namespace Neurex {

class ImGuiLayer : public Layer {
public:
	ImGuiLayer();
	~ImGuiLayer();

	void attached() override;
	void detached() override;
	void on_event(Event& event) override;
	void on_imgui_render() override;

	void begin();
	void end();

private:
	float current_time = 0.0f;
};

}
