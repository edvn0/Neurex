
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
}

void Sandbox2D::detached() { }

void Sandbox2D::updated(Timestep ts)
{
	camera_controller.on_update(ts);

	RenderCommand::clear();
	Renderer2D::begin_scene(camera_controller.get_camera());
	Renderer2D::draw_quad({ -1.0f, 0.0f, 0.0f }, { 0.8f, 0.8f }, square_color);
	Renderer2D::draw_quad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.6f, 0.1f, 1.0f });
	Renderer2D::draw_rotated_quad({ -0.5f, -0.5f, 0.0f }, { 22.0, 0.0, 0.0 }, { 0.5f, 0.75f }, { 0.9f, 0.3f, 0.1f, 1.0f });
	Renderer2D::draw_quad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, checkerboard_texture);
	Renderer2D::end_scene();
}

void Sandbox2D::on_imgui_render()
{
	ImGui::Begin("FPS");
	ImGui::Text("%f", Application::the().get_fps());
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square color", glm::value_ptr(square_color));
	ImGui::End();

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