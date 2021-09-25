#include <Neurex.h>

class Sandbox : public Neurex::Application
{
public:
	Sandbox() {};
	~Sandbox() {};
};

Neurex::Application* Neurex::create_application() {
	return new Sandbox();
}