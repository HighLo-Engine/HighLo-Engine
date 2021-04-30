#pragma once

#include "HighLoPch.h"

// TODO: Move to Core file
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
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoves, FileMenu,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4),
	};

	template<typename T>
	using EventFn = std::function<bool(T&)>;

	#define REGISTER_EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
	virtual EventType GetEventType() const override { return GetStaticType(); }\
	virtual const char *GetName() const override { return #type; }

	#define REGISTER_EVENT_CLASS_CATEGORY(type) virtual int GetCategoryFlags() const override { return type; }

	class HLAPI Event
	{
	friend class EventDispatcher;

	public:
		bool m_Handled = false; /**< Indicates whether the Event has been handled or not. */

		/**
		 *
		 *  Is going to be implemented by the actual Events.
		 *
		 * @return Returns the correct EventType after it gets implemented by the subclass.
		 */
		virtual EventType GetEventType() const = 0;

		/**
		 *
		 * * Is going to be implemented by the actual Events.
		 *
		 * @return Returns the Name of the Event after it gets implemented by the subclass.
		 */
		virtual const char *GetName() const = 0;

		/**
		 *
		 * Is going to be implemented by the actual Events.
		 *
		 * @return Returns the CategoryFlags of the Event after it gets implemented by the subclass.
		 */
		virtual int GetCategoryFlags() const = 0;

		/**
		 *
		 * Converts the Name of the Event to a String.
		 *
		 * @return New FakeString with the Name of the correct EventName.
		 */
		virtual std::string ToString() const
		{
			return GetName();
		}

		/**
		 *
		 * Tells if the provided EventCategory is set in the implementing EventSubClass.
		 *
		 * @param c the Category which should be checked whether it is part of the EventSubClass.
		 * @return Returns true if the Category is a part of the EventCategories.
		 */
#pragma warning(push)
#pragma warning(disable: 26812)
		bool IsInCategory(EventCategory c)
		{
			return GetCategoryFlags() & c;
		}
#pragma warning(pop)
	};

	class HLAPI EventDispatcher
	{
	public:

		EventDispatcher(Event &e)
			: m_Event(e) {}

		template<typename EventType, typename FunctionType>
		bool Dispatch(const FunctionType &func)
		{
			if (m_Event.GetEventType() == EventType::GetStaticType())
			{
				m_Event.m_Handled = func(static_cast<EventType&>(m_Event));
				return true;
			}

			return false;
		}

	private:
		Event &m_Event;
	};

	inline std::ostream &operator<<(std::ostream &stream, const Event &e)
	{
		return stream << e.ToString();
	}
}