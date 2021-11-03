#pragma once

#include "Scene.h"

namespace Engine {

class SceneSerializer {
public:
	SceneSerializer(const ref<Scene>& scene);

	void serialize(const std::string& filepath);
	void serialize_runtime(const std::string& filepath);

	bool deserialize(const std::string& filepath);
	bool deserialize_runtime(const std::string& filepath);

private:
	ref<Scene> scene;
};

}