#pragma once
#include "Engine/Core/DataTypes/HLString.h"
#include "Engine/Math/HLMath.h"

struct ImGuiTextBuffer;

namespace highlo
{
	class Window;

	class ImGuiRenderer
	{
	public:
		HLAPI static void Init(Window* window);
		HLAPI static void Shutdown();

		HLAPI static void BeginScene();
		HLAPI static void EndScene();

		HLAPI static void ShowDemoWindow();
		HLAPI static void ShouldDisplayDebugInformation(bool show);
		HLAPI static void ShouldUseCustomConsole(bool use);

		HLAPI static void StartWindow(const HLString& title, uint32_t width = 0, uint32_t height = 0);
		HLAPI static void EndWindow();

		HLAPI static void BeginChild(const HLString& id, uint32_t width = 0, uint32_t height = 0);
		HLAPI static void EndChild();

		HLAPI static void NewLine();
		HLAPI static void Separator();
		HLAPI static void SameLine();
		HLAPI static void Text(const HLString& text);
		HLAPI static void SliderInt(const HLString& text, int& val, int min, int max);
		HLAPI static void SliderFloat(const HLString& text, float& val, float min, float max);
		HLAPI static bool Button(const HLString& text, float width = 0, float height = 0);
		HLAPI static bool Combobox(const HLString& text, std::vector<HLString>& items, int& selected_index);
		HLAPI static bool Checkbox(const HLString& text, bool& checked);
		HLAPI static void InputText(const HLString& label, char* text, size_t buffersize);
		HLAPI static void ColorPicker(const HLString& label, glm::vec3& color);
		HLAPI static void ColorPicker(const HLString& label, glm::vec4& color);

		HLAPI static void AddToTextBuffer(const HLString& text);

	private:
		static bool s_ShouldDisplayDebugInformation;
		static bool s_ShouldUseCustomConsole;
		static std::shared_ptr<ImGuiTextBuffer> s_ImGuiTextBuffer;

		static void DisplayDebugInformation();
		static void DisplayCustomConsole();
	};
}
