#pragma once

#include "nxpch.h"
#include "Window.h"

#include <GLFW/glfw3.h>

namespace Neurex {

	class WindowsWindow : public Window {
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		// Inherited via Window
		virtual size_t get_width() const override { return window_data.width; };
		virtual size_t get_height() const override { return window_data.height; };;

		virtual void set_event_callback(const EventCallback& callback) override { window_data.callback = callback; };
		virtual void set_vsync(bool enabled) override;
		virtual bool is_vsync() override;
	private:
		virtual void init(const WindowProps& props);
		virtual void shutdown();
	private:
		GLFWwindow* win_window;

		struct WindowData {
			std::string title;
			unsigned int width;
			unsigned int height;
			bool vsync;

			EventCallback callback;
		};

		WindowData window_data;
	};
}
