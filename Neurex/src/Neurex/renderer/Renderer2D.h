#pragma once

#include "Neurex/renderer/OrthographicCamera.h"
#include "Neurex/renderer/Texture.h"
#include "Shader.h"
#include "VertexArray.h"

namespace Neurex {

class Renderer2D {
public:
	static void init();
	static void shut_down();

	static void begin_scene(const OrthographicCamera& cam);

	static void draw_quad(
		const glm::vec3& pos, const glm::vec2& size, const glm::vec4& colour);
	static void draw_quad(
		const glm::vec2& pos, const glm::vec2& size, const glm::vec4& colour);
	static void draw_quad(const glm::vec2& pos, const glm::vec2& size,
		const ref<Texture2D>& texture);
	static void draw_quad(const glm::vec3& pos, const glm::vec2& size,
		const ref<Texture2D>& texture);
	static void draw_rotated_quad(const glm::vec3& pos, const glm::vec3& rot,
		const glm::vec2& size, const glm::vec4& colour);

	static void end_scene();
};

} // namespace Neurex
