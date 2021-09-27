#pragma once

#include "Core.h"
#include "Window.h"
#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"
#include "Events/ApplicationEvent.h"
#include "LayerStack.h"
#include "imgui/ImGuiLayer.h"

namespace Neurex {

	class NX_API Application
	{
	public:
		Application();
		virtual ~Application();

		void start();

		void on_event(Event& event);

		void add_layer(Layer* layer);
		void add_overlay(Layer* overlay);

		static inline Application& the() { return *instance; }
		inline Window& get_window() { return *window; }
	private:
		std::unique_ptr<Window> window;
		ImGuiLayer* imgui_layer;

		bool is_running = true;
		LayerStack stack;

		unsigned int array_vertex, buffer_vertex, buffer_index;
	private:
		static Application* instance;
	};

	Application* create_application();
}

