#include "CameraController.h"
#include "nxpch.h"

namespace Neurex {

OrthographicCameraController::OrthographicCameraController(
	float aspect_ratio_, float zoom_level)
	: aspect_ratio(aspect_ratio_)
	, zoom(zoom_level)
	, camera(-aspect_ratio * zoom, aspect_ratio * zoom, -zoom, zoom){};

void OrthographicCameraController::on_update(Timestep t)
{
	float step = t.get_seconds();

	if (Input::is_key_pressed(NX_KC_Q)) {
		camera_rotation += (-rotation_speed * step);
	}

	if (Input::is_key_pressed(NX_KC_E)) {
		camera_rotation += (rotation_speed * step);
	}

	if (Input::is_key_pressed(NX_KC_W)) {
		camera_position += (-glm::vec3(0, move_speed, 0) * step * zoom);
	}

	if (Input::is_key_pressed(NX_KC_S)) {
		camera_position += (glm::vec3(0, move_speed, 0) * step * zoom);
	}

	if (Input::is_key_pressed(NX_KC_A)) {
		camera_position += (glm::vec3(move_speed, 0, 0) * step * zoom);
	}

	if (Input::is_key_pressed(NX_KC_D)) {
		camera_position += (-glm::vec3(move_speed, 0, 0) * step * zoom);
	}

	camera.set_position(camera_position);
	camera.set_rotation(camera_rotation);
};

void OrthographicCameraController::on_event(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.dispatch_event<MouseScrolledEvent>([&](MouseScrolledEvent& e) {
		zoom -= e.get_offset_y();
		zoom = std::max(zoom, minimal_zoom_camera_controller);
		camera.set_projection(
			-aspect_ratio * zoom, aspect_ratio * zoom, -zoom, zoom);
		return false;
	});

	dispatcher.dispatch_event<WindowResizeEvent>([&](WindowResizeEvent& e) {
		aspect_ratio = e.get_width() / e.get_height();
		camera.set_projection(
			-aspect_ratio * zoom, aspect_ratio * zoom, -zoom, zoom);
		return false;
	});

	dispatcher.dispatch_event<MouseButtonPressedEvent>(
		[&](MouseButtonPressedEvent& ev) {
			if (ev.get_mouse_button() != NX_MC_BUTTON_LEFT)
				return false;

			if (!is_pressed) { // we have previous
				is_pressed = true;
				cursor_x = ev.get_x();
				cursor_y = ev.get_y();
			}
			return false;
		});

	dispatcher.dispatch_event<MouseButtonReleasedEvent>(
		[&](MouseButtonReleasedEvent& ev) {
			if (ev.get_mouse_button() != NX_MC_BUTTON_LEFT)
				return false;

			if (is_pressed) {
				auto diff_x = (ev.get_x() - cursor_x);
				auto diff_y = (ev.get_y() - cursor_y);

				camera_position
					+= glm::vec3(-diff_x / 1200.0f, diff_y / 800.0f, 0.0f);
				camera.set_position(camera_position);

				NX_INFO("Vector difference cursor and chosen: {0}, {1}", diff_x,
					diff_y);
				is_pressed = false;
			}
			return false;
		});
};
}
