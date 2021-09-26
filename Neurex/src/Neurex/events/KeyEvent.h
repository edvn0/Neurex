#pragma once

#include "Event.h"

namespace Neurex 
{

	class NX_API KeyEvent : public Event
	{
	protected:
		KeyEvent(int keycode) : key_code(keycode) {}
	public:
		EVENT_CLASS_CATEGORY(EventCategoryApplication | EventCategoryInput)
	protected:
		int key_code;
	};

	class NX_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, int repeats = 0) : KeyEvent(keycode), repeat_count(repeats) {};
		
		inline int get_repeat_count() const { return repeat_count; }

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << key_code << ", " << repeat_count;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		int repeat_count;
	};

	class NX_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode) : KeyEvent(keycode) {};

		EVENT_CLASS_TYPE(KeyReleased)
	};
}