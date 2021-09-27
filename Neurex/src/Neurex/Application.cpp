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
		imgui_layer = new ImGuiLayer();
		add_overlay(imgui_layer);

		glClearColor(0.2f, 0.2f, 0.2f, 1);

		// Vertex Array
		
		glGenVertexArrays(1, &array_vertex);
		glBindVertexArray(array_vertex);

		// Vertex Buffer
		
		glGenBuffers(1, &buffer_vertex);
		glBindBuffer(GL_ARRAY_BUFFER, buffer_vertex);

		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0,
			0.0f, 0.5f, 0.0f
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		// Index Buffer
		
		glGenBuffers(1, &buffer_index);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_index);
		
		unsigned int indices[3] = { 0,1,2 };

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// Shader
	};

	Application::~Application()
	{
	}

	void Application::start()
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(array_vertex);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

		while (is_running) {
			for (auto* l : stack) {
				l->updated();
			}

			imgui_layer->begin();
			for (auto* l : stack) {
				l->on_imgui_render();
			}			
			imgui_layer->end();
			
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
