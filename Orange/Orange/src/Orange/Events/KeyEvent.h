#pragma once

#include "Orange/Events/Event.h"
#include "Orange/Core/KeyCodes.h"

namespace Orange
{
	class KeyEvent : public Event
	{
	public:
		KeyCode GetKeyCode() { return o_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput);

	protected:
		KeyEvent(const KeyCode keyCode)
			:o_KeyCode(keyCode) {}

		KeyCode o_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(const KeyCode keycode)
			:KeyEvent(keycode) {}

		KeyPressedEvent(const KeyCode keycode, bool isRepeat = false)
		:KeyEvent(keycode), o_IsRepeat(isRepeat) {}

		bool IsRepeat() const { return o_IsRepeat; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "���̰����¼�: " << o_KeyCode << " (�ظ�= " << o_IsRepeat << " ��)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		bool o_IsRepeat;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(const KeyCode keyCode)
			:KeyEvent(keyCode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "�����ͷ��¼�: " << o_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(const KeyCode keyCode)
			:KeyEvent(keyCode){}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "���������¼�: " << o_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}
