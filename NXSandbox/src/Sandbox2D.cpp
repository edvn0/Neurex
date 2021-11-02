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
	cherno_texture = Texture2D::create("assets/textures/cherno_logo.png");
	spritesheet = Texture2D::create("assets/sprites/PixelWeapons.png");

	weapon_zero = SpritesheetTexture::create(spritesheet, { 1, 2 }, { 32, 32 }, { 1, 2 });
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

	{
		NX_PROFILE_SCOPE("Renderer2D::GPUCalls");

		RenderCommand::clear();
		static float rotation_square = 20.0f;
		rotation_square += 30.0f * ts.get_seconds();
		if (rotation_square >= 360.0f)
			rotation_square = 0.0f;
		Renderer2D::begin_scene(camera_controller.get_camera());
		Renderer2D::draw_rotated_quad({ -1.0f, 0.0f, -0.5f }, 0.0f, { 0.8f, 0.8f }, square_color);
		Renderer2D::draw_rotated_quad(
			{ -0.5f, -0.5f, -0.2f }, glm::radians(rotation_square), { 0.5f, 0.75f }, { 0.9f, 0.3f, 0.1f, 1.0f });
		Renderer2D::draw_rotated_quad({ -3.0, -3.0, 0.2f }, glm::radians(-rotation_square), { 1.0f, 1.0f },
			checkerboard_texture, glm::vec4(1.0f), 50.0f);
		Renderer2D::draw_rotated_quad({ 0.0f, 0.0f, 0.0f }, 0.0f, { 1.0f, 1.0f }, cherno_texture);

		// Renderer2D::draw_sprite({ 2.0f, -1.5f, 0.0f }, 0.0f, { 1.0f, 2.0f }, weapon_zero);

		for (float x = -5.0f; x <= 5.0f; x += 0.5) {
			for (float y = -5.0f; y <= 5.0f; y += 0.5) {
				glm::vec4 col({ (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.5f });
				Renderer2D::draw_rotated_quad({ x, y, 0.0f }, 0.0f, { 0.249, 0.249 }, col);
			}
		}
		Renderer2D::end_scene();
	}
}

void Sandbox2D::on_imgui_render()
{
	ImGui::Begin("Stats");

	auto stats = Renderer2D::get_stats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %lu", stats.draw_calls);
	ImGui::Text("Quads: %lu", stats.quads_submitted);
	ImGui::Text("Vertices: %lu", stats.quads_submitted * 4);
	ImGui::Text("Indices: %lu", stats.quads_submitted * 6);

	ImGui::ColorEdit4("Square Color", glm::value_ptr(square_color));

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