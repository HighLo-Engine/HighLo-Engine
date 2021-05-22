#pragma once

#include <glm/glm.hpp>

#include "Engine/Core/DataTypes/HLString.h"
#include "Engine/Math/HLMath.h"
#include "Engine/Renderer/Framebuffer.h"
#include "Engine/Window/MenuBar.h"

struct ImGuiTextBuffer;

namespace highlo
{
	class Window;

	enum class PropertyFlag
	{
		None = 0,
		Color,
		Drag,
		Slider
	};

	enum class ImGuiWindowStyle
	{
		None = 0,
		Dark,
		Light
	};

	class ImGuiRenderer
	{
	public:
		HLAPI static void Init(Window* window, ImGuiWindowStyle windowStyle = ImGuiWindowStyle::Dark);
		HLAPI static void Shutdown();

		HLAPI static void StartScene();
		HLAPI static void EndScene();

		HLAPI static void ShowDemoWindow();
		HLAPI static void ShouldDisplayDebugInformation(bool show);
		HLAPI static void ShouldUseCustomConsole(bool use);

		HLAPI static void StartWindow(const HLString &title, bool pOpen = false, bool fullscreen = false);
		HLAPI static void EndWindow();

		HLAPI static void StartViewport(const HLString &title);
		HLAPI static void EndViewport();

		HLAPI static void StartChild(const HLString &id, uint32 width = 0, uint32 height = 0);
		HLAPI static void EndChild();

		HLAPI static bool StartTreeNode(const HLString &name, bool defaultOpen = true);
		HLAPI static void EndTreeNode();

		HLAPI static void StartCheckboxGroup(const HLString &label);
		HLAPI static void EndCheckboxGroup();

		HLAPI static void StartPopupModal(const HLString &text);
		HLAPI static void EndPopupModal();

		HLAPI static void NewLine();
		HLAPI static void Separator();
		HLAPI static void SameLine();
		HLAPI static void Text(const HLString& text);
		HLAPI static void SliderInt(const HLString& text, int32 &val, int32 min, int32 max);
		HLAPI static void SliderFloat(const HLString& text, float& val, float min, float max);
		HLAPI static bool Button(const HLString& text, float width = 0, float height = 0);
		HLAPI static bool Combobox(const HLString& text, std::vector<HLString> &items, int32 &selected_index);
		HLAPI static bool Checkbox(const HLString& text, bool &checked);
		HLAPI static void InputText(const HLString& label, const HLString &text);
		HLAPI static void ColorPicker(const HLString& label, glm::vec3 &color);
		HLAPI static void ColorPicker(const HLString& label, glm::vec4 &color);

		HLAPI static bool Property(const HLString &name, bool &value);
		HLAPI static bool Property(const HLString &name, float &value, float min = -1.0f, float max = 1.0f, PropertyFlag flags = PropertyFlag::Slider);
		HLAPI static bool Property(const HLString &name, glm::vec2 &value, PropertyFlag flags = PropertyFlag::Slider);
		HLAPI static bool Property(const HLString &name, glm::vec2 &value, float min = -1.0f, float max = 1.0f, PropertyFlag flags = PropertyFlag::Slider);
		HLAPI static bool Property(const HLString &name, glm::vec3 &value, PropertyFlag flags = PropertyFlag::Slider);
		HLAPI static bool Property(const HLString &name, glm::vec3 &value, float min = -1.0f, float max = 1.0f, PropertyFlag flags = PropertyFlag::Slider);
		HLAPI static bool Property(const HLString &name, glm::vec4 &value, PropertyFlag flags = PropertyFlag::Slider);
		HLAPI static bool Property(const HLString &name, glm::vec4 &value, float min = -1.0f, float max = 1.0f, PropertyFlag flags = PropertyFlag::Slider);
		
		HLAPI static void DrawFramebuffer(const Ref<Framebuffer> &framebuffer, const glm::vec2 &size, const glm::vec2 &uv0, const glm::vec2 &uv1, const glm::vec4 &tintColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), const glm::vec4 &borderColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		HLAPI static void DrawMenu(const Ref<MenuBar> &menubar);

		HLAPI static bool IsWindowHovered();
		HLAPI static bool IsWindowFocused();
		HLAPI static bool CanDraw();

		HLAPI static glm::vec2 GetCursorPosition();
		HLAPI static glm::vec2 GetMousePosition();
		HLAPI static glm::vec2 GetContentRegion();
		HLAPI static glm::vec2 GetWindowSize();
		HLAPI static glm::vec2 GetWindowPos();
		HLAPI static bool IsMouseHoveringRect(const glm::vec2 &min, const glm::vec2 &max);

		HLAPI static void OnWindowResize(uint32 width, uint32 height);

	private:

		static bool s_ShouldDisplayDebugInformation;
		static bool s_ShouldUseCustomConsole;
		static bool s_CanDraw;
		static std::shared_ptr<ImGuiTextBuffer> s_ImGuiTextBuffer;

		static void DisplayDebugInformation();
		static void DisplayCustomConsole();
		static HLString GenerateID(const HLString &name);

		static void SetDarkThemeColors();
		static void SetLightThemeColors();
	};
}
