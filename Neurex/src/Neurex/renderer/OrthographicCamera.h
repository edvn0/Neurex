#pragma once

#include <glm/gtc/matrix_transform.hpp>

namespace Neurex
{
	class OrthographicCamera {
	public:
		OrthographicCamera(float left, float right, float bottom, float top, float zn = -1.0f, float zf = 1.0f);
		
		const glm::vec3& get_position() const { return position; };
		void set_position(const glm::vec3& pos) {
			position = pos; 
			recalculate_view_matrix();
		};

		float get_rotation() const { return rotation_angle; }
		void set_rotation(float angle) {
			rotation_angle = angle; 
			recalculate_view_matrix(); 
		};

		const glm::mat4& get_projection_matrix() const { return proj_matrix; };
		const glm::mat4& get_view_matrix() const { return view_matrix; };
		const glm::mat4& get_view_projection_matrix() const { return view_proj_matrix; };
	private:
		void recalculate_view_matrix();

	private:
		glm::mat4 proj_matrix;
		glm::mat4 view_matrix;
		glm::mat4 view_proj_matrix;

		glm::vec3 position;
		float rotation_angle = 0.0f;
	};
}