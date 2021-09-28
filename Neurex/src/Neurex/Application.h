#pragma once

#include "Core.h"
#include "Window.h"
#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"
#include "Events/ApplicationEvent.h"
#include "LayerStack.h"
#include "imgui/ImGuiLayer.h"

#include "Neurex/renderer/Shader.h"
#include "Neurex/renderer/Buffer.h"
#include "Neurex/renderer/VertexArray.h"

namespace Neurex {

	class NX_API Application
	{
	public:
		Application();
		virtual ~Application();

		void run();

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

	private:
		static Application* instance;
	};

	Application* create_application();
}

