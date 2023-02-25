#pragma once

#include "Engine/Core/UUID.h"

namespace highlo
{
	enum class SelectionContext
	{
		Global = 0,
		Scene,
		AssetBrowserPanel
	};

	class SelectionManager
	{
	public:

		HLAPI static void Select(SelectionContext context, UUID selectionID);
		HLAPI static bool IsSelected(UUID selectionID);
		HLAPI static bool IsSelected(SelectionContext context, UUID selectionID);
		HLAPI static void Deselect(UUID selectionID);
		HLAPI static void Deselect(SelectionContext context, UUID selectionID);
		HLAPI static void DeselectAll();
		HLAPI static void DeselectAll(SelectionContext context);
		HLAPI static UUID GetSelection(SelectionContext context, uint32 index);

		HLAPI static uint32 GetSelectionCount(SelectionContext context);
		HLAPI static const std::vector<UUID> &GetSelections(SelectionContext context);

	private:

		static std::unordered_map<SelectionContext, std::vector<UUID>> s_Contexts;
	};
}

