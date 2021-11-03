#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "NeutraliseLayer.h"

#include <engine/core/EntryPoint.h>

using namespace Engine;

class Neutralise : public Application {
public:
	Neutralise()
		: Application(WindowProps("Neutralise - Neurex Editor", 1280, 720))

	{
		add_layer(make_scoped<NeutraliseLayer>());
	};

	~Neutralise(){};
};

Engine::Application* Engine::create_application() { return new Neutralise(); }