#pragma once

#include "OrthographicCamera.h"
#include "RenderCommand.h"
#include "Shader.h"

#include <glm/glm.hpp>

namespace Neurex {

class Renderer {
public:
	static void init();

	static void begin_scene(OrthographicCamera& camera);

	static void submit(
		const ref<VertexArray>& va, const ref<Shader>& shader, const glm::mat4& transform = glm::mat4(1.0f));

	static void end_scene();

	inline static RendererAPI::API get_api() { return RendererAPI::get_api(); }

private:
	struct SceneData {
		glm::mat4 view_projection_matrix;
	};

	static bool has_begun;
	static bool has_ended;

	static SceneData* scene_data;
};

}