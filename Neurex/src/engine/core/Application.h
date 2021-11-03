#pragma once

#include "engine/core/Core.h"
#include "engine/core/LayerStack.h"
#include "engine/core/Timestep.h"
#include "engine/core/Window.h"
#include "engine/events/ApplicationEvent.h"
#include "engine/events/KeyEvent.h"
#include "engine/events/MouseEvent.h"
#include "engine/imgui/ImGuiLayer.h"

namespace Engine {

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
