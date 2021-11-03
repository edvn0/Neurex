#pragma once

#include "engine/core/Timestep.h"
#include "engine/renderer/EditorCamera.h"

#include <entt.hpp>

namespace Engine {

class Entity;

class Scene {
public:
	Scene();
	~Scene();

	Entity create_entity(const std::string& name = std::string());
	void destroy_entity(Entity entity);

	void on_update_runtime(Timestep ts);
	void on_update_editor(Timestep ts, EditorCamera& camera);
	void on_viewport_resize(uint32_t width, uint32_t height);

	Entity get_primary_camera_entity();

private:
	template <typename T> void on_component_added(Entity entity, T& component);

private:
	entt::registry registry;
	uint32_t viewport_width = 0, viewport_height = 0;

	friend class Entity;
	friend class SceneSerializer;
	friend class SceneHierarchyPanel;
};

}