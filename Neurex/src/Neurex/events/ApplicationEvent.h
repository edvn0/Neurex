#pragma once

#include "Event.h"

namespace Neurex
{

	class NX_API WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(float width_, float height_): width(width_), height(height_) {}

		inline float get_width() const { return width; }
		inline float get_height() const { return height; }

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << width << ", " << height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		float width, height;
	};

	class NX_API WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}
		
		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class NX_API AppTickEvent : public Event
	{
	public:
		AppTickEvent() {};

		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class NX_API AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() {};

		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class NX_API AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() {};

		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

}
