#pragma once

#include "Event.h"

namespace Neurex
{
	class NX_API MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float x, float y) : mouse_x(x), mouse_y(y) {}

		inline float get_x() const { return mouse_x; }
		inline float get_y() const { return mouse_y; }

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << mouse_x << ", " << mouse_y;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float mouse_x, mouse_y;
	};

	class NX_API MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float x, float y) : offset_x(x), offset_y(y) {}

		inline float get_offset_x() const { return offset_x; }
		inline float get_offset_y() const { return offset_y; }

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << offset_x << ", " << offset_y;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float offset_x, offset_y;
	};

	class NX_API MouseButtonPressedEvent : public Event
	{
	public:
		MouseButtonPressedEvent(int mouse_button_): mouse_button(mouse_button_) {};

		inline int get_mouse_button() const { return mouse_button; }

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << mouse_button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		int mouse_button;
	};

	class NX_API MouseButtonReleasedEvent : public Event
	{
	public:
		MouseButtonReleasedEvent(int mouse_button_) : mouse_button(mouse_button_) {};

		inline int get_mouse_button() const { return mouse_button; }

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << mouse_button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		int mouse_button;
	};
}