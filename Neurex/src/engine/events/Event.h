#pragma once

#include "engine/core/Core.h"
#include "nxpch.h"

namespace Engine {

enum class EventType {
	None = 0,
	WindowClose,
	WindowResize,
	WindowFocus,
	WindowLostFocus,
	WindowMoved,
	WindowFramebuffer,
	AppTick,
	AppUpdate,
	AppRender,
	KeyPressed,
	KeyReleased,
	KeyTyped,
	MouseButtonPressed,
	MouseButtonReleased,
	MouseMoved,
	MouseScrolled
};

enum EventCategory {
	None = 0,
	EventCategoryApplication = BIT(0),
	EventCategoryInput = BIT(1),
	EventCategoryKeyboard = BIT(2),
	EventCategoryMouse = BIT(3),
	EventCategoryMouseButton = BIT(4),
};

class Event {
	friend class EventDispatcher;

public:
	virtual EventType get_event_type() const = 0;
	virtual const char* get_name() const = 0;
	virtual int get_category() const = 0;
	virtual std::string to_string() const { return get_name(); }

	inline bool is_in_category(EventCategory category) { return get_category() & category; }

	explicit operator bool() const { return handled; }

	bool& get_handled() { return handled; }

protected:
	bool handled = false;
};

class EventDispatcher {
	template <typename T> using EventFn = std::function<bool(T&)>;

public:
	EventDispatcher(Event& event_)
		: event(event_){};

	template <typename T = Event> bool dispatch_event(EventFn<T> func)
	{
		if (event.get_event_type() == T::get_static_type()) {
			event.handled = func(*(T*)&event);
			return true;
		}
		return false;
	}

private:
	Event& event;
};

inline std::ostream& operator<<(std::ostream& os, const Event& e)
{
	os << e.to_string();
	return os;
}

}
