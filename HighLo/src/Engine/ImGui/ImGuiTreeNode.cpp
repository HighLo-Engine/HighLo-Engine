// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "ImGuiTreeNode.h"

#include "Engine/Math/Color.h"

namespace highlo::UI
{
	bool TreeNodeWithIcon(const Ref<Texture2D> &icon, ImGuiID id, ImGuiTreeNodeFlags flags, const char *label, const char *labelEnd, ImColor iconTint)
	{
		ImGuiWindow *window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext &g = *GImGui;
		ImGuiLastItemData &lastItem = g.LastItemData;
		const ImGuiStyle &style = g.Style;
		const bool displayFrame = (flags & ImGuiTreeNodeFlags_Framed) != 0;
		const ImVec2 padding = (displayFrame || (flags & ImGuiTreeNodeFlags_FramePadding)) ? style.FramePadding : ImVec2(style.FramePadding.x, ImMin(window->DC.CurrLineTextBaseOffset, style.FramePadding.y));

		if (!labelEnd)
			labelEnd = ImGui::FindRenderedTextEnd(label);

		const ImVec2 labelSize = ImGui::CalcTextSize(label, labelEnd, false);

		// We vertically grow up to current line height up the typical widget height.
		const float frameHeight = ImMax(ImMin(window->DC.CurrLineSize.y, g.FontSize + style.FramePadding.y * 2), labelSize.y + padding.y * 2);
		ImRect frameBB;
		frameBB.Min.x = (flags & ImGuiTreeNodeFlags_SpanFullWidth) ? window->WorkRect.Min.x : window->DC.CursorPos.x;
		frameBB.Min.y = window->DC.CursorPos.y;
		frameBB.Max.x = window->WorkRect.Max.x;
		frameBB.Max.y = window->DC.CursorPos.y + frameHeight;

		if (displayFrame)
		{
			frameBB.Min.x -= IM_FLOOR(window->WindowPadding.x * 0.5f - 1.0f);
			frameBB.Max.x -= IM_FLOOR(window->WindowPadding.x * 0.5f);
		}

		const float textOffsetX = g.FontSize + (displayFrame ? padding.x * 3 : padding.x * 2);
		const float textOffsetY = ImMax(padding.y, window->DC.CurrLineTextBaseOffset);
		const float textWidth = g.FontSize + (labelSize.x > 0.0f ? labelSize.x + padding.x * 2 : 0.0f);
		ImVec2 textPos(window->DC.CursorPos.x + textOffsetX, window->DC.CursorPos.y + textOffsetY);
		ImGui::ItemSize(ImVec2(textWidth, frameHeight), padding.y);

		ImRect intersectBB = frameBB;
		if (!displayFrame && (flags & (ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth)) == 0)
		{
			intersectBB.Max.x = frameBB.Min.x + textWidth + style.ItemSpacing.x * 2.0f;
		}

		const bool isLeaf = (flags & ImGuiTreeNodeFlags_Leaf) != 0;
		bool isOpen = ImGui::TreeNodeBehaviorIsOpen(id, flags);
		if (isOpen && !g.NavIdIsAlive && (flags & ImGuiTreeNodeFlags_NavLeftJumpsBackHere) && !(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
		{
			window->DC.TreeJumpToParentOnPopMask |= (1 << window->DC.TreeDepth);
		}

		bool itemAdded = ImGui::ItemAdd(intersectBB, id);
		lastItem.StatusFlags |= ImGuiItemStatusFlags_HasDisplayRect;
		lastItem.DisplayRect = frameBB;

		if (!itemAdded)
		{
			if (isOpen && !(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
				ImGui::TreePushOverrideID(id);

			IMGUI_TEST_ENGINE_ITEM_INFO(lastItem.ID, label, lastItem.StatusFlags | (isLeaf ? 0 : ImGuiItemStatusFlags_Openable) | (isOpen ? ImGuiItemStatusFlags_Opened : 0));
			return isOpen;
		}

		ImGuiButtonFlags buttonFlags = ImGuiTreeNodeFlags_None;
		if (flags & ImGuiTreeNodeFlags_AllowItemOverlap)
			buttonFlags |= ImGuiButtonFlags_AllowItemOverlap;
		if (!isLeaf)
			buttonFlags |= ImGuiButtonFlags_PressedOnDragDropHold;

		// We allow clicking on the arrow section with keyboard modifiers held, in order to easily
		// allow browsing a tree while preserving selection with code implementing multi-selection patterns.
		// When clicking on the rest of the tree node we always disallow keyboard modifiers.

		const float arrowHitX1 = (textPos.x - textOffsetX) - style.TouchExtraPadding.x;
		const float arrowHitX2 = (textPos.x - textOffsetX) + (g.FontSize + padding.x * 2.0f) + style.TouchExtraPadding.x;
		const bool isMouseXOverArrow = (g.IO.MousePos.x >= arrowHitX1 && g.IO.MousePos.x < arrowHitX2);

		if (window != g.HoveredWindow || !isMouseXOverArrow)
			buttonFlags |= ImGuiButtonFlags_NoKeyModifiers;

		if (isMouseXOverArrow)
			buttonFlags |= ImGuiButtonFlags_PressedOnClick;
		else if (flags & ImGuiTreeNodeFlags_OpenOnDoubleClick)
			buttonFlags |= ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnDoubleClick;
		else
			buttonFlags |= ImGuiButtonFlags_PressedOnClickRelease;

		bool isSelected = (flags & ImGuiTreeNodeFlags_Selected) != 0;
		const bool wasSelected = isSelected;

		bool hovered, held;
		bool pressed = ImGui::ButtonBehavior(intersectBB, id, &hovered, &held, buttonFlags);
		bool toggled = false;

		if (!isLeaf)
		{
			if (pressed && g.DragDropHoldJustPressedId != id)
			{
				if ((flags & (ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick)) == 0 || (g.NavActivateId == id))
					toggled = true;

				if (flags & ImGuiTreeNodeFlags_OpenOnArrow)
					toggled |= isMouseXOverArrow && !g.NavDisableMouseHover;

				if ((flags & ImGuiTreeNodeFlags_OpenOnDoubleClick) && g.IO.MouseDoubleClicked[0])
					toggled = true;
			}
			else if (pressed && g.DragDropHoldJustPressedId == id)
			{
				IM_ASSERT(buttonFlags & ImGuiButtonFlags_PressedOnDragDropHold);
				if (!isOpen)
					toggled = true;
			}

			if (g.NavId == id && g.NavMoveRequest && g.NavMoveDir == ImGuiDir_Left && isOpen)
			{
				toggled = true;
				ImGui::NavMoveRequestCancel();
			}

			if (g.NavId == id && g.NavMoveRequest && g.NavMoveDir == ImGuiDir_Right && !isOpen)
			{
				toggled = true;
				ImGui::NavMoveRequestCancel();
			}

			if (toggled)
			{
				isOpen = !isOpen;
				window->DC.StateStorage->SetInt(id, isOpen);
				lastItem.StatusFlags |= ImGuiItemStatusFlags_ToggledOpen;
			}
		}

		if (flags & ImGuiTreeNodeFlags_AllowItemOverlap)
			ImGui::SetItemAllowOverlap();

		if (isSelected != wasSelected)
			lastItem.StatusFlags |= ImGuiItemStatusFlags_ToggledSelection;

		// Render

		const ImU32 arrowColor = isSelected ? Colors::Theme::BackgroundDark : Colors::Theme::Muted;
		ImGuiNavHighlightFlags navFlags = ImGuiNavHighlightFlags_TypeThin;

		if (displayFrame)
		{
			const ImU32 bgColor = ImGui::GetColorU32((held && hovered) ? ImGuiCol_HeaderActive : (hovered && !isSelected && !held && !pressed && !toggled) ? ImGuiCol_HeaderHovered : ImGuiCol_Header);

			ImGui::RenderFrame(frameBB.Min, frameBB.Max, bgColor, true, style.FrameRounding);
			ImGui::RenderNavHighlight(frameBB, id, navFlags);

			// Render Arrow
			if (flags & ImGuiTreeNodeFlags_Bullet)
				ImGui::RenderBullet(window->DrawList, ImVec2(textPos.x - textOffsetX * 0.60f, textPos.y + g.FontSize * 0.5f), arrowColor);
			else if (!isLeaf)
				ImGui::RenderArrow(window->DrawList, ImVec2(textPos.x - textOffsetX + padding.x, textPos.y), arrowColor, isOpen ? ImGuiDir_Down : ImGuiDir_Right, 1.0f);
			else
				textPos.x -= textOffsetX;

			if (flags & ImGuiTreeNodeFlags_ClipLabelForTrailingButton)
				frameBB.Max.x -= g.FontSize + style.FramePadding.x;

			// Draw Icon
			if (icon)
			{
				// Store item data
				auto itemId = lastItem.ID;
				auto itemFlags = lastItem.InFlags;
				auto itemStatusFlags = lastItem.StatusFlags;
				auto itemRect = lastItem.Rect;

				// Draw icon image which messes up last item data
				const float pad = 3.0f;
				const float arrowWidth = 20.0f + 1.0f;
				auto cursorPos = ImGui::GetCursorPos();
				UI::ShiftCursorY(-frameHeight + pad);
				UI::ShiftCursorX(arrowWidth);
				UI::Image(icon, { frameHeight - pad * 2.0f, frameHeight - pad * 2.0f }, ImVec2(0, 0), ImVec2(1, 1), iconTint);

				// Restore itme data
				ImGui::SetLastItemData(itemId, itemFlags, itemStatusFlags, itemRect);
				textPos.x += frameHeight + 2.0f;

			}

			textPos.y -= 1.0f;

			if (g.LogEnabled)
			{
				const char logPrefix[] = "\n##";
				const char logSuffix[] = "##";
				ImGui::LogRenderedText(&textPos, logPrefix, logPrefix + 3);
				ImGui::RenderTextClipped(textPos, frameBB.Max, label, labelEnd, &labelSize);
				ImGui::LogRenderedText(&textPos, logSuffix, logSuffix + 2);
			}
			else
			{
				ImGui::RenderTextClipped(textPos, frameBB.Max, label, labelEnd, &labelSize);
			}
		}
		else
		{
			if (hovered || isSelected)
			{
				const ImU32 bgColor = ImGui::GetColorU32((held && hovered) ? ImGuiCol_HeaderActive : (hovered && !isSelected && !held && !pressed && !toggled) ? ImGuiCol_HeaderHovered : ImGuiCol_Header);

				ImGui::RenderFrame(frameBB.Min, frameBB.Max, bgColor, false);
				ImGui::RenderNavHighlight(frameBB, id, navFlags);
			}

			if (flags & ImGuiTreeNodeFlags_Bullet)
				ImGui::RenderBullet(window->DrawList, ImVec2(textPos.x - textOffsetX * 0.5f, textPos.y + g.FontSize * 0.5f), arrowColor);
			else
				ImGui::RenderArrow(window->DrawList, ImVec2(textPos.x - textOffsetX + padding.x, textPos.y + g.FontSize * 0.15f), arrowColor, isOpen ? ImGuiDir_Down : ImGuiDir_Right, 0.70f);

			if (icon)
			{
				// Store item data
				auto itemId = lastItem.ID;
				auto itemFlags = lastItem.InFlags;
				auto itemStatusFlags = lastItem.StatusFlags;
				auto itemRect = lastItem.Rect;

				// Draw icon image which messes up last item data
				const float pad = 3.0f;
				const float arrowWidth = 20.0f + 1.0f;
				auto cursorPos = ImGui::GetCursorPos();
				UI::ShiftCursorY(-frameHeight + pad);
				UI::ShiftCursorX(arrowWidth);
				UI::Image(icon, { frameHeight - pad * 2.0f, frameHeight - pad * 2.0f }, ImVec2(0, 0), ImVec2(1, 1), iconTint);

				// Restore itme data
				ImGui::SetLastItemData(itemId, itemFlags, itemStatusFlags, itemRect);
				textPos.x += frameHeight + 2.0f;
			}

			textPos.y -= 1.0f;

			if (g.LogEnabled)
				ImGui::LogRenderedText(&textPos, ">");
			ImGui::RenderText(textPos, label, labelEnd, false);
		}

		if (isOpen && !(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
			ImGui::TreePushOverrideID(id);

		IMGUI_TEST_ENGINE_ITEM_INFO(id, label, window->DC.ItemFlags | (isLeaf ? 0 : ImGuiItemStatusFlags_Openable) | (isOpen ? ImGuiItemStatusFlags_Opened : 0));
		return isOpen;
	}
	
	bool TreeNodeWithIcon(const Ref<Texture2D> &icon, const void *ptrID, ImGuiTreeNodeFlags flags, ImColor iconTint, const char *format, ...)
	{
		va_list args;
		va_start(args, format);
		ImGuiWindow *window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext &g = *GImGui;
		const char *labelEnd = g.TempBuffer + ImFormatStringV(g.TempBuffer, IM_ARRAYSIZE(g.TempBuffer), format, args);
		bool isOpen = TreeNodeWithIcon(icon, window->GetID(ptrID), flags, g.TempBuffer, labelEnd, iconTint);

		va_end(args);
		return isOpen;
	}
	
	bool TreeNodeWithIcon(const Ref<Texture2D> &icon, const char *label, ImGuiTreeNodeFlags flags, ImColor iconTint)
	{
		ImGuiWindow *window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		return TreeNodeWithIcon(icon, window->GetID(label), flags, label, NULL, iconTint);
	}
}

