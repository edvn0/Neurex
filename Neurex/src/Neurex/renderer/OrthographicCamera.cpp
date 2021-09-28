#include "nxpch.h"
#include "OrthographicCamera.h"

namespace Neurex
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float zn, float zf)
	{
		proj_matrix = glm::ortho(left, right, bottom, top, zn, zf);
		view_matrix = glm::mat4(1.0f);
	}

	void OrthographicCamera::recalculate_view_matrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation_angle, glm::vec3(0, 0, 1));

		view_matrix = glm::inverse(transform);
		view_proj_matrix = proj_matrix * view_matrix;
	}
}