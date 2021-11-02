#include "SceneSerialiser.h"
#include "nxpch.h"

#include "Components.h"
#include "Entity.h"

#include <fstream>

#include <yaml-cpp/yaml.h>

namespace YAML {

template <> struct convert<glm::vec3> {
	static Node encode(const glm::vec3& rhs)
	{
		Node node;
		node.push_back(rhs.x);
		node.push_back(rhs.y);
		node.push_back(rhs.z);
		node.SetStyle(EmitterStyle::Flow);
		return node;
	}

	static bool decode(const Node& node, glm::vec3& rhs)
	{
		if (!node.IsSequence() || node.size() != 3)
			return false;

		rhs.x = node[0].as<float>();
		rhs.y = node[1].as<float>();
		rhs.z = node[2].as<float>();
		return true;
	}
};

template <> struct convert<glm::vec4> {
	static Node encode(const glm::vec4& rhs)
	{
		Node node;
		node.push_back(rhs.x);
		node.push_back(rhs.y);
		node.push_back(rhs.z);
		node.push_back(rhs.w);
		node.SetStyle(EmitterStyle::Flow);
		return node;
	}

	static bool decode(const Node& node, glm::vec4& rhs)
	{
		if (!node.IsSequence() || node.size() != 4)
			return false;

		rhs.x = node[0].as<float>();
		rhs.y = node[1].as<float>();
		rhs.z = node[2].as<float>();
		rhs.w = node[3].as<float>();
		return true;
	}
};

}
namespace Neurex {

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
	return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
	return out;
}

SceneSerializer::SceneSerializer(const ref<Scene>& scene_)
	: scene(scene_)
{
}

static void serialize_entity(YAML::Emitter& out, Entity entity)
{
	out << YAML::BeginMap; // Entity
	out << YAML::Key << "Entity" << YAML::Value << "12837192831273"; // TODO: Entity ID goes here

	if (entity.has_component<TagComponent>()) {
		out << YAML::Key << "TagComponent";
		out << YAML::BeginMap; // TagComponent

		auto& tag = entity.get_component<TagComponent>().tag;
		out << YAML::Key << "Tag" << YAML::Value << tag;

		out << YAML::EndMap; // TagComponent
	}

	if (entity.has_component<TransformComponent>()) {
		out << YAML::Key << "TransformComponent";
		out << YAML::BeginMap; // TransformComponent

		auto& tc = entity.get_component<TransformComponent>();
		out << YAML::Key << "Translation" << YAML::Value << tc.translation;
		out << YAML::Key << "Rotation" << YAML::Value << tc.rotation;
		out << YAML::Key << "Scale" << YAML::Value << tc.scale;

		out << YAML::EndMap; // TransformComponent
	}

	if (entity.has_component<CameraComponent>()) {
		out << YAML::Key << "CameraComponent";
		out << YAML::BeginMap; // CameraComponent

		auto& cameraComponent = entity.get_component<CameraComponent>();
		auto& camera = cameraComponent.camera;

		out << YAML::Key << "Camera" << YAML::Value;
		out << YAML::BeginMap; // Camera
		out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.get_projection_type();
		out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.get_perspective_vertical_FOV();
		out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.get_perspective_near_clip();
		out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.get_perspective_far_clip();
		out << YAML::Key << "OrthographicSize" << YAML::Value << camera.get_orthographic_size();
		out << YAML::Key << "OrthographicNear" << YAML::Value << camera.get_orthographic_near_clip();
		out << YAML::Key << "OrthographicFar" << YAML::Value << camera.get_orthographic_far_clip();
		out << YAML::EndMap; // Camera

		out << YAML::Key << "Primary" << YAML::Value << cameraComponent.primary;
		out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.fixed_aspect_ratio;

		out << YAML::EndMap; // CameraComponent
	}

	if (entity.has_component<SpriteRendererComponent>()) {
		out << YAML::Key << "SpriteRendererComponent";
		out << YAML::BeginMap; // SpriteRendererComponent

		auto& spriteRendererComponent = entity.get_component<SpriteRendererComponent>();
		out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.colour;

		out << YAML::EndMap; // SpriteRendererComponent
	}

	out << YAML::EndMap; // Entity
}

void SceneSerializer::serialize(const std::string& filepath)
{
	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "Scene" << YAML::Value << "Untitled";
	out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
	scene->registry.each([&](auto entityID) {
		Entity entity = { entityID, scene.get() };
		if (!entity)
			return;

		serialize_entity(out, entity);
	});
	out << YAML::EndSeq;
	out << YAML::EndMap;

	std::ofstream fout(filepath);
	fout << out.c_str();
}

void SceneSerializer::serialize_runtime(const std::string& filepath)
{
	// Not implemented
}

bool SceneSerializer::deserialize(const std::string& filepath)
{
	YAML::Node data;
	try {
		data = YAML::LoadFile(filepath);
	} catch (YAML::ParserException e) {
		return false;
	}

	if (!data["Scene"])
		return false;

	std::string sceneName = data["Scene"].as<std::string>();
	NX_CORE_TRACE("Deserializing scene '{0}'", sceneName);

	auto entities = data["Entities"];
	if (entities) {
		for (auto entity : entities) {
			uint64_t uuid = entity["Entity"].as<uint64_t>(); // TODO

			std::string name;
			auto tagComponent = entity["TagComponent"];
			if (tagComponent)
				name = tagComponent["Tag"].as<std::string>();

			NX_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", uuid, name);

			Entity deserializedEntity = scene->create_entity(name);

			auto transformComponent = entity["TransformComponent"];
			if (transformComponent) {
				// Entities always have transforms
				auto& tc = deserializedEntity.get_component<TransformComponent>();
				tc.translation = transformComponent["Translation"].as<glm::vec3>();
				tc.rotation = transformComponent["Rotation"].as<glm::vec3>();
				tc.scale = transformComponent["Scale"].as<glm::vec3>();
			}

			auto cameraComponent = entity["CameraComponent"];
			if (cameraComponent) {
				auto& cc = deserializedEntity.add_component<CameraComponent>();

				auto cameraProps = cameraComponent["Camera"];
				cc.camera.set_projection_type(
					(SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());

				cc.camera.set_perspective_verticalFOV(cameraProps["PerspectiveFOV"].as<float>());
				cc.camera.set_perspective_near_clip(cameraProps["PerspectiveNear"].as<float>());
				cc.camera.set_perspective_far_clip(cameraProps["PerspectiveFar"].as<float>());

				cc.camera.set_orthographic_size(cameraProps["OrthographicSize"].as<float>());
				cc.camera.set_orthographic_near_clip(cameraProps["OrthographicNear"].as<float>());
				cc.camera.set_orthographic_far_clip(cameraProps["OrthographicFar"].as<float>());

				cc.primary = cameraComponent["Primary"].as<bool>();
				cc.fixed_aspect_ratio = cameraComponent["FixedAspectRatio"].as<bool>();
			}

			auto spriteRendererComponent = entity["SpriteRendererComponent"];
			if (spriteRendererComponent) {
				auto& src = deserializedEntity.add_component<SpriteRendererComponent>();
				src.colour = spriteRendererComponent["Color"].as<glm::vec4>();
			}
		}
	}

	return true;
}

bool SceneSerializer::deserialize_runtime(const std::string& filepath)
{
	// Not implemented
	NX_CORE_ASSERT(false, "");
	return false;
}

}