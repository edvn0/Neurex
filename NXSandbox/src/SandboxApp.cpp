#include <Neurex.h>

class ExampleLayer : public Neurex::Layer {
public:
	ExampleLayer(): Layer("Sandbox") {
	}

	void updated() override {
		if (Neurex::Input::is_key_pressed(NX_KC_TAB))
		{
			NX_INFO("Tab was pressed!");
		}
	}

	void on_event(Neurex::Event& event)
	{
		if (event.get_event_type() == Neurex::EventType::KeyPressed) {
			auto& e = static_cast<Neurex::KeyPressedEvent&>(event);
			NX_TRACE("{0}", e.get_key_code());
		}
	}
};


class Sandbox : public Neurex::Application
{
public:
	Sandbox() {
		add_layer(new ExampleLayer());
		add_overlay(new Neurex::ImGuiLayer());
	};
	
	~Sandbox() {};
};

Neurex::Application* Neurex::create_application() {
	return new Sandbox();
}