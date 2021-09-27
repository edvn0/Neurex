#pragma once

#include "nxpch.h"

#include "Core.h"
#include "Events/Event.h"

namespace Neurex {

	struct WindowProps {
		std::string title;
		size_t width;
		size_t height;

		WindowProps(const std::string& title_ = "Neurex Engine", size_t w = 1280, size_t h = 720) : title(title_), width(w), height(h) {}
	};

	class NX_API Window {
	public:
		using EventCallback = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual size_t get_width() const = 0;
		virtual size_t get_height() const = 0;

		virtual void on_update() = 0;

		virtual void set_event_callback(const EventCallback& callback) = 0;
		virtual void set_vsync(bool enabled) = 0;
		virtual bool is_vsync() = 0;
		virtual void resize_window(float w, float h) const = 0;

		virtual void* get_natively() const = 0;

		static Window* create(const WindowProps& props = WindowProps());
	};
}