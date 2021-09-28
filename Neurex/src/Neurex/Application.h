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

		unsigned int array_vertex;

		std::shared_ptr<Shader> triangle_shader;
		std::shared_ptr<VertexArray> triangle_vertex_array;

		std::shared_ptr<Shader> square_shader;
		std::shared_ptr<VertexArray> square_vertex_array;
	private:
		static Application* instance;
	};

	Application* create_application();
}

