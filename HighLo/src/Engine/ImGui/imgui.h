// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

// defined in Windows.h
#undef DrawText

#ifndef IMGUI_DEFINE_MATH_OPERATORS
	#define IMGUI_DEFINE_MATH_OPERATORS
#endif // IMGUI_DEFINE_MATH_OPERATORS

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "Engine/ImGui/ImGui/imgui.h"
#include "Engine/ImGui/ImGui/imgui_internal.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Engine/ECS/Entity.h"
#include "Engine/Renderer/Texture.h"

#include "Engine/Window/Window.h"
#include "Engine/Window/PopupMenu.h"

namespace highlo::UI
{
	enum class ImGuiWindowStyle
	{
		None = 0,
		Dark,
		Light
	};

	HLAPI void InitImGui(Window *window, ImGuiWindowStyle windowStyle);
	HLAPI void ShutdownImGui();

	HLAPI void BeginScene();
	HLAPI void EndScene();

	HLAPI void BeginWindow(const HLString &title, bool pOpen = false, bool fullscreen = false);
	HLAPI void EndWindow();

	HLAPI void BeginViewport(const HLString &title, bool *pOpen = nullptr, const Ref<PopupMenu> &popupMenu = nullptr);
	HLAPI void EndViewport();

	HLAPI void DisplayDebugInformation();
	HLAPI void SetDarkThemeColors();
	HLAPI void SetLightThemeColors();

	HLAPI bool IsMouseEnabed();
	HLAPI void SetMouseEnabled(const bool value);

	HLAPI void DrawMenu(const Ref<MenuBar> &menuBar);
	HLAPI void DrawPopupMenu(const Ref<PopupMenu> &menu);
	HLAPI void DrawHelpMarker(const HLString &description);

	HLAPI void PushID();
	HLAPI void PopID();

	HLAPI void Separator();

	HLAPI void PushItemDisabled();
	HLAPI void PopItemDisabled();
	HLAPI bool IsItemDisabled();

	HLAPI void BeginPropertyGrid(int32 colNum = 1);
	HLAPI void EndPropertyGrid();

	HLAPI bool BeginTreeNode(const HLString &name, bool defaultOpen = true);
	HLAPI void EndTreeNode();

	HLAPI void BeginCheckboxGroup(const HLString &label);
	HLAPI void EndCheckboxGroup();

	HLAPI bool IsWindowHovered();
	HLAPI bool IsWindowFocused();
	HLAPI glm::vec2 GetCursorPosition();
	HLAPI glm::vec2 GetMousePosition();
	HLAPI glm::vec2 GetContentRegion();
	HLAPI glm::vec2 GetWindowSize();
	HLAPI glm::vec2 GetWindowPos();
	HLAPI std::pair<glm::vec2, glm::vec2> GetWindowRect();
	HLAPI bool IsMouseHoveringRect(const glm::vec2 &min, const glm::vec2 &max);
	HLAPI bool IsMouseOverGizmo();
	HLAPI bool IsUsingGizmo();
	HLAPI bool IsItemHovered();

	HLAPI void EnableMenuBar();
	HLAPI void DisableMenuBar();
	HLAPI ImGuiWindowStyle GetCurrentWindowStyle();

	HLAPI void DrawText(const HLString &text);
	HLAPI void DrawInputText(const HLString &label, const HLString &value);
	HLAPI bool DrawInputText(const HLString &label, HLString &value);
	HLAPI bool DrawInputTextWithLength(const HLString &label, HLString &value, size_t length);

	HLAPI bool DrawCheckbox(const HLString &label, bool &value);
	HLAPI bool DrawCheckboxGroup(const HLString &label, bool &value);
	HLAPI bool DrawColorPicker(const HLString &label, glm::vec3 &value);
	HLAPI bool DrawButton(const HLString &label, const ImVec2 &size = ImVec2(0, 0));

	HLAPI bool DrawDragInt(const HLString &label, int32 &value);
	HLAPI bool DrawDragFloat(const HLString &label, float &value, float delta = 0.1f, float min = 0.0f, float max = 0.0f);
	HLAPI bool DrawDragFloat2(const HLString &label, glm::vec2 &value, float delta = 0.1f);
	HLAPI bool DrawDragFloat3(const HLString &label, glm::vec3 &value, float delta = 0.1f);
	HLAPI bool DrawDragFloat4(const HLString &label, glm::vec4 &value, float delta = 0.1f);
	HLAPI bool DrawIntSlider(const HLString &label, int32 &value, int32 min, int32 max);
	HLAPI bool DrawFloatSlider(const HLString &label, float &value, float min, float max);
	HLAPI bool DrawVec2Slider(const HLString &label, glm::vec2 &value, float min, float max);
	HLAPI bool DrawVec3Slider(const HLString &label, glm::vec3 &value, float min, float max);
	HLAPI bool DrawVec4Slider(const HLString &label, glm::vec4 &value, float min, float max);

	HLAPI bool DrawVec2(const HLString &label, glm::vec2 &values, float resetValue = 0.0f, float columnWidth = 100.0f);
	HLAPI bool DrawVec3(const HLString &label, glm::vec3 &values, float resetValue = 0.0f, float columnWidth = 100.0f);
	HLAPI bool DrawVec4(const HLString &label, glm::vec4 &values, float resetValue = 0.0f, float columnWidth = 100.0f);

	template<typename TEnum, typename TType = int32>
	HLAPI bool DrawDropdown(const HLString &label, const char **options, int32 optionsCount, TEnum &selected)
	{
		TType selectedIndex = (TType)selected;
		const char *current = options[selectedIndex];
		bool changed = false;
		const HLString id = "##" + HLString(label);

		ImGui::Text(*label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		if (IsItemDisabled())
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);

		if (ImGui::BeginCombo(*id, current))
		{
			for (int32 i = 0; i < optionsCount; ++i)
			{
				const bool isSelected = (current == options[i]);
				if (ImGui::Selectable(options[i], isSelected))
				{
					current = options[i];
					selected = (TEnum)i;
					changed = true;
				}

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

		if (IsItemDisabled())
			ImGui::PopStyleVar();

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return changed;
	}

	HLAPI bool DrawDropdown(const HLString &label, const char **options, int32 optionsCount, int32 *selected);
	HLAPI bool DrawDropdown(const HLString &label, const std::vector<HLString> &options, int32 optionsCount, int32 *selected);

	HLAPI void Image(const Ref<Texture2D> &texture, const ImVec2 &size, const ImVec2 &uv0 = ImVec2(0, 0), const ImVec2 &uv1 = ImVec2(1, 1), const ImVec4 &tintColor = ImVec4(1, 1, 1, 1), const ImVec4 &borderColor = ImVec4(0, 0, 0, 0));
	HLAPI void Image(const Ref<Texture2D> &texture, uint32 layer, const ImVec2 &size, const ImVec2 &uv0 = ImVec2(0, 0), const ImVec2 &uv1 = ImVec2(1, 1), const ImVec4 &tintColor = ImVec4(1, 1, 1, 1), const ImVec4 &borderColor = ImVec4(0, 0, 0, 0));
	HLAPI void ImageMip(const Ref<Texture2D> &texture, uint32 mip, const ImVec2 &size, const ImVec2 &uv0 = ImVec2(0, 0), const ImVec2 &uv1 = ImVec2(1, 1), const ImVec4 &tintColor = ImVec4(1, 1, 1, 1), const ImVec4 &borderColor = ImVec4(0, 0, 0, 0));
	HLAPI bool ImageButton(const Ref<Texture2D> &texture, const ImVec2 &size, const ImVec2 &uv0 = ImVec2(0, 0), const ImVec2 &uv1 = ImVec2(1, 1), int32 framePadding = -1, const ImVec4 &bgColor = ImVec4(0, 0, 0, 0), const ImVec4 &tintColor = ImVec4(1, 1, 1, 1));
	HLAPI bool ImageButton(const HLString &id, const Ref<Texture2D> &texture, const ImVec2 &size, const ImVec2 &uv0 = ImVec2(0, 0), const ImVec2 &uv1 = ImVec2(1, 1), int32 framePadding = -1, const ImVec4 &bgColor = ImVec4(0, 0, 0, 0), const ImVec4 &tintColor = ImVec4(1, 1, 1, 1));
}

