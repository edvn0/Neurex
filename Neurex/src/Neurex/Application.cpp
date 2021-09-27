#include "nxpch.h"

#include "Application.h"
#include "Input.h"

#include <glad/glad.h>

namespace Neurex {

	Application* Application::instance = nullptr;

	Application::Application()
	{
		NX_CORE_ASSERT(!instance, "Application already exists.");
		instance = this;

		window = std::unique_ptr<Window>(Window::create());
		window->set_event_callback(BEFn(Application::on_event));
	};

	Application::~Application()
	{
	}

	void Application::start()
	{
		glClearColor(0, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		while (is_running) {
			for (auto* l : stack) {
				l->updated();
			}

			if (Input::is_mouse_button_pressed(1)) {
				NX_CORE_INFO("{0}", "Clicked");
			}

			window->on_update();
		}
	}

	void Application::on_event(Event& event)
	{
		EventDispatcher dispatcher(event);

		dispatcher.dispatch_event<WindowCloseEvent>([&](WindowCloseEvent& e) {
			is_running = false;
			return true;
		});

		dispatcher.dispatch_event<WindowResizeEvent>([&](WindowResizeEvent& e) {
			window->resize_window(e.get_width(), e.get_height());
			return true;
		});

		for (auto it = stack.end(); it != stack.begin();) {
			(*--it)->on_event(event);
			if (event) {
				break;
			}
		}
	}

	void Application::add_layer(Layer* layer)
	{
		stack.push(layer);
		layer->attached();
	}

	void Application::add_overlay(Layer* overlay)
	{
		stack.push_overlay(overlay);
		overlay->attached();
	}

}
