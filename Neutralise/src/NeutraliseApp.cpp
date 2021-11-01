#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "NeutraliseLayer.h"

#include <Neurex/core/EntryPoint.h>

using namespace Neurex;

class Neutralise : public Application {
public:
	Neutralise()
		: Application(WindowProps("Neutralise - Neurex Editor", 1280, 720))

	{
		add_layer(make_scoped<NeutraliseLayer>());
	};

	~Neutralise(){};
};

Neurex::Application* Neurex::create_application() { return new Neutralise(); }