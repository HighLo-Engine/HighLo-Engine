#pragma once

#include "EventBase.h"
#include "Engine/Core/UUID.h"
#include "Engine/Editor/SelectionManager.h"

namespace highlo
{
	class SelectionChangedEvent : public Event
	{
	public:

		SelectionChangedEvent(SelectionContext context, UUID selection, bool selected)
			: m_Context(context), m_Selection(selection), m_Selected(selected)
		{
		}

		HLAPI SelectionContext GetContext() const { return m_Context; }
		HLAPI UUID GetSelection() const { return m_Selection; }
		HLAPI bool IsSelected() const { return m_Selected; }

		HLAPI HLString ToString() const override
		{
			std::stringstream ss;
			ss << "EntitySelectionChangedEvent: Context(" << (int32)m_Context << "), Selection(" << m_Selection << "), " << m_Selected;
			return ss.str();
		}

		REGISTER_EVENT_CLASS_CATEGORY(EventCategoryScene)
		REGISTER_EVENT_CLASS_TYPE(SelectionChanged)

	private:

		SelectionContext m_Context;
		UUID m_Selection;
		bool m_Selected;
	};
}

