#include "nxpch.h"

#include "Application.h"
#include "Neurex/core/Clock.h"
#include "Neurex/core/Input.h"
#include "Neurex/renderer/Renderer.h"

namespace Neurex {

Application* Application::instance = nullptr;

Application::Application()
{
	NX_CORE_ASSERT(!instance, "Application already exists.");
	instance = this;

	window = std::unique_ptr<Window>(Window::create());
	window->set_event_callback([&](Event& event) { this->on_event(event); });
	imgui_layer = make_scoped<ImGuiLayer>();

	Renderer::init();

	add_overlay(std::move(imgui_layer));
};

Application::~Application() = default;

void Application::run()
{
	while (is_running) {
		auto time = Clock::get_time();
		Timestep step = time - last_time;
		last_time = time;
		fps = (1.0 / step.get_seconds());

		for (auto& l : stack) {
			l->updated(step);
		}

		imgui_layer->begin();
		for (auto& l : stack) {
			l->on_imgui_render();
		}
		imgui_layer->end();

		window->on_update();
	}
}

void Application::on_event(Event& event)
{
	EventDispatcher dispatcher(event);

	dispatcher.dispatch_event<WindowCloseEvent>([&](WindowCloseEvent& e) {
		is_running = false;
		return true;
	});

	dispatcher.dispatch_event<WindowResizeEvent>([&](WindowResizeEvent& e) {
		window->resize_window(e.get_width(), e.get_height());
		return true;
	});

	dispatcher.dispatch_event<WindowFramebufferEvent>([&](WindowFramebufferEvent& e) {
		window->resize_framebuffer(e.get_width(), e.get_height());
		return true;
	});

	for (auto it = stack.end(); it != stack.begin();) {
		(*--it)->on_event(event);
		if (event) {
			break;
		}
	}
}

void Application::add_layer(scoped<Layer> layer)
{
	stack.push(std::move(layer));
}

void Application::add_overlay(scoped<Layer> overlay)
{
	stack.push_overlay(std::move(overlay));
}

}