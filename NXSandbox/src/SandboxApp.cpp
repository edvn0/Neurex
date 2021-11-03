#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Sandbox2D.h"

#include <Engine/core/EntryPoint.h>

using namespace Engine;
class Sandbox : public Application {
public:
	Sandbox()
		: Application(WindowProps("Sandbox", 1280, 720))
	{
		// add_layer(make_scoped<ExampleLayer>());
		add_layer(make_scoped<Sandbox2D>());
	};

	~Sandbox(){};
};

Engine::Application* Engine::create_application() { return new Sandbox(); }