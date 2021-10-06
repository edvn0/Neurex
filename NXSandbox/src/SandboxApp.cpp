#include <Neurex.h>

using namespace Neurex;

class ExampleLayer : public Layer {
public:
	ExampleLayer()
		: Layer("Sandbox")
		, camera(-1.6f, 1.6f, -0.9f, 0.9f)
		, square_position(0.0)
		, triangle_position(0.0)
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
			-0.50f, -0.50f, 0.0f,
			0.50f, -0.50f, 0.0f,
			0.50f, 0.50f, 0.0f,
			-0.50f, 0.50f, 0.0f
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
			uniform mat4 uniform_transform;

			out vec3 vertex_position;

			void main()
			{
				vertex_position = attrib_position;
				gl_Position = uniform_view_projection * uniform_transform * vec4(attrib_position, 1.0);
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
			uniform mat4 uniform_transform;

			out vec3 vertex_position;
			out vec4 vertex_colour;

			void main()
			{
				vertex_position = attrib_position;
				vertex_colour = attrib_colour;
				gl_Position = uniform_view_projection * uniform_transform * vec4(attrib_position, 1.0);
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

	virtual void updated(Timestep ts) override
	{
		RenderCommand::set_clear_colour({ 0.2f, 0.2f, 0.2f, 1 });
		RenderCommand::clear();
		Renderer::begin_scene(camera);

		static auto scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		for (int i = 0; i < num_squares; i++) {
			for (int j = 0; j < num_squares; j++) {
				auto pos = glm::vec3(i * 0.11, j * 0.11, 0.0f);
				auto transform = glm::translate(glm::mat4(1.0), pos) * scale;
				Renderer::submit(square_vertex_array, square_shader, transform);
			}
		}
		Renderer::submit(triangle_vertex_array, triangle_shader, glm::translate(glm::mat4(1.0), triangle_position));
		Renderer::end_scene();
	}

	virtual void on_imgui_render() override
	{
	}

	virtual void on_event(Event& event) override
	{
		EventDispatcher dispatcher(event);

		dispatcher.dispatch_event<KeyPressedEvent>([&](KeyPressedEvent& ev) {
			if (ev.get_key_code() == NX_KC_Q) {
				camera.rotate(2.5f);
			}

			if (ev.get_key_code() == NX_KC_E) {
				camera.rotate(-2.5f);
			}

			if (ev.get_key_code() == NX_KC_W) {
				camera.move(-glm::vec3(0, 0.1, 0));
			}

			if (ev.get_key_code() == NX_KC_S) {
				camera.move(glm::vec3(0, 0.1, 0));
			}

			if (ev.get_key_code() == NX_KC_A) {
				camera.move(glm::vec3(0.1, 0, 0));
			}

			if (ev.get_key_code() == NX_KC_D) {
				camera.move(-glm::vec3(0.1, 0, 0));
			}

			if (ev.get_key_code() == NX_KC_R) {
				camera.set_position(glm::vec3(0, 0, 0));
			}

			if (ev.get_key_code() == NX_KC_X) {
				num_squares++;
			}

			if (ev.get_key_code() == NX_KC_Y) {
				num_squares--;
			}

			return false;
		});
	}

private:
	std::shared_ptr<Shader> triangle_shader;
	std::shared_ptr<VertexArray> triangle_vertex_array;

	std::shared_ptr<Shader> square_shader;
	std::shared_ptr<VertexArray> square_vertex_array;

	glm::vec3 square_position;
	glm::vec3 triangle_position;

	int num_squares = 20;

	OrthographicCamera camera;
};

class Sandbox : public Application {
public:
	Sandbox()
	{
		add_layer(new ExampleLayer());
	};

	~Sandbox(){};
};

Neurex::Application* Neurex::create_application()
{
	return new Sandbox();
}