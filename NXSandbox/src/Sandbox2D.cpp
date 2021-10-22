
#include "Sandbox2D.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox 2D")
	, camera_controller(16.0f / 9.0f)
{
}

void Sandbox2D::attached()
{
	RenderCommand::set_clear_colour({ 0.1f, 0.1f, 0.1f, 1 });

	square_vertex_array = VertexArray::create();

	float squareVertices[3 * 4] = {
		-0.5f,
		-0.5f,
		0.0f,
		0.5f,
		-0.5f,
		0.0f,
		0.5f,
		0.5f,
		0.0f,
		-0.5f,
		0.5f,
		0.0f
	};

	ref<VertexBuffer> square_vb = VertexBuffer::create(squareVertices, sizeof(squareVertices));
	square_vb->set_layout({ { ShaderDataType::Float3, "a_Position" } });
	square_vertex_array->add_vertex_buffer(square_vb);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	ref<IndexBuffer> square_ib = IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
	square_vertex_array->set_index_buffer(square_ib);

	flat_color_shader = Shader::create("assets/shaders/flat_colour.glsl");
}

void Sandbox2D::detached() { }

void Sandbox2D::updated(Timestep ts)
{
	camera_controller.on_update(ts);

	flat_color_shader->bind();
	flat_color_shader->upload_uniform("u_Color", square_color);

	RenderCommand::clear();
	Renderer::begin_scene(camera_controller.get_camera());

	auto a = glm::scale(glm::mat4(1.0f), glm::vec3(1.4f));
	Renderer::submit(square_vertex_array, flat_color_shader, a);

	//	Renderer2D::begin_scene(camera_controller.camera());
	//	Renderer2D::draw_quad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	//	Renderer2D::draw_quad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
	//	Renderer2D::draw_quad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, checkerboard_texture);
	//	Renderer2D::end_scene();

	Renderer::end_scene();
}

void Sandbox2D::on_imgui_render()
{
	ImGui::Begin("FPS");
	ImGui::Text("%f", Application::the().get_fps());
	ImGui::End();

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square color", glm::value_ptr(square_color));
	ImGui::End();
}

void Sandbox2D::on_event(Event& event)
{
	camera_controller.on_event(event);
	EventDispatcher dispatcher(event);

	dispatcher.dispatch_event<KeyPressedEvent>([&](KeyPressedEvent& ev) {
		if (ev.get_key_code() == NX_KC_Q) {
			return Application::the().exit();
		}
		return false;
	});
}