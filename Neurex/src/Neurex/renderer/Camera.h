#pragma once

#include <glm/glm.hpp>

#include "Neurex/core/Core.h"

namespace Neurex {

class Camera {
public:
	Camera() = default;
	Camera(const glm::mat4& projection_)
		: projection(projection_)
	{
	}

	virtual ~Camera() = default;

	const glm::mat4& get_projection() const { return projection; }

protected:
	glm::mat4 projection = glm::mat4(1.0f);
};

}