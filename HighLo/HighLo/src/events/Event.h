#pragma once
#include <core/HLCore.h>
#include <string>
#include <sstream>

#define BIT(x) (1 << x)

namespace highlo
{
	/*
	*
	* Thank you to TheCherno and his YouTube videos that first showed
	* how to implement this event system, I am adopting the same system
	* in this engine because it is very efficient and easy to use in my
	* opinion. All credit for the "Event" class goes to TheCherno.
	*
	*/

	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryWindow       = BIT(0),
		EventCategoryInput        = BIT(1),
		EventCategoryKeyboard     = BIT(2),
		EventCategoryMouse        = BIT(3),
		EventCategoryMouseButton  = BIT(4)
	};

#define REGISTER_EVT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define REGISTER_EVT_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class Event
	{
	public:
		bool Handled = false;

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

#pragma warning(push)
#pragma warning(disable: 26812)
		inline bool IsInCategory(EventCategory category) { return GetCategoryFlags() & category; }
#pragma warning(pop)
	};
}
