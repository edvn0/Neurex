#pragma once

#include "Scene.h"

#include "entt.hpp"

namespace Engine {

class Entity {
public:
	Entity() = default;
	Entity(entt::entity handle, Scene* scene);
	Entity(const Entity& other) = default;

	template <typename T, typename... Args> T& add_component(Args&&... args)
	{
		NX_CORE_ASSERT(!has_component<T>(), "Entity already has component!");
		T& component = scene->registry.emplace<T>(entity_handle, std::forward<Args>(args)...);
		scene->on_component_added<T>(*this, component);
		return component;
	}

	template <typename T> T& get_component()
	{
		NX_CORE_ASSERT(has_component<T>(), "Entity does not have component!");
		return scene->registry.get<T>(entity_handle);
	}

	template <typename T> bool has_component() { return scene->registry.has<T>(entity_handle); }

	template <typename T> void remove_component()
	{
		NX_CORE_ASSERT(has_component<T>(), "Entity does not have component!");
		scene->registry.remove<T>(entity_handle);
	}

	operator bool() const { return entity_handle != entt::null; }
	operator entt::entity() const { return entity_handle; }
	operator uint32_t() const { return (uint32_t)entity_handle; }

	bool operator==(const Entity& other) const
	{
		return entity_handle == other.entity_handle && scene == other.scene;
	}

	bool operator!=(const Entity& other) const { return !(*this == other); }

private:
	entt::entity entity_handle{ entt::null };
	Scene* scene = nullptr;
};

}