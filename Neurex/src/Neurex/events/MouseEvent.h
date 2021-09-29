#pragma once

#include "Event.h"

namespace Neurex
{
	class MouseMovedEvent : public Event
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
		
		static EventType get_static_type() { return EventType::MouseMoved; };
		virtual EventType get_event_type() const override { return get_static_type(); };
		virtual const char* get_name() const override { return "MouseMoved"; };
		virtual int get_category() const override { return EventCategory::EventCategoryMouse | EventCategory::EventCategoryInput; }
	private:
		float mouse_x, mouse_y;
	};

	class MouseScrolledEvent : public Event
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
		
		static EventType get_static_type() { return EventType::MouseScrolled; };
		virtual EventType get_event_type() const override { return get_static_type(); };
		virtual const char* get_name() const override { return "MouseScrolled"; };
		virtual int get_category() const override { return EventCategory::EventCategoryMouse | EventCategory::EventCategoryInput; }
	private:
		float offset_x, offset_y;
	};

	class MouseButtonPressedEvent : public Event
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
		
		static EventType get_static_type() { return EventType::MouseButtonPressed; };
		virtual EventType get_event_type() const override { return get_static_type(); };
		virtual const char* get_name() const override { return "MouseButtonPressed"; };
		virtual int get_category() const override { return EventCategory::EventCategoryMouseButton | EventCategory::EventCategoryInput; }
	private:
		int mouse_button;
	};

	class MouseButtonReleasedEvent : public Event
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
		
		static EventType get_static_type() { return EventType::MouseButtonReleased; };
		virtual EventType get_event_type() const override { return get_static_type(); };
		virtual const char* get_name() const override { return "MouseButtonReleased"; };
		virtual int get_category() const override { return EventCategory::EventCategoryMouseButton | EventCategory::EventCategoryInput; }
	private:
		int mouse_button;
	};
}
