// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-11-03) initial release
//

#pragma once

#include "imgui.h"
#include "ImGuiScopeHelpers.h"
#include "Engine/Math/Color.h"

#include "Engine/Assets/Asset.h"
#include "Engine/Assets/AssetTypes.h"
#include "Engine/Assets/AssetManager.h"
#include "Engine/Application/Application.h"

namespace highlo::UI
{
	static bool IsMatchingSearch(const HLString &item, const HLString &searchQuery, bool caseSensitive = false, bool stripWhiteSpaces = true, bool stripUnderScores = true)
	{
		if (searchQuery.IsEmpty())
			return true;

		if (item.IsEmpty())
			return false;

		HLString itemRaw = item;
		HLString nameSanitized = stripUnderScores ? itemRaw.Replace("_", "") : item;

		if (stripWhiteSpaces)
			nameSanitized = nameSanitized.Replace(" ", "");

		HLString searchQueryRaw = searchQuery;
		HLString searchString = stripWhiteSpaces ? searchQueryRaw.Replace(" ", "") : searchQuery;

		if (!caseSensitive)
		{
			nameSanitized = nameSanitized.ToLowerCase();
			searchString = searchString.ToLowerCase();
		}

		return nameSanitized.Contains(searchString);
	}

	class Widgets
	{
	public:

		HLAPI static void Init();
		HLAPI static void Shutdown();

		template<uint32 BufferSize = 256, typename StringType = HLString>
		HLAPI static bool SearchWidget(StringType &searchString, const char *hint = nullptr, bool *grabFocus = nullptr)
		{
			UI::PushID();

			UI::ShiftCursorY(1.0f);

			HLString searchPlaceholder = "";
			if (hint)
			{
				searchPlaceholder = hint;
			}
			else
			{
				Translation *translation = HLApplication::Get().GetActiveTranslation();
				searchPlaceholder = translation->GetText("imgui-widgets-search-widget-placeholder");
			}

			const bool layoutSuspended = []
			{
				ImGuiWindow *window = ImGui::GetCurrentWindow();
				if (window->DC.CurrentLayout)
				{
					ImGui::SuspendLayout();
					return true;
				}
				return false;
			}();

			bool modified = false;
			bool searching = false;

			const float areaPosX = ImGui::GetCursorPosX();
			const float framePaddingY = ImGui::GetStyle().FramePadding.y;

			UI::ScopedStyle rounding(ImGuiStyleVar_FrameRounding, 3.0f);
			UI::ScopedStyle padding(ImGuiStyleVar_FramePadding, ImVec2(28.0f, framePaddingY));

			if constexpr (std::is_same<StringType, HLString>::value)
			{
				char searchBuffer[BufferSize]{};

				// Only copy the content if any is available, otherwise strcpy_s will throw a weird exception ("" seems not be copyable)
				if (!searchString.IsEmpty())
					strcpy_s<BufferSize>(searchBuffer, *searchString);

				if (ImGui::InputText(GenerateID(), searchBuffer, BufferSize))
				{
					searchString = searchBuffer;
					modified = true;
				}
				else if (ImGui::IsItemDeactivatedAfterEdit())
				{
					searchString = searchBuffer;
					modified = true;
				}

				searching = searchBuffer[0] != 0;
			}
			else
			{
				static_assert(std::is_same<decltype(&searchString[0]), char *>::value,
							  "searchString paramenter must be HLString& or char*");

				if (ImGui::InputText(GenerateID(), searchString, BufferSize))
				{
					modified = true;
				}
				else if (ImGui::IsItemDeactivatedAfterEdit())
				{
					modified = true;
				}

				searching = searchString[0] != 0;
			}

			if (grabFocus && *grabFocus)
			{
				if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows)
					&& !ImGui::IsAnyItemActive()
					&& !ImGui::IsMouseClicked(0))
				{
					ImGui::SetKeyboardFocusHere(-1);
				}

				if (ImGui::IsItemFocused())
					*grabFocus = false;
			}

			UI::DrawItemActivityOutline(3.0f, true, Colors::Theme::Accent);
			ImGui::SetItemAllowOverlap();

			ImGui::SameLine(areaPosX + 5.0f);

			if (layoutSuspended)
				ImGui::ResumeLayout();

			ImGui::BeginHorizontal(GenerateID(), ImGui::GetItemRectSize());
			const ImVec2 iconSize(ImGui::GetTextLineHeight(), ImGui::GetTextLineHeight());

			// Search icon
			{
				const float iconYOffset = framePaddingY - 3.0f;
				UI::ShiftCursorY(iconYOffset);
				UI::Image(s_SearchIcon, iconSize, ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 0.2f));
				UI::ShiftCursorY(-iconYOffset);

				// Hint
				if (!searching)
				{
					UI::ShiftCursorY(-framePaddingY + 1.0f);
					UI::ScopedColor text(ImGuiCol_Text, Colors::Theme::TextDarker);
					UI::ScopedStyle padding(ImGuiStyleVar_FramePadding, ImVec2(0.0f, framePaddingY));
					ImGui::TextUnformatted(*searchPlaceholder);
					UI::ShiftCursorY(-1.0f);
				}
			}

			ImGui::Spring();

			// Clear icon
			if (searching)
			{
				const float spacingX = 4.0f;
				const float lineHeight = ImGui::GetItemRectSize().y - framePaddingY / 2.0f;

				if (ImGui::InvisibleButton(GenerateID(), ImVec2{ lineHeight, lineHeight }))
				{
					if constexpr (std::is_same<StringType, HLString>::value)
						searchString.Clear();
					else
						memset(searchString, 0, BufferSize);

					modified = true;
				}

				if (ImGui::IsMouseHoveringRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()))
					ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);

				UI::DrawButtonImage(s_ClearIcon, IM_COL32(160, 160, 160, 200),
									IM_COL32(170, 170, 170, 255),
									IM_COL32(160, 160, 160, 150),
									UI::RectExpanded(UI::GetItemRect(), -2.0f, -2.0f));

				ImGui::Spring(-1.0f, spacingX * 2.0f);
			}

			ImGui::EndHorizontal();
			UI::ShiftCursorY(-1.0f);
			UI::PopID();
			return modified;
		}

		HLAPI static bool OptionsButton()
		{
			const bool clicked = ImGui::InvisibleButton("##options", ImVec2(ImGui::GetFrameHeight(), ImGui::GetFrameHeight()));

			const float spaceAvail = std::min(ImGui::GetItemRectSize().x, ImGui::GetItemRectSize().y);
			const float desiredIconSize = 15.0f;
			const float padding = std::max((spaceAvail - desiredIconSize) / 2.0f, 0.0f);

			constexpr auto buttonColor = Colors::Theme::Text;
			const uint8 value = uint8(ImColor(buttonColor).Value.x * 255);
			UI::DrawButtonImage(s_GearIcon, IM_COL32(value, value, value, 200), IM_COL32(value, value, value, 255), IM_COL32(value, value, value, 150), UI::RectExpanded(UI::GetItemRect(), -padding, -padding));

			return clicked;
		}

		HLAPI static bool AssetSearchPopup(const HLString &id, AssetType type, AssetHandle &selected, bool *cleared = nullptr, const HLString &hint = "Search Assets", const ImVec2 &size = { 250.0f, 350.0f });
		HLAPI static bool AssetSearchPopup(const HLString &id, AssetHandle &selected, bool *cleared = nullptr, const HLString &hint = "Search Assets", const ImVec2 &size = { 250.0f, 350.0f }, std::initializer_list<AssetType> assetTypes = {});

	private:

		static Ref<Texture2D> s_SearchIcon;
		static Ref<Texture2D> s_ClearIcon;
		static Ref<Texture2D> s_GearIcon;
	};
}

