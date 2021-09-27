#include "nxpch.h"

#include "Application.h"

#include <glad/glad.h>

namespace Neurex {


	Application::Application() : window(Window::create())
	{
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
	}

	void Application::add_overlay(Layer* overlay)
	{
		stack.push_overlay(overlay);
	}

}
