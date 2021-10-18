#pragma once

#include "Neurex/core/Input.h"
#include "Neurex/core/KeyCodes.h"
#include "Neurex/core/Timestep.h"
#include "Neurex/events/ApplicationEvent.h"
#include "Neurex/events/MouseEvent.h"
#include "Neurex/renderer/OrthographicCamera.h"

namespace Neurex {
class OrthographicCameraController {
public:
	OrthographicCameraController(float aspect_ratio, float zoom_level = 1.0f);

	void on_update(Timestep t);

	void on_event(Event& e);

	OrthographicCamera& get_camera() { return camera; };

private:
	float aspect_ratio;
	float zoom = 1.0f;

	float camera_rotation = 0.0f;
	glm::vec3 camera_position = { 0, 0, 0 };
	float rotation_speed = 45.0f;
	float move_speed = 0.5f;

	OrthographicCamera camera;
};
}