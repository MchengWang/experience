#pragma once

#include "Orange/Debug/Instrumentor.h"
#include "Orange/Core/Base.h"

#include <functional>

namespace Orange
{
	// Orange �е��¼���ǰ��������״̬������ζ�ŵ��¼�����ʱ���������������ȣ�������������
	// ���ڽ��������õĲ��Կ��������¼��л����¼����ߣ����ڸ��½׶ε� ��event�� ���ִ������ǡ�

	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication            = BIT(0),
		EventCategoryInput                  = BIT(1),
		EventCategoryKeyboard               = BIT(2),
		EventCategoryMouse                  = BIT(3),
		EventCategoryMouseButton            = BIT(4)
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }
#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class Event
	{
		friend class EventDispatcher;
	public:
		bool Handled = false;

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event)
			:o_Event(event)
		{
		}

		template<typename T, typename F>
		bool Dispatch(const F& func)
		{
			if (o_Event.GetEventType() == T::GetStaticType())
			{
				o_Event.Handled |= func(static_cast<T&>(o_Event));
				return true;
			}

			return false;
		}

	private:
		Event& o_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}

}
