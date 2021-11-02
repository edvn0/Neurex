#include "Scene.h"
#include "nxpch.h"

#include "Components.h"
#include "Neurex/renderer/Renderer2D.h"

#include <glm/glm.hpp>

#include "Entity.h"

namespace Neurex {

Scene::Scene() { }

Scene::~Scene() { }

Entity Scene::create_entity(const std::string& name)
{
	Entity entity = { registry.create(), this };
	entity.add_component<TransformComponent>();
	auto& tag = entity.add_component<TagComponent>();
	tag.tag = name.empty() ? "Entity" : name;
	return entity;
}

void Scene::destroy_entity(Entity entity) { registry.destroy(entity); }

void Scene::on_update_runtime(Timestep ts)
{
	// Update scripts
	{
		registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {
			// TODO: Move to Scene::OnScenePlay
			if (!nsc.instance) {
				nsc.instance = nsc.instantiate_script();
				nsc.instance->entity = Entity{ entity, this };
				nsc.instance->on_create();
			}

			nsc.instance->on_update(ts);
		});
	}

	// Render 2D
	Camera* main_camera = nullptr;
	glm::mat4 camera_transform;
	{
		auto view = registry.view<TransformComponent, CameraComponent>();
		for (auto entity : view) {
			auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

			if (camera.primary) {
				main_camera = &camera.camera;
				camera_transform = transform.get_transform();
				break;
			}
		}
	}

	if (main_camera) {
		Renderer2D::begin_scene(*main_camera, camera_transform);

		auto group = registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		// for (auto entity : group) {
		//	auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
		//
		//	Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);

		Renderer2D::end_scene();
	}
}

void Scene::on_update_editor(Timestep ts, EditorCamera& camera)
{
	Renderer2D::begin_scene(camera);

	auto group = registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
	// for (auto entity : group) {
	//	auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
	//
	//	Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);

	Renderer2D::end_scene();
}

void Scene::on_viewport_resize(uint32_t width, uint32_t height)
{
	viewport_width = width;
	viewport_height = height;

	// Resize our non-FixedAspectRatio cameras
	auto view = registry.view<CameraComponent>();
	for (auto entity : view) {
		auto& cameraComponent = view.get<CameraComponent>(entity);
		if (!cameraComponent.fixed_aspect_ratio)
			cameraComponent.camera.set_viewport_size(width, height);
	}
}

Entity Scene::get_primary_camera_entity()
{
	auto view = registry.view<CameraComponent>();
	for (auto entity : view) {
		const auto& camera = view.get<CameraComponent>(entity);
		if (camera.primary)
			return Entity{ entity, this };
	}
	return {};
}

template <typename T> void Scene::on_component_added(Entity entity, T& component) { static_assert(true); }

template <> void Scene::on_component_added<TransformComponent>(Entity entity, TransformComponent& component) { }

template <> void Scene::on_component_added<CameraComponent>(Entity entity, CameraComponent& component)
{
	if (viewport_width > 0 && viewport_height > 0)
		component.camera.set_viewport_size(viewport_width, viewport_height);
}

template <>
void Scene::on_component_added<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
{
}

template <> void Scene::on_component_added<TagComponent>(Entity entity, TagComponent& component) { }

template <> void Scene::on_component_added<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
{
}
}