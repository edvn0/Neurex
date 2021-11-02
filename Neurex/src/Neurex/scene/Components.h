#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "Neurex/renderer/Texture.h"
#include "SceneCamera.h"
#include "ScriptableEntity.h"

#include <functional>

namespace Neurex {

struct TagComponent {
	std::string tag;

	TagComponent() = default;
	TagComponent(const TagComponent&) = default;
	TagComponent(const std::string& tag_)
		: tag(tag_)
	{
	}
};

struct TransformComponent {
	glm::vec3 translation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 scale = { 1.0f, 1.0f, 1.0f };

	TransformComponent() = default;
	TransformComponent(const TransformComponent&) = default;
	TransformComponent(const glm::vec3& translation_)
		: translation(translation_)
	{
	}

	glm::mat4 get_transform() const
	{
		glm::mat4 rotation = glm::toMat4(glm::quat(rotation));

		return glm::translate(glm::mat4(1.0f), translation) * rotation * glm::scale(glm::mat4(1.0f), scale);
	}
};

struct SpriteRendererComponent {
	glm::vec4 colour{ 1.0f, 1.0f, 1.0f, 1.0f };
	ref<Texture2D> texture;
	float tiling_factor = 1.0f;

	SpriteRendererComponent() = default;
	SpriteRendererComponent(const SpriteRendererComponent&) = default;
	SpriteRendererComponent(const glm::vec4& colour_)
		: colour(colour_)
	{
	}
};

struct CameraComponent {
	SceneCamera camera;
	bool primary = true;
	bool fixed_aspect_ratio = false;

	CameraComponent() = default;
	CameraComponent(const CameraComponent&) = default;
};

struct NativeScriptComponent {
public:
	ScriptableEntity* instance = nullptr;

	std::function<ScriptableEntity*(void)> instantiate_script;
	std::function<void(NativeScriptComponent*)> destroy_script;

	template <typename T> void bind()
	{
		instantiate_script = []() { return static_cast<ScriptableEntity*>(new T()); };
		destroy_script = [](NativeScriptComponent* nsc) {
			delete nsc->instance;
			nsc->instance = nullptr;
		};
	}
};

}