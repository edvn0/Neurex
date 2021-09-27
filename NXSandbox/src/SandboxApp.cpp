#include <Neurex.h>

class ExampleLayer : public Neurex::Layer {
public:
	ExampleLayer(): Layer("Sandbox") {

	}

	void updated() override {
		NX_INFO("ExampleLayer::updated");
	}

	void on_event(Neurex::Event& event) {
		NX_TRACE("{0}", event);
	}
};


class Sandbox : public Neurex::Application
{
public:
	Sandbox() {
		add_layer(new ExampleLayer());
	};
	
	~Sandbox() {};
};

Neurex::Application* Neurex::create_application() {
	return new Sandbox();
}