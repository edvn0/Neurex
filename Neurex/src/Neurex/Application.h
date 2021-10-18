#pragma once

#include "Core.h"
#include "LayerStack.h"
#include "Neurex/core/Timestep.h"
#include "Window.h"
#include "events/ApplicationEvent.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"
#include "imgui/ImGuiLayer.h"

namespace Neurex {

class Application {
public:
	Application();
	virtual ~Application();

	void run();

	void on_event(Event& event);

	void add_layer(Layer* layer);
	void add_overlay(Layer* overlay);

	static inline Application& the() { return *instance; }
	inline Window& get_window() { return *window; }

	inline bool exit()
	{
		is_running = false;
		return true;
	}

private:
	std::unique_ptr<Window> window;
	ImGuiLayer* imgui_layer;

	bool is_running = true;
	LayerStack stack;

	Timestep::TimeDelta last_time = 0.0f;

private:
	static Application* instance;
};

Application* create_application();
}
