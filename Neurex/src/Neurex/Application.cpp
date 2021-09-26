#include "nxpch.h"

#include "Application.h"

namespace Neurex {

	Application::Application()
	{
		WindowResizeEvent event(1000, 100);
		KeyPressedEvent key_pressed(96);
		MouseMovedEvent moved(100, 100);
		std::cout << event.to_string() << "\n";
		std::cout << key_pressed.to_string() << "\n";
		std::cout << moved.to_string();
	}

	Application::~Application()
	{
	}

	void Application::start()
	{
		while (true);
	}

}
