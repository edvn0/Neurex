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

		triangle_vertex_array.reset(VertexArray::create());

		glClearColor(0.2f, 0.2f, 0.2f, 1);

		// Vertex Buffer

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.1f, 0.0f, 1.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.1f, 1.0f,
			0.0f, 0.5f, 0.0f, 1.0f, 0.5f, 0.2f, 1.0f
		};

		std::shared_ptr<VertexBuffer> triangle_vertex_buffer;
		triangle_vertex_buffer.reset(VertexBuffer::create(vertices, sizeof(vertices)));
		
		BufferLayout layout = {
			{ ShaderDataType::Float3, "attrib_position" },
			{ ShaderDataType::Float4, "attrib_colour" }
		};
		triangle_vertex_buffer->set_layout(layout);

		// Index Buffer

		triangle_vertex_array->add_vertex_buffer(triangle_vertex_buffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> triangle_index_buffer;
		triangle_index_buffer.reset(IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t)));
		triangle_vertex_array->set_index_buffer(triangle_index_buffer);

		
		// SQUARE

		float square_vertices[3 * 4] = {
			-0.85f, -0.85f, 0.0f,
			0.85f, -0.85f, 0.0f,
			0.85f, 0.85f, 0.0f,
			-0.85f, 0.85f, 0.0f
		};

		square_vertex_array.reset(VertexArray::create());

		std::shared_ptr<VertexBuffer> square_vb;
		square_vb.reset(VertexBuffer::create(square_vertices, sizeof(square_vertices)));

		BufferLayout square_layout = {
			{ ShaderDataType::Float3, "attrib_position" },
		};
		square_vb->set_layout(square_layout);
		square_vertex_array->add_vertex_buffer(square_vb);

		uint32_t square_indices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> square_index_buffer;
		square_index_buffer.reset(IndexBuffer::create(square_indices, sizeof(square_indices) / sizeof(uint32_t)));
		square_vertex_array->set_index_buffer(square_index_buffer);


		std::string vertex_src_square, fragment_src_square;

		vertex_src_square = R"(
			#version 330 core
			
			layout(location = 0) in vec3 attrib_position;

			out vec3 vertex_position;

			void main() 
			{
				vertex_position = attrib_position;
				gl_Position = vec4(attrib_position, 1.0);
			}
		)";

		fragment_src_square = R"(
			#version 330 core
			
			layout(location = 0) out vec4 output_colour;

			in vec3 vertex_position;
			in vec4 vertex_colour;

			void main()
			{
				output_colour = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";

		square_shader = std::make_shared<Shader>(vertex_src_square, fragment_src_square);

		// END SQUARE
		
		
		// Shader

		std::string vertex_src, fragment_src;

		vertex_src = R"(
			#version 330 core
			
			layout(location = 0) in vec3 attrib_position;
			layout(location = 1) in vec4 attrib_colour;

			out vec3 vertex_position;
			out vec4 vertex_colour;

			void main() 
			{
				vertex_position = attrib_position;
				vertex_colour = attrib_colour;
				gl_Position = vec4(attrib_position, 1.0);
			}
		)";

		fragment_src = R"(
			#version 330 core
			
			layout(location = 0) out vec4 output_colour;

			in vec3 vertex_position;
			in vec4 vertex_colour;

			void main()
			{
				output_colour = vec4(vertex_position, 1.0) * 0.5 + 0.5;
				output_colour = vertex_colour;
			}
		)";

		triangle_shader = std::make_shared<Shader>(vertex_src, fragment_src);
	};

	Application::~Application()
	{
	}

	void Application::start()
	{
		glClear(GL_COLOR_BUFFER_BIT);

		square_shader->bind();
		square_vertex_array->bind();
		glDrawElements(GL_TRIANGLES, square_vertex_array->get_index_buffer()->get_count(), GL_UNSIGNED_INT, nullptr);


		triangle_shader->bind();
		triangle_vertex_array->bind();
		glDrawElements(GL_TRIANGLES, triangle_vertex_array->get_index_buffer()->get_count(), GL_UNSIGNED_INT, nullptr);

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
