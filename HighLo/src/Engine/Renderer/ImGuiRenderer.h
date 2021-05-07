#pragma once

#include <glm/glm.hpp>

#include "Engine/Core/DataTypes/HLString.h"
#include "Engine/Math/HLMath.h"

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

		HLAPI static void BeginScene();
		HLAPI static void EndScene();

		HLAPI static void ShowDemoWindow();
		HLAPI static void ShouldDisplayDebugInformation(bool show);
		HLAPI static void ShouldUseCustomConsole(bool use);

		HLAPI static void StartWindow(const HLString &title, uint32 width = 0, uint32 height = 0);
		HLAPI static void EndWindow();

		HLAPI static void BeginChild(const HLString &id, uint32 width = 0, uint32 height = 0);
		HLAPI static void EndChild();

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
		HLAPI static bool Property(const HLString &name, glm::vec2 &value, PropertyFlag flags);
		HLAPI static bool Property(const HLString &name, glm::vec2 &value, float min = -1.0f, float max = 1.0f, PropertyFlag flags = PropertyFlag::Slider);
		HLAPI static bool Property(const HLString &name, glm::vec3 &value, PropertyFlag flags);
		HLAPI static bool Property(const HLString &name, glm::vec3 &value, float min = -1.0f, float max = 1.0f, PropertyFlag flags = PropertyFlag::Slider);
		HLAPI static bool Property(const HLString &name, glm::vec4 &value, PropertyFlag flags);
		HLAPI static bool Property(const HLString &name, glm::vec4 &value, float min = -1.0f, float max = 1.0f, PropertyFlag flags = PropertyFlag::Slider);
		
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
