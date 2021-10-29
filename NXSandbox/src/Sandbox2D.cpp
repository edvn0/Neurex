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
	checkerboard_texture = Texture2D::create("assets/textures/checkerboard.png");
	NX_INFO("{0}, {1}, {2}", checkerboard_texture->get_renderer_id(), checkerboard_texture->get_width(),
		checkerboard_texture->get_height());
	cherno_texture = Texture2D::create("assets/textures/cherno_logo.png");
	NX_INFO("{0}, {1}, {2}", cherno_texture->get_renderer_id(), cherno_texture->get_width(),
		cherno_texture->get_height());
}

void Sandbox2D::detached() { }

void Sandbox2D::updated(Timestep ts)
{
	NX_PROFILE_FUNCTION();

	static constexpr auto radians = glm::radians(22.0f);

	{
		NX_PROFILE_SCOPE("CameraController::OnUpdate");
		camera_controller.on_update(ts);
	}

	RenderCommand::clear();
	{
		NX_PROFILE_SCOPE("Renderer2D::GPUCalls");
		Renderer2D::begin_scene(camera_controller.get_camera());

		Renderer2D::draw_quad({ -1.0f, 0.0f, 0.0f }, { 0.8f, 0.8f }, square_color);

		Renderer2D::draw_rotated_quad(
			{ -0.5f, -0.5f, 0.0f }, radians, { 0.5f, 0.75f }, { 0.9f, 0.3f, 0.1f, 1.0f });

		Renderer2D::draw_quad({ 0.0, 0.0 }, { 0.5f, 0.5f }, cherno_texture);
		Renderer2D::draw_quad({ 0.1, 0.0 }, { 0.5f, 0.5f }, cherno_texture);
		Renderer2D::draw_quad({ 0.2, 0.0 }, { 0.5f, 0.5f }, cherno_texture);
		Renderer2D::draw_quad({ 0.3, 0.0 }, { 0.5f, 0.5f }, cherno_texture);

		Renderer2D::draw_quad({ 0.0, 0.0 }, { 10.0f, 10.0f }, checkerboard_texture);
		Renderer2D::draw_quad({ 0.0, 0.1 }, { 10.0f, 10.0f }, checkerboard_texture);
		Renderer2D::draw_quad({ 0.0, 0.2 }, { 10.0f, 10.0f }, checkerboard_texture);
		Renderer2D::draw_quad({ 0.0, 0.3 }, { 10.0f, 10.0f }, checkerboard_texture);

		Renderer2D::end_scene();
	}
}

void Sandbox2D::on_imgui_render()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square color", glm::value_ptr(square_color));

	auto& stats = Renderer2D::get_stats();

	ImGui::Text(
		"Renderer2D Draw Stats: \nCalls: %d\nQuads: %d", (int)stats.draw_calls, (int)stats.quads_submitted);
	ImGui::End();
}

void Sandbox2D::on_event(Event& event)
{
	camera_controller.on_event(event);
	EventDispatcher dispatcher(event);

	dispatcher.dispatch_event<KeyPressedEvent>([&](KeyPressedEvent& ev) {
		if (ev.get_key_code() == NX_KC_X) {
			return Application::the().exit();
		}
		return false;
	});
}