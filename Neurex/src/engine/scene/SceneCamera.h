#pragma once

#include "engine/renderer/Camera.h"

namespace Engine {

class SceneCamera : public Camera {
public:
	enum class ProjectionType { Perspective = 0, Orthographic = 1 };

public:
	SceneCamera();
	virtual ~SceneCamera() = default;

	void set_perspective(float verticalFOV, float nearClip, float farClip);
	void set_orthographic(float size, float nearClip, float farClip);

	void set_viewport_size(uint32_t width, uint32_t height);

	float get_perspective_vertical_FOV() const { return perspective_FOV; }

	void set_perspective_verticalFOV(float verticalFov)
	{
		perspective_FOV = verticalFov;
		recalculate_projection();
	}

	float get_perspective_near_clip() const { return perspective_near; }

	void set_perspective_near_clip(float nearClip)
	{
		perspective_near = nearClip;
		recalculate_projection();
	}

	float get_perspective_far_clip() const { return perspective_far; }

	void set_perspective_far_clip(float farClip)
	{
		perspective_far = farClip;
		recalculate_projection();
	}

	float get_orthographic_size() const { return orthographic_size; }
	void set_orthographic_size(float size)
	{
		orthographic_size = size;
		recalculate_projection();
	}
	float get_orthographic_near_clip() const { return orthographic_near; }
	void set_orthographic_near_clip(float nearClip)
	{
		orthographic_near = nearClip;
		recalculate_projection();
	}
	float get_orthographic_far_clip() const { return orthographic_far; }
	void set_orthographic_far_clip(float farClip)
	{
		orthographic_far = farClip;
		recalculate_projection();
	}

	ProjectionType get_projection_type() const { return projection_type; }
	void set_projection_type(ProjectionType type)
	{
		projection_type = type;
		recalculate_projection();
	}

private:
	void recalculate_projection();

private:
	ProjectionType projection_type = ProjectionType::Orthographic;

	float perspective_FOV = glm::radians(45.0f);
	float perspective_near = 0.01f, perspective_far = 1000.0f;

	float orthographic_size = 10.0f;
	float orthographic_near = -1.0f, orthographic_far = 1.0f;

	float aspect_ratio = 0.0f;
};

}