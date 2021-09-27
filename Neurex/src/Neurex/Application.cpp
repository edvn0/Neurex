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

		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0,
			0.0f, 0.5f, 0.0f
		};

		vertex_buffer.reset(VertexBuffer::create(vertices, sizeof(vertices)));

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		// Index Buffer
		
		uint32_t indices[3] = { 0, 1, 2 };
		index_buffer.reset(IndexBuffer::create(indices, sizeof(indices)));

		// Shader

		std::string vertex_src, fragment_src;

		vertex_src = R"(
			#version 450 core
			
			layout(location = 0) in vec4 attrib_position;

			out vec4 vertex_position;

			void main() 
			{
				gl_Position = attrib_position;
				vertex_position = attrib_position;
			}
		)";

		fragment_src = R"(
			#version 450 core
			
			layout(location = 0) out vec4 output_colour;

			in vec4 vertex_position;

			void main()
			{
				output_colour = vertex_position * 0.5 + 0.5;
			}
		)";

		red_triangle_shader = std::make_unique<Shader>(vertex_src, fragment_src);
	};

	Application::~Application()
	{
	}

	void Application::start()
	{
		glClear(GL_COLOR_BUFFER_BIT);

		red_triangle_shader->bind();
		glBindVertexArray(array_vertex);
		glDrawElements(GL_TRIANGLES, index_buffer->get_count(), GL_UNSIGNED_INT, nullptr);

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
