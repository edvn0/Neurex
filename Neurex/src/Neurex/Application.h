#pragma once

#include "Core.h"
#include "Window.h"
#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"
#include "Events/ApplicationEvent.h"
#include "LayerStack.h"

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
	private:
		std::unique_ptr<Window> window;
		bool is_running = true;
		LayerStack stack;
	};

	Application* create_application();
}

