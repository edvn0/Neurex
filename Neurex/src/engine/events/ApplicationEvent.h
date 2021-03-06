#pragma once

#include "engine/events/Event.h"

namespace Engine {
class WindowFramebufferEvent : public Event {
public:
	WindowFramebufferEvent(int w, int h)
		: width(w)
		, height(h)
	{
	}

	inline int get_width() const { return width; }
	inline int get_height() const { return height; }

	static EventType get_static_type() { return EventType::WindowFramebuffer; };
	virtual EventType get_event_type() const override { return get_static_type(); };
	virtual const char* get_name() const override { return "WindowFramebuffer"; };
	virtual int get_category() const override { return EventCategory::EventCategoryApplication; };

private:
	int width;
	int height;
};

class WindowResizeEvent : public Event {
public:
	WindowResizeEvent(float width_, float height_)
		: width(width_)
		, height(height_)
	{
	}

	inline float get_width() const { return width; }
	inline float get_height() const { return height; }

	std::string to_string() const override
	{
		std::stringstream ss;
		ss << "WindowResizeEvent: " << width << ", " << height;
		return ss.str();
	}

	static EventType get_static_type() { return EventType::WindowResize; };
	virtual EventType get_event_type() const override { return get_static_type(); };
	virtual const char* get_name() const override { return "WindowResize"; };
	virtual int get_category() const override { return EventCategory::EventCategoryApplication; };

private:
	float width, height;
};

class WindowCloseEvent : public Event {
public:
	WindowCloseEvent() { }
	static EventType get_static_type() { return EventType::WindowClose; };
	virtual EventType get_event_type() const override { return get_static_type(); };
	virtual const char* get_name() const override { return "WindowClose"; };
	virtual int get_category() const override { return EventCategory::EventCategoryApplication; };
};

class AppTickEvent : public Event {
public:
	AppTickEvent(){};
	static EventType get_static_type() { return EventType::AppTick; };
	virtual EventType get_event_type() const override { return get_static_type(); };
	virtual const char* get_name() const override { return "AppTick"; };
	virtual int get_category() const override { return EventCategory::EventCategoryApplication; };
};

class AppRenderEvent : public Event {
public:
	AppRenderEvent(){};
	static EventType get_static_type() { return EventType::AppRender; };
	virtual EventType get_event_type() const override { return get_static_type(); };
	virtual const char* get_name() const override { return "AppRender"; };
	virtual int get_category() const override { return EventCategory::EventCategoryApplication; };
};

class AppUpdateEvent : public Event {
public:
	AppUpdateEvent(){};
	static EventType get_static_type() { return EventType::AppUpdate; };
	virtual EventType get_event_type() const override { return get_static_type(); };
	virtual const char* get_name() const override { return "AppUpdate"; };
	virtual int get_category() const override { return EventCategory::EventCategoryApplication; };
};

}
