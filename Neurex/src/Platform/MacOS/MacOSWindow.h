#pragma once

#include "Neurex/Window.h"
#include "Neurex/renderer/GraphicsContext.h"
#include "nxpch.h"


#include <GLFW/glfw3.h>

namespace Neurex {

class MacOSWindow : public Window {
public:
	MacOSWindow(const WindowProps& props);
	virtual ~MacOSWindow();

	void on_update() override;

	size_t get_width() const override { return window_data.width; };
	size_t get_height() const override { return window_data.height; };

	void set_event_callback(const EventCallback& callback) override { window_data.callback = callback; };
	void set_vsync(bool enabled) override;
	bool is_vsync() override;
	void resize_window(float w, float h) const override;
	void resize_framebuffer(int w, int h) const override;

	inline virtual void* get_natively() const override { return win_window; };

private:
	virtual void init(const WindowProps& props);
	virtual void shutdown();
	virtual void setup_events();

private:
	GLFWwindow* win_window;

	GraphicsContext* window_context;

	struct WindowData {
		std::string title = "Window";
		size_t width = 1280;
		size_t height = 720;
		bool vsync = true;

		EventCallback callback;
	};

	WindowData window_data;
};

}
