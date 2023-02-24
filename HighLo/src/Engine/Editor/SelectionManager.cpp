#include "HighLoPch.h"
#include "SelectionManager.h"

#include "Engine/Application/Application.h"

namespace highlo
{
	std::unordered_map<SelectionContext, std::vector<UUID>> SelectionManager::s_Contexts;

	void SelectionManager::Select(SelectionContext context, UUID selectionID)
	{
		auto &contextSelections = s_Contexts[context];
		if (std::find(contextSelections.begin(), contextSelections.end(), selectionID) != contextSelections.end())
		{
			// Already exists in the current context, abort
			return;
		}

		bool existsInAnotherContext = false;
		for (auto &[existingContext, existingContextSelections] : s_Contexts)
		{
			if (context == existingContext)
				continue;

			if (std::find(existingContextSelections.begin(), existingContextSelections.end(), selectionID) != existingContextSelections.end())
			{
				existsInAnotherContext = true;
				break;
			}
		}

		if (!existsInAnotherContext)
		{
			contextSelections.push_back(selectionID);
			HLApplication::Get().DispatchEvent<SelectionChangedEvent>(context, selectionID, true);
		}
	}

	bool SelectionManager::IsSelected(UUID selectionID)
	{
		for (const auto &[contextID, contextSelections] : s_Contexts)
		{
			if (std::find(contextSelections.begin(), contextSelections.end(), selectionID) != contextSelections.end())
			{
				return true;
			}
		}

		return false;
	}

	bool SelectionManager::IsSelected(SelectionContext context, UUID selectionID)
	{
		const auto &contextSelections = s_Contexts[context];
		return std::find(contextSelections.begin(), contextSelections.end(), selectionID) != contextSelections.end();
	}

	void SelectionManager::Deselect(UUID selectionID)
	{
		for (auto &[context, contextSelections] : s_Contexts)
		{
			auto it = std::find(contextSelections.begin(), contextSelections.end(), selectionID);
			if (it == contextSelections.end())
				continue;

			HLApplication::Get().DispatchEvent<SelectionChangedEvent>(context, selectionID, false);
			contextSelections.erase(it);
			break;
		}
	}

	void SelectionManager::Deselect(SelectionContext context, UUID selectionID)
	{
		auto &contextSelections = s_Contexts[context];
		auto it = std::find(contextSelections.begin(), contextSelections.end(), selectionID);
		if (it == contextSelections.end())
			return;

		contextSelections.erase(it);
	}

	void SelectionManager::DeselectAll()
	{
		for (auto &[context, contextSelections] : s_Contexts)
		{
			for (const auto &selectionID : contextSelections)
			{
				HLApplication::Get().DispatchEvent<SelectionChangedEvent>(context, selectionID, false);
			}

			contextSelections.clear();
		}
	}

	void SelectionManager::DeselectAll(SelectionContext context)
	{
		auto &contextSelections = s_Contexts[context];

		for (const auto &selectionID : contextSelections)
		{
			HLApplication::Get().DispatchEvent<SelectionChangedEvent>(context, selectionID, false);
		}

		contextSelections.clear();
	}

	UUID SelectionManager::GetSelection(SelectionContext context, uint32 index)
	{
		auto &contextSelections = s_Contexts[context];
		HL_ASSERT(index >= 0 && index < contextSelections.size());
		return contextSelections[index];
	}

	uint32 SelectionManager::GetSelectionCount(SelectionContext context)
	{
		return s_Contexts[context].size();
	}

	const std::vector<UUID> &SelectionManager::GetSelections(SelectionContext context)
	{
		return s_Contexts[context];
	}

}

