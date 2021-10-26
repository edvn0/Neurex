#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Sandbox2D.h"

#include <Neurex/core/EntryPoint.h>

using namespace Neurex;
class Sandbox : public Application {
public:
	Sandbox()
	{
		// add_layer(make_scoped<ExampleLayer>());
		add_layer(make_scoped<Sandbox2D>());
	};

	~Sandbox(){};
};

Neurex::Application* Neurex::create_application() { return new Sandbox(); }