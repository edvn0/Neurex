#pragma once

#include "engine/core/Input.h"
#include "engine/core/KeyCodes.h"
#include "engine/core/MouseCodes.h"
#include "engine/core/Timestep.h"
#include "engine/events/ApplicationEvent.h"
#include "engine/events/MouseEvent.h"
#include "engine/renderer/OrthographicCamera.h"

static constexpr float minimal_zoom_camera_controller = 0.1f;

namespace Engine {
class OrthographicCameraController {
public:
	OrthographicCameraController(float aspect_ratio, float zoom_level = 1.0f);

	void on_update(Timestep t);

	void on_event(Event& e);

	void resize(uint32_t width, uint32_t height);

	OrthographicCamera& get_camera() { return camera; };

	void set_zoom(float zoom_)
	{
		zoom = zoom_;
		calculate_view();
	}

private:
	void calculate_view();

private:
	float aspect_ratio;
	float zoom = 1.0f;

	float camera_rotation = 0.0f;
	glm::vec3 camera_position = { 0, 0, 0 };
	float rotation_speed = 45.0f;
	float move_speed = 2.0f;

	OrthographicCamera camera;

private: // For user.
	float cursor_x = 0.0f;
	float cursor_y = 0.0f;
	bool is_pressed = false;
};
}