#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Sandbox2D.h"

#include <Neurex/core/EntryPoint.h>

using namespace Neurex;

class ExampleLayer : public Layer {
public:
	ExampleLayer()
		: Layer("Sandbox")
		, controller(1280.0f / 720.0f)
	{
		RenderCommand::set_clear_colour({ 0.1f, 0.1f, 0.1f, 1 });
		vertex_array = VertexArray::create();

		float vertices[3 * 7] = { -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f, 0.0f, 0.5f, 0.0f, 0.8f,
			0.8f, 0.2f, 1.0f };

		ref<VertexBuffer> vertex_buffer
			= VertexBuffer::create(vertices, sizeof(vertices));
		BufferLayout layout = { { ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" } };
		vertex_buffer->set_layout(layout);
		vertex_array->add_vertex_buffer(vertex_buffer);

		uint32_t indices[3] = { 0, 1, 2 };
		ref<IndexBuffer> index_buffer
			= IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t));
		vertex_array->set_index_buffer(index_buffer);

		square_vertex_array = VertexArray::create();

		float squareVertices[5 * 4]
			= { -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
				  0.5f, 0.5f, 0.0f, 1.0f, 1.0f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f };

		ref<VertexBuffer> square_vb
			= VertexBuffer::create(squareVertices, sizeof(squareVertices));
		square_vb->set_layout({ { ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" } });
		square_vertex_array->add_vertex_buffer(square_vb);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		ref<IndexBuffer> square_ib = IndexBuffer::create(
			squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		square_vertex_array->set_index_buffer(square_ib);

		std::string vertexSrc = R"(
			#version 410 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
			out vec3 v_Position;
			out vec4 v_Color;
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 410 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			in vec4 v_Color;
			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		shader = Shader::create("VertexPosColor", vertexSrc, fragmentSrc);

		std::string flatColorShaderVertexSrc = R"(
			#version 410 core
			
			layout(location = 0) in vec3 a_Position;
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
			out vec3 v_Position;
			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string flatColorShaderFragmentSrc = R"(
			#version 410 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			
			uniform vec3 u_Color;
			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";

		flat_color_shader = Shader::create(
			"FlatColor", flatColorShaderVertexSrc, flatColorShaderFragmentSrc);

		auto texture_shader
			= shader_library.load("assets/shaders/texture.glsl");

		texture = Texture2D::create("assets/textures/checkerboard.png");
		cherno_logo_texture
			= Texture2D::create("assets/textures/cherno_logo.png");

		texture_shader->bind();
		texture_shader->upload_uniform("u_Texture", 0);
	}

	virtual void updated(Timestep ts) override
	{
		controller.on_update(ts);

		RenderCommand::clear();

		Renderer::begin_scene(controller.get_camera());

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.1f));

		flat_color_shader->bind();
		flat_color_shader->upload_uniform("u_Color", square_colour);

		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform
					= glm::translate(glm::mat4(1.0f), pos) * scale;
				Renderer::submit(
					square_vertex_array, flat_color_shader, transform);
			}
		}

		auto texture_shader = shader_library.get("texture");

		texture->bind();
		Renderer::submit(square_vertex_array, texture_shader,
			glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		// cherno_logo_texture->bind();
		// Renderer::submit(square_vertex_array, texture_shader,
		// glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		Renderer::end_scene();
	}

	virtual void on_imgui_render() override
	{
		ImGui::Begin("Title");
		ImGui::Text("%s", debug_name.c_str());
		ImGui::End();

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
	ShaderLibrary shader_library;
	ref<Shader> shader;
	ref<VertexArray> vertex_array;

	ref<Shader> flat_color_shader;
	ref<VertexArray> square_vertex_array;

	ref<Texture2D> texture, cherno_logo_texture;

	OrthographicCameraController controller;
	glm::vec3 square_colour = { 0.2f, 0.3f, 0.8f };
};

class Sandbox : public Application {
public:
	Sandbox()
	{
		// add_layer(make_scoped<ExampleLayer>());
		add_layer(make_scoped<Sandbox2D>());
	};

	~Sandbox(){};
};

Neurex::Application* Neurex::create_application() { return new Sandbox(); }