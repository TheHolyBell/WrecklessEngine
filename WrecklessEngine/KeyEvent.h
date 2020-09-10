#pragma once
#include "Event.h"
#include "KeyCode.h"
#include <sstream>

class KeyEvent : public Event
{
public:
	inline Input::KeyCode GetKeyCode() const { return m_KeyCode; }

	EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
protected:
	KeyEvent(Input::KeyCode keycode)
		: m_KeyCode(keycode) {}

	Input::KeyCode m_KeyCode;
};

class KeyPressedEvent : public KeyEvent
{
public:
	KeyPressedEvent(Input::KeyCode keycode, int repeatCount = 0)
		: KeyEvent(keycode), m_RepeatCount(repeatCount) {}

	inline int GetRepeatCount() const { return m_RepeatCount; }

	virtual std::string ToString() const override
	{
		std::ostringstream oss;
		oss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
		return oss.str();
	}

	EVENT_CLASS_TYPE(KeyPressed)
private:
	int m_RepeatCount;
};

class KeyReleasedEvent : public KeyEvent
{
public:
	KeyReleasedEvent(Input::KeyCode keycode)
		: KeyEvent(keycode){}

	virtual std::string ToString() const override
	{
		std::ostringstream oss;
		oss << "KeyReleasedEvent: " << m_KeyCode;
		return oss.str();
	}

	EVENT_CLASS_TYPE(KeyReleased);
};

class KeyTypedEvent : public KeyEvent
{
public:
	KeyTypedEvent(Input::KeyCode keycode)
		: KeyEvent(keycode)
	{}

	virtual std::string ToString() const override
	{
		std::ostringstream oss;
		oss << "KeyTypedEvent: " << m_KeyCode;
		return oss.str();
	}

	EVENT_CLASS_TYPE(KeyTyped);
};