#include <Neurex.h>

using namespace Neurex;

class ExampleLayer : public Layer {
public:
	ExampleLayer(): Layer("Sandbox"), camera(-1.0f, 1.0f, -1.0f, 1.0f)
	{
		triangle_vertex_array.reset(VertexArray::create());


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

			uniform mat4 uniform_view_projection;

			out vec3 vertex_position;

			void main() 
			{
				vertex_position = attrib_position;
				gl_Position = uniform_view_projection * vec4(attrib_position, 1.0);
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

			uniform mat4 uniform_view_projection;

			out vec3 vertex_position;
			out vec4 vertex_colour;

			void main() 
			{
				vertex_position = attrib_position;
				vertex_colour = attrib_colour;
				gl_Position = uniform_view_projection * vec4(attrib_position, 1.0);
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
	}

	void updated() override {
		RenderCommand::set_clear_colour({ 0.2f, 0.2f, 0.2f, 1 });
		RenderCommand::clear();

		{
			Renderer::begin_scene();
			square_shader->bind();
			square_shader->upload_uniform_mat4("uniform_view_projection", camera.get_view_projection_matrix());
			Renderer::submit(square_vertex_array);
			triangle_shader->bind();
			triangle_shader->upload_uniform_mat4("uniform_view_projection", camera.get_view_projection_matrix());
			Renderer::submit(triangle_vertex_array);
			Renderer::end_scene();
		}
	}

	virtual void on_imgui_render() override {
	}

	void on_event(Event& event)
	{
	}
private:
	std::shared_ptr<Shader> triangle_shader;
	std::shared_ptr<VertexArray> triangle_vertex_array;

	std::shared_ptr<Shader> square_shader;
	std::shared_ptr<VertexArray> square_vertex_array;

	OrthographicCamera camera;
};


class Sandbox : public Application
{
public:
	Sandbox() {
		add_layer(new ExampleLayer());
	};
	
	~Sandbox() {};
};

Neurex::Application* Neurex::create_application() {
	return new Sandbox();
}