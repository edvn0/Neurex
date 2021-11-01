#pragma once

#include "Neurex/core/Core.h"
#include "Neurex/core/LayerStack.h"
#include "Neurex/core/Timestep.h"
#include "Neurex/core/Window.h"
#include "Neurex/events/ApplicationEvent.h"
#include "Neurex/events/KeyEvent.h"
#include "Neurex/events/MouseEvent.h"
#include "Neurex/imgui/ImGuiLayer.h"

namespace Neurex {

class Application {
public:
	Application(const WindowProps& props);
	virtual ~Application();

	void run();

	void on_event(Event& event);

	void add_layer(scoped<Layer> layer);
	void add_overlay(scoped<Layer> overlay);

	static inline Application& the() { return *instance; }
	inline Window& get_window() { return *window; }
	inline float get_fps() { return fps; };

	const inline scoped<Layer>& get_imgui_layer() const { return stack.get_imgui_layer(); };

	inline bool exit()
	{
		is_running = false;
		return true;
	}

private:
	scoped<Window> window;
	scoped<ImGuiLayer> imgui_layer;

	bool is_running = true;
	LayerStack stack;

	Timestep::TimeDelta last_time = 0.0f;
	float fps = 0.0f;

private:
	static Application* instance;
};

Application* create_application();
}
