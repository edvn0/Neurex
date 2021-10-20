#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Sandbox2D.h"

using namespace Neurex;

class ExampleLayer : public Layer {
public:
	ExampleLayer()
		: Layer("Sandbox")
		, controller(1280.0f / 720.0f)
	{
		// SQUARE
		float vertices[] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		ref<VertexBuffer> vertex_buffer = VertexBuffer::create(vertices, sizeof(vertices));

		BufferLayout layout = { { ShaderDataType::Float3, "attrib_position" },
			{ ShaderDataType::Float4, "attrib_color" } };

		vertex_buffer->set_layout(layout);

		// Index buffer
		uint32_t indices[3] = { 0, 1, 2 };
		ref<IndexBuffer> index_buffer = IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t));

		triangle_vertex_array = VertexArray::create();
		triangle_vertex_array->add_vertex_buffer(vertex_buffer);
		triangle_vertex_array->set_index_buffer(index_buffer);

		// Square
		square_vertex_array = VertexArray::create();

		float square_vertices[] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f, 0.0f, 1.0f
		};

		ref<VertexBuffer> square_vb = VertexBuffer::create(square_vertices, sizeof(square_vertices));
		square_vb->set_layout({ { ShaderDataType::Float3, "attrib_position" },
			{ ShaderDataType::Float2, "vertex_tex_coords" } });
		square_vertex_array->add_vertex_buffer(square_vb);

		uint32_t square_indices[] = { 0, 1, 2, 2, 3, 0 };
		ref<IndexBuffer> square_ib = IndexBuffer::create(square_indices, sizeof(square_indices) / sizeof(uint32_t));
		square_vertex_array->set_index_buffer(square_ib);

		auto vertex_source = R"(
			#version 330 core
			layout(location = 0) in vec3 attrib_position;
			layout(location = 1) in vec4 attrib_color;
			uniform mat4 uniform_view_projection;
			uniform mat4 uniform_transform;
			out vec3 v_position;
			out vec4 v_color;
			void main() {
				v_position = attrib_position;
				v_color = attrib_color;
				gl_Position = uniform_view_projection * uniform_transform * vec4(attrib_position, 1.0);
			}
		)";

		auto fragment_source = R"(
			#version 330 core
			layout(location = 0) out vec4 color;
			in vec3 v_position;
			in vec4 v_color;
			void main() {
				color = vec4(v_position * 0.5 + 0.5, 1.0);
				color = v_color;
			}
		)";

		vertex_pos_shader = Shader::create("vertex position color", vertex_source, fragment_source);

		auto flat_color_shader_vertex_source = R"(
			#version 330 core
			layout(location = 0) in vec3 attrib_position;
			uniform mat4 uniform_view_projection;
			uniform mat4 uniform_transform;
			out vec3 v_position;
			void main() {
				v_position = attrib_position;
				gl_Position = uniform_view_projection * uniform_transform * vec4(attrib_position, 1.0);
			}
	    )";

		auto flat_color_shader_fragment_source = R"(
			#version 330 core
			layout(location = 0) out vec4 color;
			in vec3 v_position;
			uniform vec3 uniform_color;
			void main() {
				color = vec4(uniform_color, 1.0);
			}
 		)";

		flat_colour_shader = Shader::create("flat color shader", flat_color_shader_vertex_source,
			flat_color_shader_fragment_source);

		auto texture_shader = shader_library.load("assets/shaders/texture_shader.glsl");

		texture = Texture2D::create("assets/textures/checkerboard.png");
		cherno_texture = Texture2D::create("assets/textures/cherno_logo.png");

		texture_shader->bind();
		texture_shader->upload_uniform("tex_sampler", 0);
	}

	virtual void updated(Timestep ts) override
	{
		controller.on_update(ts);

		RenderCommand::clear();
		RenderCommand::set_clear_colour({ 0.1f, 0.24f, 0.1f, 1 });

		Renderer::begin_scene(controller.get_camera());

		static auto scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		flat_colour_shader->bind();
		flat_colour_shader->upload_uniform("u_color", square_colour);

		for (int i = 0; i < num_squares; i++) {
			for (int j = 0; j < num_squares; j++) {
				auto pos = glm::vec3(i * 0.11, j * 0.11, 0.0f);
				auto transform = glm::translate(glm::mat4(1.0), pos) * scale;
				Renderer::submit(square_vertex_array, flat_colour_shader, transform);
			}
		}

		auto shader = shader_library.get("texture_shader");

		texture->bind();
		Renderer::submit(square_vertex_array, shader, glm::scale(glm::mat4(1.0), glm::vec3(1.5f)));
		cherno_texture->bind();
		Renderer::submit(square_vertex_array, shader, glm::scale(glm::mat4(1.0), glm::vec3(1.5f)));

		Renderer::end_scene();
	}

	virtual void on_imgui_render() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Squares Colour", glm::value_ptr(square_colour));
		ImGui::End();
	}

	virtual void on_event(Event& event) override
	{
		controller.on_event(event);

		EventDispatcher dispatcher(event);

		dispatcher.dispatch_event<KeyPressedEvent>([&](KeyPressedEvent& ev) {
			if (ev.get_key_code() == NX_KC_Q) {
				return Application::the().exit();
			}
			return false;
		});
	}

private:
	ref<VertexArray> triangle_vertex_array;
	ref<Shader> flat_colour_shader;
	ref<Shader> vertex_pos_shader;
	ref<VertexArray> square_vertex_array;

	ref<Texture2D> texture;
	ref<Texture2D> cherno_texture;

	glm::vec3 square_position = { 0, 0, 0 };

	int num_squares = 20;

	glm::vec3 square_colour = glm::vec3(1.0f);

	OrthographicCameraController controller;
	ShaderLibrary shader_library;
};

class Sandbox : public Application {
public:
	Sandbox()
	{
		// add_layer(new ExampleLayer());
		add_layer(new Sandbox2D());
	};

	~Sandbox(){};
};

Neurex::Application* Neurex::create_application()
{
	return new Sandbox();
}