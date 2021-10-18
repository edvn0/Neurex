#include "Neurex/CameraController.h"
#include "nxpch.h"

namespace Neurex {

OrthographicCameraController::OrthographicCameraController(float aspect_ratio_, float zoom_level)
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
		camera_position += (-glm::vec3(0, move_speed, 0) * step);
	}

	if (Input::is_key_pressed(NX_KC_S)) {
		camera_position += (glm::vec3(0, move_speed, 0) * step);
	}

	if (Input::is_key_pressed(NX_KC_A)) {
		camera_position += (glm::vec3(move_speed, 0, 0) * step);
	}

	if (Input::is_key_pressed(NX_KC_D)) {
		camera_position += (-glm::vec3(move_speed, 0, 0) * step);
	}

	camera.set_position(camera_position);
	camera.set_rotation(camera_rotation);
};

void OrthographicCameraController::on_event(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.dispatch_event<MouseScrolledEvent>([&](MouseScrolledEvent& e) {
		zoom -= e.get_offset_y();
		camera.set_projection(-aspect_ratio * zoom, aspect_ratio * zoom, -zoom, zoom);
		return false;
	});

	dispatcher.dispatch_event<WindowResizeEvent>([&](WindowResizeEvent& e) {
		aspect_ratio = e.get_width() / e.get_height();
		camera.set_projection(-aspect_ratio * zoom, aspect_ratio * zoom, -zoom, zoom);
		return false;
	});
};
}
