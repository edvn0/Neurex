#include "SceneCamera.h"
#include "nxpch.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Engine {

SceneCamera::SceneCamera() { recalculate_projection(); }

void SceneCamera::set_perspective(float verticalFOV, float nearClip, float farClip)
{
	projection_type = ProjectionType::Perspective;
	perspective_FOV = verticalFOV;
	perspective_near = nearClip;
	perspective_far = farClip;
	recalculate_projection();
}

void SceneCamera::set_orthographic(float size, float nearClip, float farClip)
{
	projection_type = ProjectionType::Orthographic;
	orthographic_size = size;
	orthographic_near = nearClip;
	orthographic_far = farClip;
	recalculate_projection();
}

void SceneCamera::set_viewport_size(uint32_t width, uint32_t height)
{
	aspect_ratio = (float)width / (float)height;
	recalculate_projection();
}

void SceneCamera::recalculate_projection()
{
	if (projection_type == ProjectionType::Perspective) {
		projection = glm::perspective(perspective_FOV, aspect_ratio, perspective_near, perspective_far);
	} else {
		float orthoLeft = -orthographic_size * aspect_ratio * 0.5f;
		float orthoRight = orthographic_size * aspect_ratio * 0.5f;
		float orthoBottom = -orthographic_size * 0.5f;
		float orthoTop = orthographic_size * 0.5f;

		projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, orthographic_near, orthographic_far);
	}
}

}