// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "ImGuiWidgets.h"

namespace highlo::UI
{
	Ref<Texture2D> Widgets::s_SearchIcon;
	Ref<Texture2D> Widgets::s_ClearIcon;
	Ref<Texture2D> Widgets::s_GearIcon;

	void Widgets::Init()
	{
		s_SearchIcon = Texture2D::LoadFromFile("assets/Resources/icons/search.png");
		s_ClearIcon = Texture2D::LoadFromFile("assets/Resources/icons/close.png");
		s_GearIcon = Texture2D::LoadFromFile("assets/Resources/icons/settings.png");
	}
	
	void Widgets::Shutdown()
	{
		s_GearIcon.Reset();
		s_ClearIcon.Reset();
		s_SearchIcon.Reset();
	}
	
	bool Widgets::AssetSearchPopup(const HLString &id, AssetType type, AssetHandle &selected, bool *cleared, const HLString &hint, const ImVec2 &size)
	{
		UI::ScopedColor popupBg(ImGuiCol_PopupBg, UI::ColorWithMultipliedValue(Colors::Theme::Background, 1.6f));
		bool modified = false;
		static bool grabFocus = true;
		const auto &assetRegistry = AssetManager::Get()->GetAssetRegistry();
		AssetHandle current = selected;

		if (UI::IsItemDisabled())
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);

		ImGui::SetNextWindowSize({ size.x, 0.0f });

		if (UI::BeginPopup(*id, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
		{
			static HLString searchedString = "";

			if (ImGui::GetCurrentWindow()->Appearing)
			{
				grabFocus = true;
				searchedString.Clear();
			}

			UI::ShiftCursor(3.0f, 2.0f);
			ImGui::SetNextItemWidth(ImGui::GetWindowWidth() - ImGui::GetCursorPosX() * 2.0f);
			Widgets::SearchWidget(searchedString, *hint, &grabFocus);
			const bool searching = !searchedString.IsEmpty();

			if (cleared != nullptr)
			{
				UI::ScopedColorStack buttonColors(
					ImGuiCol_Button, UI::ColorWithMultipliedValue(Colors::Theme::Background, 1.0f),
					ImGuiCol_ButtonHovered, UI::ColorWithMultipliedValue(Colors::Theme::Background, 1.2f),
					ImGuiCol_ButtonActive, UI::ColorWithMultipliedValue(Colors::Theme::Background, 0.9f));
				UI::ScopedStyle border(ImGuiStyleVar_FrameBorderSize, 0.0f);

				ImGui::SetCursorPosX(0.0f);
				ImGui::PushItemFlag(ImGuiItemFlags_NoNav, searching);

				if (ImGui::Button("Clear", { ImGui::GetWindowWidth(), 0.0f }))
				{
					*cleared = true;
					modified = true;
				}

				ImGui::PopItemFlag();
			}

			// Draw list of assets
			{
				UI::ScopedColor listBoxBg(ImGuiCol_FrameBg, IM_COL32_DISABLE);
				UI::ScopedColor listBoxBorder(ImGuiCol_Border, IM_COL32_DISABLE);

				ImGuiID listId = ImGui::GetID("##searchListBox");
				if (ImGui::BeginListBox("##searchListBox", { -FLT_MIN, 0.0f }))
				{
					bool forwardFocus = false;

					ImGuiContext &g = *GImGui;
					if (g.NavJustMovedToId != 0)
					{
						if (g.NavJustMovedToId == listId)
						{
							forwardFocus = true;
							ImGui::ActivateItem(listId);
							ImGui::SetKeyboardFocusHere(1);
						}
					}

					for (auto it = assetRegistry.cbegin(); it != assetRegistry.cend(); ++it)
					{
						const auto &[path, metaData] = *it;
						if (metaData.Type != type)
							continue;

						const HLString &assetName = metaData.FilePath.Filename();
						if (!searchedString.IsEmpty() && !UI::IsMatchingSearch(assetName, searchedString))
							continue;

						bool isSelected = (current == metaData.Handle);
						if (ImGui::Selectable(*assetName, isSelected))
						{
							current = metaData.Handle;
							selected = metaData.Handle;
							modified = true;
						}

						if (forwardFocus)
							forwardFocus = false;
						else if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndListBox();
				}

			}

			if (modified)
				ImGui::CloseCurrentPopup();

			UI::EndPopup();
		}

		if (UI::IsItemDisabled())
			ImGui::PopStyleVar();

		return modified;
	}

	bool Widgets::AssetSearchPopup(const HLString &id, AssetHandle &selected, bool *cleared, const HLString &hint, const ImVec2 &size, std::initializer_list<AssetType> assetTypes)
	{
		UI::ScopedColor popupBG(ImGuiCol_PopupBg, UI::ColorWithMultipliedValue(Colors::Theme::Background, 1.6f));
		bool modified = false;
		const auto &assetRegistry = AssetManager::Get()->GetAssetRegistry();
		AssetHandle current = selected;

		if (UI::IsItemDisabled())
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);

		ImGui::SetNextWindowSize({ size.x, 0.0f });

		static bool grabFocus = true;

		if (UI::BeginPopup(*id, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
		{
			static HLString searchedString = "";

			if (ImGui::GetCurrentWindow()->Appearing)
			{
				grabFocus = true;
				searchedString.Clear();
			}

			// Search widget
			UI::ShiftCursor(3.0f, 2.0f);
			ImGui::SetNextItemWidth(ImGui::GetWindowWidth() - ImGui::GetCursorPosX() * 2.0f);
			Widgets::SearchWidget(searchedString, *hint, &grabFocus);
			const bool searching = !searchedString.IsEmpty();

			// Clear property button
			if (cleared != nullptr)
			{
				UI::ScopedColorStack buttonColours(
					ImGuiCol_Button, UI::ColorWithMultipliedValue(Colors::Theme::Background, 1.0f),
					ImGuiCol_ButtonHovered, UI::ColorWithMultipliedValue(Colors::Theme::Background, 1.2f),
					ImGuiCol_ButtonActive, UI::ColorWithMultipliedValue(Colors::Theme::Background, 0.9f));
				UI::ScopedStyle border(ImGuiStyleVar_FrameBorderSize, 0.0f);

				ImGui::SetCursorPosX(0);
				ImGui::PushItemFlag(ImGuiItemFlags_NoNav, searching);

				if (ImGui::Button("Clear", { ImGui::GetWindowWidth(), 0.0f }))
				{
					*cleared = true;
					modified = true;
				}

				ImGui::PopItemFlag();
			}

			// Draw List of assets
			{
				UI::ScopedColor listBoxBg(ImGuiCol_FrameBg, IM_COL32_DISABLE);
				UI::ScopedColor listBoxBorder(ImGuiCol_Border, IM_COL32_DISABLE);

				ImGuiID listID = ImGui::GetID("##SearchListBox");
				if (ImGui::BeginListBox("##SearchListBox", ImVec2(-FLT_MIN, 0.0f)))
				{
					bool forwardFocus = false;

					ImGuiContext &g = *GImGui;
					if (g.NavJustMovedToId != 0)
					{
						if (g.NavJustMovedToId == listID)
						{
							forwardFocus = true;
							// ActivateItem moves keyboard navigation focuse inside of the window
							ImGui::ActivateItem(listID);
							ImGui::SetKeyboardFocusHere(1);
						}
					}

					for (auto it = assetRegistry.cbegin(); it != assetRegistry.cend(); it++)
					{
						const auto &[path, metadata] = *it;
						bool isValidType = false;

						for (AssetType type : assetTypes)
						{
							if (metadata.Type == type)
							{
								isValidType = true;
								break;
							}
						}

						if (!isValidType)
							continue;

						const std::string assetName = metadata.FilePath.Filename();

						if (!searchedString.IsEmpty() && !UI::IsMatchingSearch(assetName, searchedString))
							continue;

						bool isSelected = (current == metadata.Handle);
						if (ImGui::Selectable(assetName.c_str(), isSelected))
						{
							current = metadata.Handle;
							selected = metadata.Handle;
							modified = true;
						}

						{
							const HLString &assetType = utils::AssetTypeToString(metadata.Type);
							ImVec2 textSize = ImGui::CalcTextSize(*assetType);
							ImVec2 rectSize = ImGui::GetItemRectSize();
							float paddingX = ImGui::GetStyle().FramePadding.x;

							ImGui::SameLine(rectSize.x - textSize.x - paddingX);

							UI::ScopedColor textColour(ImGuiCol_Text, Colors::Theme::TextDarker);
							ImGui::TextUnformatted(*assetType.ToUpperCase());
						}

						if (forwardFocus)
							forwardFocus = false;
						else if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndListBox();
				}
			}
			if (modified)
				ImGui::CloseCurrentPopup();

			UI::EndPopup();
		}

		if (UI::IsItemDisabled())
			ImGui::PopStyleVar();

		return modified;
	}
}

