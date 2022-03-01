// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "SettingsPanel.h"

SettingsPanel::SettingsPanel()
{
}

SettingsPanel::~SettingsPanel()
{
}

void SettingsPanel::OnUIRender(bool *pOpen)
{
	if (!(*pOpen))
		return;

	ImGui::Begin("Settings", pOpen, ImGuiWindowFlags_NoResize);

	ImVec2 textSize = ImGui::CalcTextSize("Restore default settings");
	float buttonWidth = textSize.x + 16.0f;
	float buttonHeight = (textSize.y * 2.0f) + 2.0f;

	if (ImGui::BeginTabBar("##settingsPanel"))
	{
		if (ImGui::BeginTabItem("General"))
		{
			ImGui::SameLine(ImGui::GetContentRegionAvail().x - buttonWidth);
			if (UI::DrawButton("Restore default settings", { buttonWidth, buttonHeight }))
			{
				ImGui::OpenPopup("RestoreDefaultSettings");
			}

			if (ShowYesNoDialogue())
			{
			}

			HLString name = "Untitled Scene";
			UI::DrawInputText("Scene Name", name);

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Input"))
		{
			ImGui::SameLine(ImGui::GetContentRegionAvail().x - buttonWidth);
			if (UI::DrawButton("Restore default settings", { buttonWidth, buttonHeight }))
			{
				ImGui::OpenPopup("RestoreDefaultSettings");
			}

			if (ShowYesNoDialogue())
			{
			}

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Asset Browser"))
		{
			AssetBrowserSettings &settings = AssetBrowserPanel::GetSettings();

			ImGui::SameLine(ImGui::GetContentRegionAvail().x - buttonWidth);
			if (UI::DrawButton("Restore default settings", { buttonWidth, buttonHeight }))
			{
				ImGui::OpenPopup("RestoreDefaultSettings");
			}

			if (ShowYesNoDialogue())
			{
				AssetBrowserPanel::RestoreDefaultSettings();
			}

			UI::DrawFloatSlider("Thumbnail Size", settings.ThumbnailSize, 1.0f, 512.0f);
			UI::DrawCheckbox("Show Asset Type", settings.ShowAssetType, UI::LabelAlignment::Right);

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	UI::DrawButton("Save Settings");

	ImGui::End();
}

bool SettingsPanel::ShowYesNoDialogue()
{
	bool result = false;

	if (ImGui::BeginPopupModal("RestoreDefaultSettings", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Are you sure you want to restore the default settings? This can not be undone!");
		float columnWidth = ImGui::GetContentRegionAvailWidth() / 4;

		ImGui::Columns(4, 0, false);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::SetColumnWidth(1, columnWidth);
		ImGui::SetColumnWidth(2, columnWidth);
		ImGui::SetColumnWidth(3, columnWidth);
		ImGui::NextColumn();

		if (ImGui::Button("Yes", ImVec2(columnWidth, 0)))
		{
			result = true;
			ImGui::CloseCurrentPopup();
		}

		ImGui::NextColumn();
		ImGui::SetItemDefaultFocus();

		if (ImGui::Button("No", ImVec2(columnWidth, 0)))
		{
			result = false;
			ImGui::CloseCurrentPopup();
		}

		ImGui::NextColumn();
		ImGui::EndPopup();
	}

	return result;
}

