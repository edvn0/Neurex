#pragma once

#include "nxpch.h"
#include "Neurex/Window.h"

#include <GLFW/glfw3.h>

namespace Neurex {
	class WindowsWindow : public Window {
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void on_update() override;

		size_t get_width() const override { return window_data.width; };
		size_t get_height() const override { return window_data.height; };

		void set_event_callback(const EventCallback& callback) override { window_data.callback = callback; };
		void set_vsync(bool enabled) override;
		bool is_vsync() override;
		void resize_window(float w, float h) const override;

		inline virtual void* get_natively() const { return win_window; };

	private:
		virtual void init(const WindowProps& props);
		virtual void shutdown();
		virtual void setup_events();
	private:
		GLFWwindow* win_window;

		struct WindowData {
			std::string title;
			size_t width;
			size_t height;
			bool vsync;

			EventCallback callback;
		};

		WindowData window_data;
	};
}
