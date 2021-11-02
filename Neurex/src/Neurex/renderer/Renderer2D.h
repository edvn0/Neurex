#pragma once

#include "Neurex/renderer/OrthographicCamera.h"
#include "Neurex/renderer/Shader.h"
#include "Neurex/renderer/Texture.h"
#include "Neurex/renderer/VertexArray.h"

#include "Neurex/renderer/Camera.h"
#include "Neurex/renderer/EditorCamera.h"
#include "Neurex/scene/Components.h"

namespace Neurex {
struct RendererStats {
	size_t draw_calls = 0;
	size_t quads_submitted = 0;
};

class Renderer2D {
public:
	static void init();
	static void shut_down();

	static void begin_scene(const OrthographicCamera& cam);
	static void begin_scene(const Camera& camera, const glm::mat4& transform);
	static void begin_scene(const EditorCamera& camera);

	/**
	 * Draws quads by colour
	 **/
	static void draw_rotated_quad(
		const glm::vec3& pos, float rotation_radians, const glm::vec2& size, const glm::vec4& colour);

	/**
	 * Draws quads by textures.
	 **/
	static void draw_rotated_quad(const glm::vec3& pos, float rotation_radians, const glm::vec2& size,
		const ref<Texture2D>& texture, const glm::vec4& tint_color = glm::vec4(1.0f), float tiling_factor = 1.0f);

	static void draw_sprite(const glm::mat4& transform, SpriteRendererComponent& src, int entity_id);

	static void draw_quad(const glm::mat4& transform, const glm::vec4& color, int entity_id = -1);

	static void draw_quad(const glm::mat4& transform, const ref<Texture2D>& texture, float tiling_factor = 1.0f,
		const glm::vec4& tintColor = glm::vec4(1.0f), int entity_id = -1);

	static RendererStats& get_stats();

	static void end_scene();
	static void flush();

private:
	static void start_batch();
	static void next_batch();
};

} // namespace Neurex
