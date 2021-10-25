#pragma once

#include "Event.h"

namespace Neurex {

class KeyEvent : public Event {
protected:
	KeyEvent(int keycode)
		: key_code(keycode)
	{
	}

public:
	inline const int get_key_code() const { return key_code; }

	virtual int get_category() const { return EventCategory::EventCategoryKeyboard | EventCategory::EventCategoryInput; };

protected:
	int key_code;
};

class KeyPressedEvent : public KeyEvent {
public:
	KeyPressedEvent(int keycode, int repeats = 0)
		: KeyEvent(keycode)
		, repeat_count(repeats){};

	inline int get_repeat_count() const { return repeat_count; }

	std::string to_string() const override
	{
		std::stringstream ss;
		ss << "KeyPressedEvent: " << key_code << ", " << repeat_count;
		return ss.str();
	}

	static EventType get_static_type() { return EventType::KeyPressed; };
	virtual EventType get_event_type() const override { return get_static_type(); };
	virtual const char* get_name() const override { return "KeyPressed"; };

private:
	int repeat_count;
};

class KeyReleasedEvent : public KeyEvent {
public:
	KeyReleasedEvent(int keycode)
		: KeyEvent(keycode){};

	static EventType get_static_type() { return EventType::KeyReleased; };
	virtual EventType get_event_type() const override { return get_static_type(); };
	virtual const char* get_name() const override { return "KeyReleased"; };
};

class KeyTypedEvent : public KeyEvent {
public:
	KeyTypedEvent(int keycode)
		: KeyEvent(keycode){};

	std::string to_string() const override
	{
		std::stringstream ss;
		ss << "KeyTypedEvent: " << key_code;
		return ss.str();
	}

	static EventType get_static_type() { return EventType::KeyTyped; };
	virtual EventType get_event_type() const override { return get_static_type(); };
	virtual const char* get_name() const override { return "KeyTyped"; };
};
}