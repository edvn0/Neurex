#pragma once

#include "RenderCommand.h"
#include "OrthographicCamera.h"
#include "Shader.h"

namespace Neurex
{

	class Renderer {
	public:
		static void begin_scene(OrthographicCamera& camera);

		static void submit(const std::shared_ptr<VertexArray>& va, const std::shared_ptr<Shader>& shader);

		static void end_scene();

		inline static RendererAPI::API get_api() { return RendererAPI::get_api(); }
	private:
		struct SceneData {
			glm::mat4 view_projection_matrix;
		};

		static SceneData* scene_data;
	};

}