#pragma once

#include "VertexArray.h"
#include <glm/glm.hpp>

namespace Neurex {
class RendererAPI {
public:
	enum class API {
		None = 0,
		OpenGL,
		DirectX,
		Vulkan,
		Metal
	};

public:
	virtual void clear() = 0;
	virtual void set_clear_colour(const glm::vec4& color) = 0;

	virtual void draw_indexed(const ref<VertexArray>& va) = 0;

	inline static API get_api() { return chosen_api; };

private:
	static API chosen_api;
};
}