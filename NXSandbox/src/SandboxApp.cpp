#include <Neurex.h>


class ExampleLayer : public Neurex::Layer {
public:
	ExampleLayer(): Layer("Sandbox") {
	}

	void updated() override {
		
	}

	virtual void on_imgui_render() override {
	}

	void on_event(Neurex::Event& event)
	{
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