#include "EditorCamera.h"
#include "nxpch.h"

#include "engine/core/Input.h"
#include "engine/core/KeyCodes.h"
#include "engine/core/MouseCodes.h"

#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Engine {

EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
	: FOV(fov)
	, aspect_ratio(aspectRatio)
	, near_clip(nearClip)
	, far_clip(farClip)
	, Camera(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip))
{
	update_view();
}

void EditorCamera::update_projection()
{
	aspect_ratio = viewport_width / viewport_height;
	projection = glm::perspective(glm::radians(FOV), aspect_ratio, near_clip, far_clip);
}

void EditorCamera::update_view()
{
	// m_Yaw = m_Pitch = 0.0f; // Lock the camera's rotation
	position = calculate_position();

	glm::quat orientation = get_orientation();
	view_matrix = glm::translate(glm::mat4(1.0f), position) * glm::toMat4(orientation);
	view_matrix = glm::inverse(view_matrix);
}

std::pair<float, float> EditorCamera::pan_speed() const
{
	float x = std::min(viewport_width / 1000.0f, 2.4f); // max = 2.4f
	float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

	float y = std::min(viewport_height / 1000.0f, 2.4f); // max = 2.4f
	float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

	return { xFactor, yFactor };
}

float EditorCamera::rotation_speed() const { return 0.8f; }

float EditorCamera::zoom_speed() const
{
	float distance = distance * 0.2f;
	distance = std::max(distance, 0.0f);
	float speed = distance * distance;
	speed = std::min(speed, 100.0f); // max speed = 100
	return speed;
}

void EditorCamera::on_update(Timestep ts)
{
	if (Input::is_key_pressed(NX_KC_LEFT_ALT)) {
		const glm::vec2& mouse{ Input::mouse_x(), Input::mouse_y() };
		glm::vec2 delta = (mouse - initial_mouse_position) * 0.003f;
		initial_mouse_position = mouse;

		if (Input::is_mouse_button_pressed(NX_MC_BUTTON_LEFT))
			mouse_pan(delta);
		else if (Input::is_mouse_button_pressed(NX_MC_BUTTON_MIDDLE))
			mouse_rotate(delta);
		else if (Input::is_mouse_button_pressed(NX_MC_BUTTON_RIGHT))
			mouse_zoom(delta.y);
	}

	update_view();
}

void EditorCamera::on_event(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.dispatch_event<MouseScrolledEvent>([&](MouseScrolledEvent& e) {
		float delta = e.get_offset_y() * 0.1f;
		mouse_zoom(delta);
		update_view();
		return false;
	});
}

void EditorCamera::mouse_pan(const glm::vec2& delta)
{
	auto [xSpeed, ySpeed] = pan_speed();
	focal_point += -get_right_direction() * delta.x * xSpeed * distance;
	focal_point += get_up_direction() * delta.y * ySpeed * distance;
}

void EditorCamera::mouse_rotate(const glm::vec2& delta)
{
	float yawSign = get_up_direction().y < 0 ? -1.0f : 1.0f;
	yaw += yawSign * delta.x * rotation_speed();
	pitch += delta.y * rotation_speed();
}

void EditorCamera::mouse_zoom(float delta)
{
	distance -= delta * zoom_speed();
	if (distance < 1.0f) {
		focal_point += get_forward_direction();
		distance = 1.0f;
	}
}

glm::vec3 EditorCamera::get_up_direction() const
{
	return glm::rotate(get_orientation(), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 EditorCamera::get_right_direction() const
{
	return glm::rotate(get_orientation(), glm::vec3(1.0f, 0.0f, 0.0f));
}

glm::vec3 EditorCamera::get_forward_direction() const
{
	return glm::rotate(get_orientation(), glm::vec3(0.0f, 0.0f, -1.0f));
}

glm::vec3 EditorCamera::calculate_position() const { return focal_point - get_forward_direction() * distance; }

glm::quat EditorCamera::get_orientation() const { return glm::quat(glm::vec3(-pitch, -yaw, 0.0f)); }

}