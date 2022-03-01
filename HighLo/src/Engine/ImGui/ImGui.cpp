// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "ImGui.h"

#include "Engine/Application/Application.h"
#include "ImGuizmo.h"

#include "ImGuiRenderer.h"

#ifdef HIGHLO_API_GLFW
	#include <GLFW/glfw3.h>
	#include "Engine/ImGui/ImGui/backends/imgui_impl_glfw.h"
	#define IMGUI_WINDOW_IMPL_FN_NAME(fn) ImGui_ImplGlfw_##fn
#else
	#include "Engine/ImGui/ImGui/backends/imgui_impl_win32.h"
	#define IMGUI_WINDOW_IMPL_FN_NAME(fn) ImGui_ImplWin32_##fn
#endif // HIGHLO_API_GLFW

#ifdef HIGHLO_API_VULKAN
#include "Engine/Platform/Vulkan/VulkanTexture2D.h"
#include "Engine/ImGui/ImGui/backends/imgui_impl_vulkan_with_textures.h"
#endif // HIGHLO_API_VULKAN

#include "ImGuiWidgets.h"

namespace highlo::UI
{
	namespace utils
	{
		static void DrawSubPopupMenuInternal(Ref<PopupMenu> &parentMenu, const HLString &name, const std::vector<PopupMenuItem> &items)
		{
			// TODO
		}

		static void DrawSubMenuInternal(Ref<FileMenu> &parentMenu, const HLString &menuName, const std::vector<Ref<MenuItem>> &items)
		{
		if (ImGui::BeginMenu(*menuName))
		{
			for (uint32 i = 0; i < items.size(); ++i)
			{
				Ref<MenuItem> currentItem = items[i];
				if (currentItem->IsSubmenu)
					DrawSubMenuInternal(parentMenu, currentItem->Name, currentItem->SubmenuItems);

				if (currentItem->Separator)
				{
					ImGui::Separator();
					continue;
				}

				if (ImGui::MenuItem(*currentItem->Name, *currentItem->Shortcut, currentItem->IsSelected, currentItem->Visible))
				{
					Ref<MenuItem> before = Ref<MenuItem>::Create(currentItem->Name, currentItem->ID, currentItem->Callback, currentItem->Visible, currentItem->Separator);
					before->IsSelected = currentItem->IsSelected;
					before->IsSubmenu = currentItem->IsSubmenu;
					before->SubmenuItems = currentItem->SubmenuItems;
					before->Shortcut = currentItem->Shortcut;
					currentItem->Callback(parentMenu.Get(), currentItem.Get());
					if (!before.Equals(currentItem))
					{
						FileMenuChangedEvent e(currentItem);
						HLApplication::Get().GetWindow().GetEventCallback()(e);
					}
				}
			}

			ImGui::EndMenu();
			}
		}

		static void DrawMenuInternal(Ref<FileMenu> &fileMenu)
		{
			if (ImGui::BeginMenu(*fileMenu->GetName()))
			{
				std::vector<Ref<MenuItem>> items = fileMenu->GetMenuItems();
				for (uint32 i = 0; i < items.size(); ++i)
				{
					Ref<MenuItem> currentItem = items[i];
					if (currentItem->IsSubmenu)
						DrawSubMenuInternal(fileMenu, currentItem->Name, currentItem->SubmenuItems);

					if (currentItem->Separator)
					{
						ImGui::Separator();
						continue;
					}

					if (ImGui::MenuItem(*currentItem->Name, *currentItem->Shortcut, currentItem->IsSelected, currentItem->Visible))
					{
						Ref<MenuItem> before = Ref<MenuItem>::Create(currentItem->Name, currentItem->ID, currentItem->Callback, currentItem->Visible, currentItem->Separator);
						before->IsSelected = currentItem->IsSelected;
						before->IsSubmenu = currentItem->IsSubmenu;
						before->SubmenuItems = currentItem->SubmenuItems;
						before->Shortcut = currentItem->Shortcut;
						currentItem->Callback(fileMenu.Get(), items[i].Get());
						if (!before.Equals(items[i].Get()))
						{
							FileMenuChangedEvent e(currentItem);
							HLApplication::Get().GetWindow().GetEventCallback()(e);
						}
					}
				}

				ImGui::EndMenu();
			}
		}

		static void DrawPopupMenuInternal(const HLString &name, const std::vector<PopupMenuItem> &items)
		{
			// TODO
		}
	}

	static int32 s_ImGuiUIContextID = 0;
	static uint32 s_ImGuiIDCounter = 0;
	static char s_ImGuiIDBuffer[16];
	static int32 s_ImGuiCheckboxCount = 0;

	static bool s_ShouldDisplayDebugInformation = false;
	static bool s_UseMenuBar = false;
	static ImGuiWindowStyle s_ImGuiWindowStyle = ImGuiWindowStyle::None;
	static ImGuiContext *s_ImGuiContext = nullptr;
	static const ImWchar s_FontAwesomeIconsRanges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };

	static Ref<ImGuiRenderer> s_ImGuiRenderer = ImGuiRenderer::Create();

	void InitImGui(Window *window, ImGuiWindowStyle windowStyle)
	{
		IMGUI_CHECKVERSION();
		s_ImGuiContext = ImGui::CreateContext();
		s_ImGuiWindowStyle = windowStyle;

		// Globals are not shared accross DLL, so we need to call ImGui::SetCurrentContext
		ImGui::SetCurrentContext(s_ImGuiContext);

		ImGuiIO &io = ImGui::GetIO();
		io.IniFilename = "editorconfig.ini"; // rename imgui.ini to editorconfig to hide the usage from imgui
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Enable docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;     // Enable Multi-Viewport / Platform Windows
		io.ConfigViewportsNoAutoMerge = true;
		io.ConfigViewportsNoTaskBarIcon = true;

		io.Fonts->AddFontFromFileTTF("assets/fonts/BarlowSemiCondensedFontFamily/BarlowSemiCondensed-Regular.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesDefault());

		// Add FontAwesome 5 Font
		ImFontConfig iconsConfig;
		iconsConfig.MergeMode = true;
		iconsConfig.PixelSnapH = true;
		io.Fonts->AddFontFromFileTTF("assets/fonts/FontAwesome/fa-solid-900.ttf", 16.0f, &iconsConfig, s_FontAwesomeIconsRanges);
		
		// Combine both fonts into one font
		io.Fonts->Build();

		// Internal method used to override ImGui's theme colors with our own
		if (windowStyle == ImGuiWindowStyle::Dark)
		{
			ImGui::StyleColorsDark();
			SetDarkThemeColors();
		}
		else
		{
			ImGui::StyleColorsClassic();
			SetLightThemeColors();
		}

		ImGuiStyle &style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}
		else
		{
			style.Colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.15f, 0.15f, style.Colors[ImGuiCol_WindowBg].w);
		}

		s_ImGuiRenderer->Init(window);
		Widgets::Init();
	}

	void ShutdownImGui()
	{
		Widgets::Shutdown();
		s_ImGuiRenderer->Shutdown();
		ImGui::DestroyContext();
	}

	void BeginScene()
	{
		s_ImGuiRenderer->NewFrame();

		ImGui::NewFrame();
		ImGuizmo::BeginFrame();

		if (s_ShouldDisplayDebugInformation)
			DisplayDebugInformation();

	//	ImGui::ShowDemoWindow();
	}

	void EndScene()
	{
		ImGuiIO &io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)HLApplication::Get().GetWindow().GetWidth(), (float)HLApplication::Get().GetWindow().GetHeight());

		s_ImGuiRenderer->Render();
		s_ImGuiRenderer->RenderDrawData();

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
		#ifdef HIGHLO_API_GLFW
			GLFWwindow *backupContext = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backupContext);
		#else
			HGLRC backupContext = wglGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			wglMakeCurrent(wglGetCurrentDC(), backupContext);
		#endif // HIGHLO_API_GLFW
			}

		ImGui::EndFrame();
	}

	void BeginWindow(const HLString &title, bool pOpen, bool fullscreen)
	{
		static ImGuiDockNodeFlags optFlags = ImGuiDockNodeFlags_None;
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking;

	#ifdef HIGHLO_API_GLFW
		if (s_UseMenuBar)
			windowFlags |= ImGuiWindowFlags_MenuBar;
	#endif // HIGHLO_API_GLFW

		if (fullscreen)
		{
			ImGuiViewport *viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin(*title, &pOpen, windowFlags);
		ImGui::PopStyleVar();

		if (fullscreen)
			ImGui::PopStyleVar(2);

		// Dockspace
		ImGuiIO &io = ImGui::GetIO();
		ImGuiStyle &style = ImGui::GetStyle();

		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspaceID = ImGui::GetID("DockSpace");
			ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), optFlags);
		}

		style.WindowMinSize.x = minWinSizeX;

	#ifdef HIGHLO_API_GLFW
		if (HLApplication::Get().GetWindow().HasMenuBar())
			DrawMenu(HLApplication::Get().GetWindow().GetMenuBar());
	#endif // HIGHLO_API_GLFW
	}

	void EndWindow()
	{
		ImGui::End();
	}

	void BeginViewport(const HLString &title, bool *pOpen, const Ref<PopupMenu> &popupMenu)
	{
		ImGui::Begin(*title, pOpen);

		// Check if the mouse is in the area of the viewport
		if (popupMenu)
		{
			if (ImGui::IsWindowHovered())
			{
			#ifdef HIGHLO_API_GLFW
				DrawPopupMenu(popupMenu);
			#else
				popupMenu->Update();
			#endif // HIGHLO_API_GLFW
			}
		}
	}

	void EndViewport()
	{
		ImGui::End();
	}

	bool BeginPopup(const char *id, ImGuiWindowFlags flags)
	{
		bool opened = false;

		if (ImGui::BeginPopup(id, flags))
		{
			opened = true;

			const float padding = ImGui::GetStyle().WindowBorderSize;
			const ImRect windowRect = UI::RectExpanded(ImGui::GetCurrentWindow()->Rect(), -padding, -padding);
			ImGui::PushClipRect(windowRect.Min, windowRect.Max, false);
			const ImColor color1 = ImGui::GetStyleColorVec4(ImGuiCol_PopupBg);
			const ImColor color2 = UI::ColorWithMultipliedValue(color1, 0.8f);
			ImGui::GetWindowDrawList()->AddRectFilledMultiColor(windowRect.Min, windowRect.Max, color1, color1, color2, color2);
			ImGui::GetWindowDrawList()->AddRect(windowRect.Min, windowRect.Max, UI::ColorWithMultipliedValue(color1, 1.1f));
			ImGui::PopClipRect();

			// Popped in EndPopup()
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, IM_COL32(0, 0, 0, 80));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1.0, 1.0f));
		}

		return opened;
	}

	void EndPopup()
	{
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();
		ImGui::EndPopup();
	}

	uint32 *GetCurrentImGuiIDCounter()
	{
		return &s_ImGuiIDCounter;
	}

	void DisplayDebugInformation()
	{
		ImGui::Begin("HighLo Demo Window");

		static float f = 0.0f;
		static float col = 0.0f;
		int32 show = 1;
		ImGui::Text("Hello, world!");
		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
		ImGui::ColorEdit3("clear color", (float *) &col);
		if (ImGui::Button("Test Window")) show ^= 1;
		if (ImGui::Button("Another Window")) show ^= 1;
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImVec2 v = ImGui::GetWindowSize();
		ImGui::Text("%f %f", v.x, v.y);
		ImGui::End();
	}

	void SetDarkThemeColors()
	{
		auto &colors = ImGui::GetStyle().Colors;
		ImGuiStyle *style = &ImGui::GetStyle();
		s_ImGuiWindowStyle = ImGuiWindowStyle::Dark;

		float hspacing = 8.0f;
		float vspacing = 6.0f;
		style->DisplaySafeAreaPadding = ImVec2(0.0f, 0.0f);
		style->WindowPadding = ImVec2(hspacing / 2.0f, vspacing);
		style->FramePadding = ImVec2(hspacing, vspacing);
		style->ItemSpacing = ImVec2(hspacing, vspacing);
		style->ItemInnerSpacing = ImVec2(hspacing, vspacing);
		style->IndentSpacing = 20.0f;

		style->WindowRounding = 0.0f;
		style->FrameRounding = 0.0f;

		style->WindowBorderSize = 0.0f;
		style->FrameBorderSize = 1.0f;
		style->PopupBorderSize = 1.0f;

		style->ScrollbarSize = 20.0f;
		style->ScrollbarRounding = 0.0f;
		style->GrabMinSize = 5.0f;
		style->GrabRounding = 0.0f;

		colors[ImGuiCol_Text] = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.500f, 0.500f, 0.500f, 1.000f);

		colors[ImGuiCol_WindowBg] = ImVec4(0.180f, 0.180f, 0.180f, 1.000f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.280f, 0.280f, 0.280f, 0.000f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);

		colors[ImGuiCol_Border] = ImVec4(0.266f, 0.266f, 0.266f, 1.000f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.000f, 0.000f, 0.000f, 0.000f);

		colors[ImGuiCol_FrameBg] = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.200f, 0.200f, 0.200f, 1.000f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.280f, 0.280f, 0.280f, 1.000f);

		colors[ImGuiCol_TitleBg] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);

		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.277f, 0.277f, 0.277f, 1.000f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.300f, 0.300f, 0.300f, 1.000f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);

		colors[ImGuiCol_CheckMark] = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);

		colors[ImGuiCol_Button] = ImVec4(1.000f, 1.000f, 1.000f, 0.000f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
		colors[ImGuiCol_ButtonActive] = ImVec4(1.000f, 1.000f, 1.000f, 0.391f);

		colors[ImGuiCol_Header] = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);

		colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);

		colors[ImGuiCol_ResizeGrip] = ImVec4(1.000f, 1.000f, 1.000f, 0.250f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.000f, 1.000f, 1.000f, 0.670f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);

		colors[ImGuiCol_Tab] = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.352f, 0.352f, 0.352f, 1.000f);
		colors[ImGuiCol_TabActive] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);

		colors[ImGuiCol_DockingPreview] = ImVec4(1.000f, 0.391f, 0.000f, 0.781f);
		colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.180f, 0.180f, 0.180f, 1.000f);

		colors[ImGuiCol_PlotLines] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.586f, 0.586f, 0.586f, 1.000f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);

		colors[ImGuiCol_NavHighlight] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	}

	void SetLightThemeColors()
	{
		auto &colors = ImGui::GetStyle().Colors;
		ImGuiStyle *style = &ImGui::GetStyle();
		s_ImGuiWindowStyle = ImGuiWindowStyle::Light;

		float hspacing = 8.0f;
		float vspacing = 6.0f;
		style->DisplaySafeAreaPadding = ImVec2(0.0f, 0.0f);
		style->WindowPadding = ImVec2(hspacing / 2.0f, vspacing);
		style->FramePadding = ImVec2(hspacing, vspacing);
		style->ItemSpacing = ImVec2(hspacing, vspacing);
		style->ItemInnerSpacing = ImVec2(hspacing, vspacing);
		style->IndentSpacing = 20.0f;

		style->WindowRounding = 0.0f;
		style->FrameRounding = 0.0f;

		style->WindowBorderSize = 0.0f;
		style->FrameBorderSize = 1.0f;
		style->PopupBorderSize = 1.0f;

		style->ScrollbarSize = 20.0f;
		style->ScrollbarRounding = 0.0f;
		style->GrabMinSize = 5.0f;
		style->GrabRounding = 0.0f;

		ImVec4 white = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		ImVec4 transparent = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		ImVec4 dark = ImVec4(0.00f, 0.00f, 0.00f, 0.20f);
		ImVec4 darker = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);

		ImVec4 background = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
		ImVec4 tabBackground = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
		ImVec4 text = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		ImVec4 textHover = ImVec4(0.80f, 0.89f, 0.97f, 1.00f);
		ImVec4 border = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		ImVec4 grab = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);
		ImVec4 header = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
		ImVec4 active = ImVec4(0.00f, 0.47f, 0.84f, 1.00f);
		ImVec4 hover = ImVec4(0.00f, 0.47f, 0.84f, 0.20f);
		ImVec4 nav = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);

		colors[ImGuiCol_Text] = text;
		colors[ImGuiCol_TextDisabled] = ImVec4(0.500f, 0.500f, 0.500f, 1.000f);

		colors[ImGuiCol_WindowBg] = background;
		colors[ImGuiCol_ChildBg] = background;
		colors[ImGuiCol_PopupBg] = background;
		colors[ImGuiCol_MenuBarBg] = background;
		colors[ImGuiCol_TextSelectedBg] = background;
		colors[ImGuiCol_NavWindowingDimBg] = background;
		colors[ImGuiCol_ModalWindowDimBg] = background;

		colors[ImGuiCol_Border] = ImVec4(0.266f, 0.266f, 0.266f, 1.000f);
		colors[ImGuiCol_BorderShadow] = transparent;

		colors[ImGuiCol_FrameBg] = background;
		colors[ImGuiCol_FrameBgHovered] = hover;
		colors[ImGuiCol_FrameBgActive] = active;

		colors[ImGuiCol_TitleBg] = background;
		colors[ImGuiCol_TitleBgActive] = tabBackground;
		colors[ImGuiCol_TitleBgCollapsed] = background;

		colors[ImGuiCol_ScrollbarBg] = background;
		colors[ImGuiCol_ScrollbarGrab] = grab;
		colors[ImGuiCol_ScrollbarGrabHovered] = hover;
		colors[ImGuiCol_ScrollbarGrabActive] = active;

		colors[ImGuiCol_CheckMark] = white;
		colors[ImGuiCol_SliderGrab] = grab;
		colors[ImGuiCol_SliderGrabActive] = active;
		colors[ImGuiCol_DragDropTarget] = nav;

		colors[ImGuiCol_Button] = ImVec4(1.000f, 1.000f, 1.000f, 0.000f);
		colors[ImGuiCol_ButtonHovered] = hover;
		colors[ImGuiCol_ButtonActive] = active;

		colors[ImGuiCol_Header] = background;
		colors[ImGuiCol_HeaderHovered] = hover;
		colors[ImGuiCol_HeaderActive] = active;

		colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
		colors[ImGuiCol_SeparatorHovered] = hover;
		colors[ImGuiCol_SeparatorActive] = active;

		colors[ImGuiCol_ResizeGrip] = ImVec4(1.000f, 1.000f, 1.000f, 0.250f);
		colors[ImGuiCol_ResizeGripHovered] = hover;
		colors[ImGuiCol_ResizeGripActive] = active;

		colors[ImGuiCol_Tab] = tabBackground;
		colors[ImGuiCol_TabHovered] = hover;
		colors[ImGuiCol_TabActive] = background;
		colors[ImGuiCol_TabUnfocused] = tabBackground;
		colors[ImGuiCol_TabUnfocusedActive] = background;

		colors[ImGuiCol_DockingPreview] = ImVec4(1.000f, 0.391f, 0.000f, 0.781f);
		colors[ImGuiCol_DockingEmptyBg] = background;

		colors[ImGuiCol_PlotLines] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
		colors[ImGuiCol_PlotLinesHovered] = hover;
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.586f, 0.586f, 0.586f, 1.000f);
		colors[ImGuiCol_PlotHistogramHovered] = hover;

		colors[ImGuiCol_NavHighlight] = nav;
		colors[ImGuiCol_NavWindowingHighlight] = nav;
	}

	bool IsMouseEnabed()
	{
		return ImGui::GetIO().ConfigFlags & ~ImGuiConfigFlags_NoMouse;
	}

	void SetMouseEnabled(const bool value)
	{
		if (value)
			ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
		else
			ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
	}

	void DrawMenu(const Ref<MenuBar> &menuBar)
	{
		std::vector<Ref<FileMenu>> fileMenus = menuBar->GetMenus();
		if (ImGui::BeginMainMenuBar())
		{
			for (int32 i = 0; i < fileMenus.size(); ++i)
				utils::DrawMenuInternal(fileMenus[i]);

			ImGui::EndMainMenuBar();
		}
	}

	void DrawPopupMenu(const Ref<PopupMenu> &menu)
	{
		utils::DrawPopupMenuInternal(menu->GetName(), menu->GetMenuItems());
	}

	void DrawHelpMarker(const HLString &description, bool showHelpMarker)
	{
		if (showHelpMarker)
		{
			ImGui::SameLine();
			ImGui::TextDisabled("(?)");
		}

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(*description);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

	void PushID()
	{
		ImGui::PushID(s_ImGuiUIContextID++);
		s_ImGuiIDCounter = 0;
	}

	void PopID()
	{
		ImGui::PopID();
		--s_ImGuiUIContextID;
	}

	const char *GenerateID()
	{
		s_ImGuiIDBuffer[0] = '#';
		s_ImGuiIDBuffer[1] = '#';
		memset(s_ImGuiIDBuffer + 2, 0, 14);
		sprintf_s(s_ImGuiIDBuffer + 2, 14, "%o", s_ImGuiIDCounter++);
		return &s_ImGuiIDBuffer[0];
	}

	void SetToolTipV(const char *format, va_list args)
	{
		ImGui::BeginTooltipEx(0, ImGuiTooltipFlags_OverridePreviousTooltip);
		ImGui::TextV(format, args);
		ImGui::EndTooltip();
	}

	void SetToolTip(const char *format, ...)
	{
		va_list args;
		va_start(args, format);
		SetToolTipV(format, args);
		va_end(args);
	}

	ImVec2 CenteredText(const HLString &text, bool fullWidth)
	{
		float width = -1.0f;
		float fontSize = ImGui::GetFontSize() * (float)(text.Length() / 2);

		if (fullWidth)
		{
			width = ImGui::GetWindowSize().x / 2.0f;
		}
		else
		{
			width = ImGui::GetContentRegionAvail().x / 2.0f;
		}

		ImGui::SameLine(width, fontSize + (fontSize / 2));
		ImGui::Text(*text);
		return { width, fontSize + (fontSize / 2)};
	}

	void Separator()
	{
		ImGui::Separator();
	}

	void PushItemDisabled()
	{
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
	}

	void PopItemDisabled()
	{
		ImGui::PopItemFlag();
	}

	bool IsItemDisabled()
	{
		return ImGui::GetItemFlags() & ImGuiItemFlags_Disabled;
	}

	bool NavigatedTo()
	{
		ImGuiContext &g = *GImGui;
		return g.NavJustMovedToId == g.LastItemData.ID;
	}

	void BeginPropertyGrid(int32 colNum)
	{
		PushID();
		ImGui::Columns(colNum);
	}

	void EndPropertyGrid()
	{
		ImGui::Columns(1);
		PopID();
	}

	bool PropertyGridHeader(const HLString &name, bool openByDefault)
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		if (openByDefault)
			flags |= ImGuiTreeNodeFlags_DefaultOpen;

		bool open = false;
		const float framePaddingX = 6.0f;
		const float framePaddingY = 6.0f;

		UI::ScopedStyle headerRounding(ImGuiStyleVar_FrameRounding, 0.0f);
		UI::ScopedStyle headerPaddingHeight(ImGuiStyleVar_FramePadding, ImVec2(framePaddingX, framePaddingY));

		ImGui::PushID(*name);
		open = ImGui::TreeNodeEx("##noID", flags, name.C_Str());
		ImGui::PopID();

		return open;
	}

	bool BeginTreeNode(const HLString &name, bool defaultOpen)
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;
		if (defaultOpen)
			flags |= ImGuiTreeNodeFlags_DefaultOpen;

		return ImGui::TreeNodeEx(*name, flags);
	}

	void EndTreeNode()
	{
		ImGui::TreePop();
	}

	void BeginCheckboxGroup(const HLString &label)
	{
		ImGui::Text(*label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);
	}

	void EndCheckboxGroup()
	{
		ImGui::PopItemWidth();
		ImGui::NextColumn();
		s_ImGuiCheckboxCount = 0;
	}

	bool IsWindowHovered()
	{
		return ImGui::IsWindowHovered();
	}

	bool IsWindowFocused()
	{
		return ImGui::IsWindowFocused();
	}

	glm::vec2 GetCursorPosition()
	{
		ImVec2 cursorPos = ImGui::GetCursorPos();
		return { cursorPos.x, cursorPos.y };
	}

	glm::vec2 GetMousePosition()
	{
		ImVec2 pos = ImGui::GetMousePos();
		return { pos.x, pos.y };
	}

	glm::vec2 GetContentRegion()
	{
		ImVec2 region = ImGui::GetContentRegionAvail();
		return { region.x, region.y };
	}

	glm::vec2 GetWindowSize()
	{
		ImVec2 size = ImGui::GetWindowSize();
		return { size.x, size.y };
	}

	glm::vec2 GetWindowPos()
	{
		ImVec2 pos = ImGui::GetWindowPos();
		return { pos.x, pos.y };
	}

	std::pair<glm::vec2, glm::vec2> GetWindowRect()
	{
		ImVec2 min = ImGui::GetWindowContentRegionMin();
		ImVec2 max = ImGui::GetWindowContentRegionMax();
		return { { min.x, min.y }, { max.x, max.y } };
	}

	bool IsMouseHoveringRect(const glm::vec2 &min, const glm::vec2 &max)
	{
		return ImGui::IsMouseHoveringRect({ min.x, min.y }, { max.x, max.y });
	}

	bool IsMouseOverGizmo()
	{
		return ImGuizmo::IsOver();
	}

	bool IsUsingGizmo()
	{
		return ImGuizmo::IsUsing();
	}

	bool IsItemHovered()
	{
		return ImGui::IsItemHovered();
	}

	void EnableMenuBar()
	{
		s_UseMenuBar = true;
	}

	void DisableMenuBar()
	{
		s_UseMenuBar = false;
	}

	ImGuiWindowStyle GetCurrentWindowStyle()
	{
		return s_ImGuiWindowStyle;
	}

	ImU32 ColorWithValue(const ImColor &color, float value)
	{
		const ImVec4 &colorRow = color.Value;
		float hue, sat, val;
		ImGui::ColorConvertRGBtoHSV(colorRow.x, colorRow.y, colorRow.z, hue, sat, val);
		return ImColor::HSV(hue, sat, val, std::min(value, 1.0f));
	}

	ImU32 ColorWithMultipliedValue(const ImColor &color, float multiplier)
	{
		const ImVec4 &colorRow = color.Value;
		float hue, sat, val;
		ImGui::ColorConvertRGBtoHSV(colorRow.x, colorRow.y, colorRow.z, hue, sat, val);
		return ImColor::HSV(hue, sat, val, std::min(val * multiplier, 1.0f));
	}

	ImU32 ColorWithSaturation(const ImColor &color, float saturation)
	{
		const ImVec4 &colorRow = color.Value;
		float hue, sat, val;
		ImGui::ColorConvertRGBtoHSV(colorRow.x, colorRow.y, colorRow.z, hue, sat, val);
		return ImColor::HSV(hue, std::min(saturation, 1.0f), val);
	}

	ImU32 ColorWithMultipliedSaturation(const ImColor &color, float multiplier)
	{
		const ImVec4 &colorRow = color.Value;
		float hue, sat, val;
		ImGui::ColorConvertRGBtoHSV(colorRow.x, colorRow.y, colorRow.z, hue, sat, val);
		return ImColor::HSV(hue, std::min(sat * multiplier, 1.0f), val);
	}

	ImU32 ColorWithHue(const ImColor &color, float hue)
	{
		const ImVec4 &colorRow = color.Value;
		float h, s, v;
		ImGui::ColorConvertRGBtoHSV(colorRow.x, colorRow.y, colorRow.z, h, s, v);
		return ImColor::HSV(std::min(hue, 1.0f), s, v);
	}

	ImU32 ColorWithMultipliedHue(const ImColor &color, float multiplier)
	{
		const ImVec4 &colorRow = color.Value;
		float hue, sat, val;
		ImGui::ColorConvertRGBtoHSV(colorRow.x, colorRow.y, colorRow.z, hue, sat, val);
		return ImColor::HSV(std::min(hue * multiplier, 1.0f), sat, val);
	}

	void DrawShadow(const Ref<Texture2D> &shadowIcon, int32 radius, ImVec2 rectMin, ImVec2 rectMax, float alphaMultiplier, float lengthStretch, bool drawLeft, bool drawRight, bool drawTop, bool drawBottom)
	{
		const float widthOffset = lengthStretch;
		const float alphaTop = std::min(0.25f * alphaMultiplier, 1.0f);
		const float alphaSides = std::min(0.30f * alphaMultiplier, 1.0f);
		const float alphaBottom = std::min(0.60f * alphaMultiplier, 1.0f);
		const auto p1 = rectMin;
		const auto p2 = rectMax;

		ImTextureID texId = GetTextureID(shadowIcon);
		auto *drawList = ImGui::GetWindowDrawList();

		if (drawLeft)
			drawList->AddImage(texId, { p1.x - widthOffset, p1.y - radius }, { p2.x + widthOffset, p1.y }, ImVec2(0, 0), ImVec2(1, 1), ImColor(0.0f, 0.0f, 0.0f, alphaTop));

		if (drawRight)
			drawList->AddImage(texId, { p1.x - widthOffset, p2.y }, { p2.x + widthOffset, p2.y + radius }, ImVec2(0, 0), ImVec2(1, 1), ImColor(0.0f, 0.0f, 0.0f, alphaBottom));

		if (drawTop)
			drawList->AddImageQuad(texId, { p1.x - radius, p1.y - widthOffset }, { p1.x, p1.y - widthOffset }, { p1.x, p2.y + widthOffset }, { p1.x - radius, p2.y + widthOffset },
								   ImVec2(0, 0), ImVec2(0, 1), ImVec2(1, 1), ImVec2(1, 0), ImColor(0.0f, 0.0f, 0.0f, alphaSides));

		if (drawBottom)
			drawList->AddImageQuad(texId, { p2.x, p1.y - widthOffset }, { p2.x + radius, p1.y - widthOffset }, { p2.x + radius, p2.y + widthOffset }, { p2.x, p2.y + widthOffset },
								   ImVec2(0, 1), ImVec2(0, 0), ImVec2(1, 0), ImVec2(1, 1), ImColor(0.0f, 0.0f, 0.0f, alphaSides));
	}

	void DrawShadow(const Ref<Texture2D> &shadowIcon, int32 radius, ImRect rectangle, float alphaMultiplier, float lengthStretch, bool drawLeft, bool drawRight, bool drawTop, bool drawBottom)
	{
		DrawShadow(shadowIcon, radius, rectangle.Min, rectangle.Max, alphaMultiplier, lengthStretch, drawLeft, drawRight, drawTop, drawBottom);
	}

	void DrawShadow(const Ref<Texture2D> &shadowIcon, int32 radius, float alphaMultiplier, float lengthStretch, bool drawLeft, bool drawRight, bool drawTop, bool drawBottom)
	{
		DrawShadow(shadowIcon, radius, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), alphaMultiplier, lengthStretch, drawLeft, drawRight, drawTop, drawBottom);
	}

	void DrawShadowInner(const Ref<Texture2D> &shadowIcon, int32 radius, ImVec2 rectMin, ImVec2 rectMax, float alpha, float lengthStretch, bool drawLeft, bool drawRight, bool drawTop, bool drawBottom)
	{
		const float widthOffset = lengthStretch;
		const auto p1 = ImVec2(rectMin.x + radius, rectMin.y + radius);
		const auto p2 = ImVec2(rectMax.x - radius, rectMax.y - radius);
		auto *drawList = ImGui::GetWindowDrawList();

		ImTextureID textureID = GetTextureID(shadowIcon);

		if (drawTop)
			drawList->AddImage(textureID, { p1.x - widthOffset,  p1.y - radius }, { p2.x + widthOffset, p1.y }, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f), ImColor(0.0f, 0.0f, 0.0f, alpha));

		if (drawBottom)
			drawList->AddImage(textureID, { p1.x - widthOffset,  p2.y }, { p2.x + widthOffset, p2.y + radius }, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), ImColor(0.0f, 0.0f, 0.0f, alpha));

		if (drawLeft)
			drawList->AddImageQuad(textureID, { p1.x - radius, p1.y - widthOffset }, { p1.x, p1.y - widthOffset }, { p1.x, p2.y + widthOffset }, { p1.x - radius, p2.y + widthOffset },
								   { 0.0f, 1.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, ImColor(0.0f, 0.0f, 0.0f, alpha));
		if (drawRight)
			drawList->AddImageQuad(textureID, { p2.x, p1.y - widthOffset }, { p2.x + radius, p1.y - widthOffset }, { p2.x + radius, p2.y + widthOffset }, { p2.x, p2.y + widthOffset },
								   { 0.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, ImColor(0.0f, 0.0f, 0.0f, alpha));
	}

	void DrawShadowInner(const Ref<Texture2D> &shadowIcon, int32 radius, ImRect rectangle, float alpha, float lengthStretch, bool drawLeft, bool drawRight, bool drawTop, bool drawBottom)
	{
		DrawShadowInner(shadowIcon, radius, rectangle.Min, rectangle.Max, alpha, lengthStretch, drawLeft, drawRight, drawTop, drawBottom);
	}

	void DrawShadowInner(const Ref<Texture2D> &shadowIcon, int32 radius, float alpha, float lengthStretch, bool drawLeft, bool drawRight, bool drawTop, bool drawBottom)
	{
		DrawShadowInner(shadowIcon, radius, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), alpha, lengthStretch, drawLeft, drawRight, drawTop, drawBottom);
	}

	void DrawItemActivityOutline(float rounding, bool drawWhenInactive, ImColor colorWhenActive)
	{
		auto *drawList = ImGui::GetWindowDrawList();
		const ImRect rect = UI::RectExpanded(UI::GetItemRect(), 1.0, 1.0f);

		if (ImGui::IsItemHovered() && !ImGui::IsItemActive())
		{
			drawList->AddRect(rect.Min, rect.Max, ImColor(60, 60, 60), rounding, 0, 1.5f);
		}

		if (ImGui::IsItemActive())
		{
			drawList->AddRect(rect.Min, rect.Max, colorWhenActive, rounding, 0, 1.0f);
		}
		else if (!ImGui::IsItemHovered() && drawWhenInactive)
		{
			drawList->AddRect(rect.Min, rect.Max, ImColor(50, 50, 50), rounding, 0, 1.0f);
		}
	}

	ImRect GetItemRect()
	{
		return ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
	}

	ImRect RectExpanded(const ImRect &rect, float x, float y)
	{
		ImRect result = rect;
		result.Min.x -= x;
		result.Min.y -= y;
		result.Max.x += x;
		result.Max.y += y;
		return result;
	}

	ImRect RectExpanded(const ImRect &rect, const ImVec2 &xy)
	{
		return RectExpanded(rect, xy.x, xy.y);
	}

	ImRect RectOffset(const ImRect &rect, float x, float y)
	{
		ImRect result = rect;
		result.Min.x += x;
		result.Min.y += y;
		result.Max.x += x;
		result.Max.y += y;
		return result;
	}

	ImRect RectOffset(const ImRect &rect, const ImVec2 &xy)
	{
		return RectOffset(rect, xy.x, xy.y);
	}

	void ShiftCursor(float x, float y)
	{
		const ImVec2 cursor = ImGui::GetCursorPos();
		ImGui::SetCursorPos(ImVec2(cursor.x + x, cursor.y + y));
	}

	void ShiftCursorX(float distance)
	{
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + distance);
	}

	void ShiftCursorY(float distance)
	{
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + distance);
	}

	void DrawText(const HLString &text)
	{
		ImGui::Text(*text);
	}

	void DrawInputText(const HLString &label, const HLString &value)
	{
		ImGui::Text(*label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		s_ImGuiIDBuffer[0] = '#';
		s_ImGuiIDBuffer[1] = '#';
		memset(s_ImGuiIDBuffer + 2, 0, 14);
		_itoa(s_ImGuiIDCounter++, s_ImGuiIDBuffer + 2, 14);

		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		ImGui::InputText(s_ImGuiIDBuffer, (char*)*value, value.Length(), ImGuiInputTextFlags_ReadOnly);
		ImGui::PopStyleVar();

		ImGui::PopItemWidth();
		ImGui::NextColumn();
	}

	bool DrawInputText(const HLString &label, HLString &value, bool fullWidth)
	{
		bool modified = false;

		if (!label.IsEmpty())
		{
			ImGui::Text(*label);
			ImGui::NextColumn();
		}

		if (fullWidth)
			ImGui::PushItemWidth(-1);

		char buffer[256];
		strcpy_s<256>(buffer, *value);

		s_ImGuiIDBuffer[0] = '#';
		s_ImGuiIDBuffer[1] = '#';
		memset(s_ImGuiIDBuffer + 2, 0, 14);
		sprintf_s(s_ImGuiIDBuffer + 2, 14, "%o", s_ImGuiIDCounter++);

		if (IsItemDisabled())
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);

		if (ImGui::InputText(s_ImGuiIDBuffer, buffer, 256))
		{
			value = buffer;
			modified = true;
		}

		if (IsItemDisabled())
			ImGui::PopStyleVar();

		if (fullWidth)
			ImGui::PopItemWidth();

		ImGui::NextColumn();

		return modified;
	}

	bool DrawInputTextWithLength(const HLString &label, HLString &value, size_t length)
	{
		ImGui::Text(*label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		s_ImGuiIDBuffer[0] = '#';
		s_ImGuiIDBuffer[1] = '#';
		memset(s_ImGuiIDBuffer + 2, 0, 14);
		sprintf_s(s_ImGuiIDBuffer + 2, 14, "%o", s_ImGuiIDCounter++);

		if (IsItemDisabled())
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);

		bool changed = ImGui::InputText(s_ImGuiIDBuffer, value, length);

		if (IsItemDisabled())
			ImGui::PopStyleVar();

		ImGui::PopItemWidth();
		ImGui::NextColumn();
		return changed;
	}

	bool DrawCheckbox(const HLString &label, bool &value, LabelAlignment alignment)
	{
		bool modified = false;

		if (alignment == LabelAlignment::Left)
		{
			ImGui::Text(*label);
			ImGui::NextColumn();
		}

		ImGui::PushItemWidth(-1);

		s_ImGuiIDBuffer[0] = '#';
		s_ImGuiIDBuffer[1] = '#';
		memset(s_ImGuiIDBuffer + 2, 0, 14);
		sprintf_s(s_ImGuiIDBuffer + 2, 14, "%o", s_ImGuiIDCounter++);

		if (IsItemDisabled())
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);

		if (alignment == LabelAlignment::Left)
		{
			ImGui::SameLine();
		}

		if (ImGui::Checkbox(s_ImGuiIDBuffer, &value))
			modified = true;

		if (alignment == LabelAlignment::Right)
		{
			ImGui::SameLine();
			ImGui::Text(*label);
		}

		if (IsItemDisabled())
			ImGui::PopStyleVar();

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	bool DrawCheckboxGroup(const HLString &label, bool &value)
	{
		bool modified = false;

		if (++s_ImGuiCheckboxCount > 1)
			ImGui::SameLine();

		ImGui::Text(*label);
		ImGui::SameLine();

		s_ImGuiIDBuffer[0] = '#';
		s_ImGuiIDBuffer[1] = '#';
		memset(s_ImGuiIDBuffer + 2, 0, 14);
		sprintf_s(s_ImGuiIDBuffer + 2, 14, "%o", s_ImGuiIDCounter++);

		if (IsItemDisabled())
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);

		if (ImGui::Checkbox(s_ImGuiIDBuffer, &value))
			modified = true;

		if (IsItemDisabled())
			ImGui::PopStyleVar();

		return modified;
	}

	bool DrawColorPicker(const HLString &label, glm::vec3 &value)
	{
		bool modified = false;

		ImGui::Text(*label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		s_ImGuiIDBuffer[0] = '#';
		s_ImGuiIDBuffer[1] = '#';
		memset(s_ImGuiIDBuffer + 2, 0, 14);
		sprintf_s(s_ImGuiIDBuffer + 2, 14, "%o", s_ImGuiIDCounter++);

		if (IsItemDisabled())
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);

		if (ImGui::ColorEdit3(s_ImGuiIDBuffer, glm::value_ptr(value)))
			modified = true;

		if (IsItemDisabled())
			ImGui::PopStyleVar();

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	bool DrawButton(const HLString &label, const ImVec2 &size)
	{
		bool result = ImGui::Button(*label, size);
		ImGui::NextColumn();
		return result;
	}

	bool DrawDragInt(const HLString &label, int32 &value)
	{
		bool modified = false;

		ImGui::Text(*label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		s_ImGuiIDBuffer[0] = '#';
		s_ImGuiIDBuffer[1] = '#';
		memset(s_ImGuiIDBuffer + 2, 0, 14);
		sprintf_s(s_ImGuiIDBuffer + 2, 14, "%o", s_ImGuiIDCounter++);

		if (IsItemDisabled())
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);

		ImGui::SameLine();
		if (ImGui::DragInt(s_ImGuiIDBuffer, &value))
			modified = true;

		if (IsItemDisabled())
			ImGui::PopStyleVar();

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	bool DrawDragFloat(const HLString &label, float &value, float delta, float min, float max)
	{
		bool modified = false;

		ImGui::Text(*label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		s_ImGuiIDBuffer[0] = '#';
		s_ImGuiIDBuffer[1] = '#';
		memset(s_ImGuiIDBuffer + 2, 0, 14);
		sprintf_s(s_ImGuiIDBuffer + 2, 14, "%o", s_ImGuiIDCounter++);

		if (IsItemDisabled())
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);

		if (ImGui::DragFloat(s_ImGuiIDBuffer, &value, delta, min, max))
			modified = true;

		if (IsItemDisabled())
			ImGui::PopStyleVar();

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	bool DrawDragFloat2(const HLString &label, glm::vec2 &value, float delta)
	{
		bool modified = false;

		ImGui::Text(*label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		s_ImGuiIDBuffer[0] = '#';
		s_ImGuiIDBuffer[1] = '#';
		memset(s_ImGuiIDBuffer + 2, 0, 14);
		sprintf_s(s_ImGuiIDBuffer + 2, 14, "%o", s_ImGuiIDCounter++);

		if (IsItemDisabled())
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);

		if (ImGui::DragFloat2(s_ImGuiIDBuffer, glm::value_ptr(value), delta))
			modified = true;

		if (IsItemDisabled())
			ImGui::PopStyleVar();

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	bool DrawDragFloat3(const HLString &label, glm::vec3 &value, float delta)
	{
		bool modified = false;

		ImGui::Text(*label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		s_ImGuiIDBuffer[0] = '#';
		s_ImGuiIDBuffer[1] = '#';
		memset(s_ImGuiIDBuffer + 2, 0, 14);
		sprintf_s(s_ImGuiIDBuffer + 2, 14, "%o", s_ImGuiIDCounter++);

		if (IsItemDisabled())
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);

		if (ImGui::DragFloat3(s_ImGuiIDBuffer, glm::value_ptr(value), delta))
			modified = true;

		if (IsItemDisabled())
			ImGui::PopStyleVar();

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	bool DrawDragFloat4(const HLString &label, glm::vec4 &value, float delta)
	{
		bool modified = false;

		ImGui::Text(*label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		s_ImGuiIDBuffer[0] = '#';
		s_ImGuiIDBuffer[1] = '#';
		memset(s_ImGuiIDBuffer + 2, 0, 14);
		sprintf_s(s_ImGuiIDBuffer + 2, 14, "%o", s_ImGuiIDCounter++);

		if (IsItemDisabled())
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);

		if (ImGui::DragFloat4(s_ImGuiIDBuffer, glm::value_ptr(value), delta))
			modified = true;

		if (IsItemDisabled())
			ImGui::PopStyleVar();

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	bool DrawIntSlider(const HLString &label, int32 &value, int32 min, int32 max)
	{
		bool modified = false;

		ImGui::Text(*label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		s_ImGuiIDBuffer[0] = '#';
		s_ImGuiIDBuffer[1] = '#';
		memset(s_ImGuiIDBuffer + 2, 0, 14);
		sprintf_s(s_ImGuiIDBuffer + 2, 14, "%o", s_ImGuiIDCounter++);

		if (IsItemDisabled())
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);

		if (ImGui::SliderInt(s_ImGuiIDBuffer, &value, min, max))
			modified = true;

		if (IsItemDisabled())
			ImGui::PopStyleVar();

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	bool DrawUIntSlider(const HLString &label, uint32 &value, int32 min, int32 max)
	{
		bool modified = false;

		ImGui::Text(*label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		s_ImGuiIDBuffer[0] = '#';
		s_ImGuiIDBuffer[1] = '#';
		memset(s_ImGuiIDBuffer + 2, 0, 14);
		sprintf_s(s_ImGuiIDBuffer + 2, 14, "%o", s_ImGuiIDCounter++);

		if (IsItemDisabled())
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);

		int32 tmp = (int32)value;
		if (ImGui::SliderInt(s_ImGuiIDBuffer, &tmp, min, max))
		{
			modified = true;
			value = (uint32)tmp;
		}

		if (IsItemDisabled())
			ImGui::PopStyleVar();

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	bool DrawFloatSlider(const HLString &label, float &value, float min, float max)
	{
		bool modified = false;

		ImGui::Text(*label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		s_ImGuiIDBuffer[0] = '#';
		s_ImGuiIDBuffer[1] = '#';
		memset(s_ImGuiIDBuffer + 2, 0, 14);
		sprintf_s(s_ImGuiIDBuffer + 2, 14, "%o", s_ImGuiIDCounter++);

		if (IsItemDisabled())
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);

		if (ImGui::SliderFloat(s_ImGuiIDBuffer, &value, min, max))
			modified = true;

		if (IsItemDisabled())
			ImGui::PopStyleVar();

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	bool DrawVec2Slider(const HLString &label, glm::vec2 &value, float min, float max)
	{
		bool modified = false;

		ImGui::Text(*label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		s_ImGuiIDBuffer[0] = '#';
		s_ImGuiIDBuffer[1] = '#';
		memset(s_ImGuiIDBuffer + 2, 0, 14);
		sprintf_s(s_ImGuiIDBuffer + 2, 14, "%o", s_ImGuiIDCounter++);

		if (IsItemDisabled())
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);

		if (ImGui::SliderFloat2(s_ImGuiIDBuffer, glm::value_ptr(value), min, max))
			modified = true;

		if (IsItemDisabled())
			ImGui::PopStyleVar();

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	bool DrawVec3Slider(const HLString &label, glm::vec3 &value, float min, float max)
	{
		bool modified = false;

		ImGui::Text(*label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		s_ImGuiIDBuffer[0] = '#';
		s_ImGuiIDBuffer[1] = '#';
		memset(s_ImGuiIDBuffer + 2, 0, 14);
		sprintf_s(s_ImGuiIDBuffer + 2, 14, "%o", s_ImGuiIDCounter++);

		if (IsItemDisabled())
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);

		if (ImGui::SliderFloat3(s_ImGuiIDBuffer, glm::value_ptr(value), min, max))
			modified = true;

		if (IsItemDisabled())
			ImGui::PopStyleVar();

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	bool DrawVec4Slider(const HLString &label, glm::vec4 &value, float min, float max)
	{
		bool modified = false;

		ImGui::Text(*label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		s_ImGuiIDBuffer[0] = '#';
		s_ImGuiIDBuffer[1] = '#';
		memset(s_ImGuiIDBuffer + 2, 0, 14);
		sprintf_s(s_ImGuiIDBuffer + 2, 14, "%o", s_ImGuiIDCounter++);

		if (IsItemDisabled())
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);

		if (ImGui::SliderFloat4(s_ImGuiIDBuffer, glm::value_ptr(value), min, max))
			modified = true;

		if (IsItemDisabled())
			ImGui::PopStyleVar();

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	bool DrawVec2(const HLString &label, glm::vec2 &values, float resetValue, float columnWidth)
	{
		bool modified = false;

		UI::PushID();
		ImGui::TableSetColumnIndex(0);
		UI::ShiftCursor(17.0f, 7.0f);

		ImGui::Text(*label);
		UI::DrawUnderline(false, 0.0f, 2.0f);

		ImGui::TableSetColumnIndex(1);
		UI::ShiftCursor(7.0f, 0.0f);

		{
			const float spacingX = 8.0f;
			UI::ScopedStyle itemSpacing(ImGuiStyleVar_ItemSpacing, ImVec2{ spacingX, 0.0f });
			UI::ScopedStyle padding(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 2.0f });

			{
				// Begin XYZ area
				UI::ScopedColor paddingXYZ(ImGuiCol_Border, IM_COL32(0, 0, 0, 0));
				UI::ScopedColor frameXYZ(ImGuiCol_FrameBg, IM_COL32(0, 0, 0, 0));

				ImGui::BeginChild(ImGui::GetID((label + "fr").C_Str()), ImVec2(ImGui::GetContentRegionAvail().x - spacingX, ImGui::GetFrameHeightWithSpacing() + 8.0f), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
			}
			const float framePadding = 2.0f;
			const float outlineSpacing = 1.0f;
			const float lineHeight = GImGui->Font->FontSize + framePadding * 2.0f;
			const ImVec2 buttonSize = { lineHeight + 2.0f, lineHeight };
			const float inputItemWidth = (ImGui::GetContentRegionAvail().x - spacingX) / 3.0f - buttonSize.x;

			const ImGuiIO &io = ImGui::GetIO();
			auto boldFont = io.Fonts->Fonts[0];

			auto drawControl = [&](const HLString &label, float &value, const ImVec4 &colourN,
								   const ImVec4 &colourH,
								   const ImVec4 &colourP)
			{
				{
					UI::ScopedStyle buttonFrame(ImGuiStyleVar_FramePadding, ImVec2(framePadding, 0.0f));
					UI::ScopedStyle buttonRounding(ImGuiStyleVar_FrameRounding, 1.0f);
					UI::ScopedColorStack buttonColours(ImGuiCol_Button, colourN, ImGuiCol_ButtonHovered, colourH, ImGuiCol_ButtonActive, colourP);

					UI::ScopedFont buttonFont(boldFont);

					UI::ShiftCursorY(2.0f);
					if (ImGui::Button(label.C_Str(), buttonSize))
					{
						value = resetValue;
						modified = true;
					}
				}

				ImGui::SameLine(0.0f, outlineSpacing);
				ImGui::SetNextItemWidth(inputItemWidth);
				UI::ShiftCursorY(-2.0f);
				modified |= ImGui::DragFloat(("##" + label).C_Str(), &value, 0.1f, 0.0f, 0.0f, "%.2f");

				if (!UI::IsItemDisabled())
					UI::DrawItemActivityOutline(2.0f, true, Colors::Theme::Accent);
			};

			drawControl("X", values.x, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f }, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f }, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });

			ImGui::SameLine(0.0f, outlineSpacing);
			drawControl("Y", values.y, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f }, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f }, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });

			ImGui::EndChild();
		}
		UI::PopID();

		return modified;
	}

	bool DrawVec3(const HLString &label, glm::vec3 &values, float resetValue, float columnWidth)
	{
		bool modified = false;

		UI::PushID();
		ImGui::TableSetColumnIndex(0);
		UI::ShiftCursor(17.0f, 7.0f);

		ImGui::Text(*label);
		UI::DrawUnderline(false, 0.0f, 2.0f);

		ImGui::TableSetColumnIndex(1);
		UI::ShiftCursor(7.0f, 0.0f);

		{
			const float spacingX = 8.0f;
			UI::ScopedStyle itemSpacing(ImGuiStyleVar_ItemSpacing, ImVec2{ spacingX, 0.0f });
			UI::ScopedStyle padding(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 2.0f });

			{
				// Begin XYZ area
				UI::ScopedColor paddingXYZ(ImGuiCol_Border, IM_COL32(0, 0, 0, 0));
				UI::ScopedColor frameXYZ(ImGuiCol_FrameBg, IM_COL32(0, 0, 0, 0));

				ImGui::BeginChild(ImGui::GetID((label + "fr").C_Str()), ImVec2(ImGui::GetContentRegionAvail().x - spacingX, ImGui::GetFrameHeightWithSpacing() + 8.0f), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
			}
			const float framePadding = 2.0f;
			const float outlineSpacing = 1.0f;
			const float lineHeight = GImGui->Font->FontSize + framePadding * 2.0f;
			const ImVec2 buttonSize = { lineHeight + 2.0f, lineHeight };
			const float inputItemWidth = (ImGui::GetContentRegionAvail().x - spacingX) / 3.0f - buttonSize.x;

			const ImGuiIO &io = ImGui::GetIO();
			auto boldFont = io.Fonts->Fonts[0];

			auto drawControl = [&](const HLString &label, float &value, const ImVec4 &colourN,
								   const ImVec4 &colourH,
								   const ImVec4 &colourP)
			{
				{
					UI::ScopedStyle buttonFrame(ImGuiStyleVar_FramePadding, ImVec2(framePadding, 0.0f));
					UI::ScopedStyle buttonRounding(ImGuiStyleVar_FrameRounding, 1.0f);
					UI::ScopedColorStack buttonColours(ImGuiCol_Button, colourN, ImGuiCol_ButtonHovered, colourH, ImGuiCol_ButtonActive, colourP);

					UI::ScopedFont buttonFont(boldFont);

					UI::ShiftCursorY(2.0f);
					if (ImGui::Button(label.C_Str(), buttonSize))
					{
						value = resetValue;
						modified = true;
					}
				}

				ImGui::SameLine(0.0f, outlineSpacing);
				ImGui::SetNextItemWidth(inputItemWidth);
				UI::ShiftCursorY(-2.0f);
				modified |= ImGui::DragFloat(("##" + label).C_Str(), &value, 0.1f, 0.0f, 0.0f, "%.2f");

				if (!UI::IsItemDisabled())
					UI::DrawItemActivityOutline(2.0f, true, Colors::Theme::Accent);
			};

			drawControl("X", values.x, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f }, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f }, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });

			ImGui::SameLine(0.0f, outlineSpacing);
			drawControl("Y", values.y, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f }, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f }, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });

			ImGui::SameLine(0.0f, outlineSpacing);
			drawControl("Z", values.z, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f }, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f }, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });

			ImGui::EndChild();
		}
		UI::PopID();

		return modified;
	}

	bool DrawVec4(const HLString &label, glm::vec4 &values, float resetValue, float columnWidth)
	{
		bool modified = false;

		UI::PushID();
		ImGui::TableSetColumnIndex(0);
		UI::ShiftCursor(17.0f, 7.0f);

		ImGui::Text(*label);
		UI::DrawUnderline(false, 0.0f, 2.0f);

		ImGui::TableSetColumnIndex(1);
		UI::ShiftCursor(7.0f, 0.0f);

		{
			const float spacingX = 8.0f;
			UI::ScopedStyle itemSpacing(ImGuiStyleVar_ItemSpacing, ImVec2{ spacingX, 0.0f });
			UI::ScopedStyle padding(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 2.0f });

			{
				// Begin XYZ area
				UI::ScopedColor paddingXYZ(ImGuiCol_Border, IM_COL32(0, 0, 0, 0));
				UI::ScopedColor frameXYZ(ImGuiCol_FrameBg, IM_COL32(0, 0, 0, 0));

				ImGui::BeginChild(ImGui::GetID((label + "fr").C_Str()), ImVec2(ImGui::GetContentRegionAvail().x - spacingX, ImGui::GetFrameHeightWithSpacing() + 8.0f), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
			}
			const float framePadding = 2.0f;
			const float outlineSpacing = 1.0f;
			const float lineHeight = GImGui->Font->FontSize + framePadding * 2.0f;
			const ImVec2 buttonSize = { lineHeight + 2.0f, lineHeight };
			const float inputItemWidth = (ImGui::GetContentRegionAvail().x - spacingX) / 3.0f - buttonSize.x;

			const ImGuiIO &io = ImGui::GetIO();
			auto boldFont = io.Fonts->Fonts[0];

			auto drawControl = [&](const HLString &label, float &value, const ImVec4 &colourN,
								   const ImVec4 &colourH,
								   const ImVec4 &colourP)
			{
				{
					UI::ScopedStyle buttonFrame(ImGuiStyleVar_FramePadding, ImVec2(framePadding, 0.0f));
					UI::ScopedStyle buttonRounding(ImGuiStyleVar_FrameRounding, 1.0f);
					UI::ScopedColorStack buttonColours(ImGuiCol_Button, colourN, ImGuiCol_ButtonHovered, colourH, ImGuiCol_ButtonActive, colourP);

					UI::ScopedFont buttonFont(boldFont);

					UI::ShiftCursorY(2.0f);
					if (ImGui::Button(label.C_Str(), buttonSize))
					{
						value = resetValue;
						modified = true;
					}
				}

				ImGui::SameLine(0.0f, outlineSpacing);
				ImGui::SetNextItemWidth(inputItemWidth);
				UI::ShiftCursorY(-2.0f);
				modified |= ImGui::DragFloat(("##" + label).C_Str(), &value, 0.1f, 0.0f, 0.0f, "%.2f");

				if (!UI::IsItemDisabled())
					UI::DrawItemActivityOutline(2.0f, true, Colors::Theme::Accent);
			};

			drawControl("X", values.x, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f }, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f }, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });

			ImGui::SameLine(0.0f, outlineSpacing);
			drawControl("Y", values.y, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f }, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f }, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });

			ImGui::SameLine(0.0f, outlineSpacing);
			drawControl("Z", values.z, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f }, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f }, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });

// TODO: edit these colors
			ImGui::SameLine(0.0f, outlineSpacing);
			drawControl("W", values.w, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f }, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f }, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });

			ImGui::EndChild();
		}
		UI::PopID();

		return modified;
	}

	void DrawUnderline(bool fullWidth, float offsetX, float offsetY)
	{
		if (fullWidth)
		{
			if (ImGui::GetCurrentWindow()->DC.CurrentColumns != nullptr)
				ImGui::PushColumnsBackground();
			else if (ImGui::GetCurrentTable() != nullptr)
				ImGui::TablePushBackgroundChannel();
		}

		const float width = fullWidth ? ImGui::GetWindowWidth() : ImGui::GetContentRegionAvail().x;
		const ImVec2 cursor = ImGui::GetCursorScreenPos();

		ImGui::GetWindowDrawList()->AddLine(ImVec2(cursor.x + offsetX, cursor.y + offsetY), ImVec2(cursor.x + width, cursor.y + offsetY), Colors::Theme::BackgroundDark, 1.0f);

		if (fullWidth)
		{
			if (ImGui::GetCurrentWindow()->DC.CurrentColumns != nullptr)
				ImGui::PopColumnsBackground();
			else if (ImGui::GetCurrentTable() != nullptr)
				ImGui::TablePopBackgroundChannel();
		}
	}

	bool DrawDropdown(const HLString &label, const char **options, int32 optionsCount, int32 *selected)
	{
		const char *current = options[*selected];
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
					*selected = i;
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

	bool DrawDropdown(const HLString &label, const std::vector<HLString> &options, int32 *selected)
	{
		const char *current = options[*selected].C_Str();
		bool changed = false;
		const HLString id = "##" + HLString(label);
		
		ImGui::Text(*label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		if (IsItemDisabled())
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);

		ImGui::SameLine();
		if (ImGui::BeginCombo(*id, current))
		{
			for (int32 i = 0; i < options.size(); ++i)
			{
				const bool isSelected = (current == options[i]);
				if (ImGui::Selectable(*options[i], isSelected))
				{
					current = options[i].C_Str();
					*selected = i;
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

	ImTextureID GetTextureID(const Ref<Texture2D> &texture)
	{
#ifdef HIGHLO_API_VULKAN
		Ref<VulkanTexture2D> vulkanTexture = texture.As<VulkanTexture2D>();
		const VkDescriptorImageInfo &textureInfo = vulkanTexture->GetVulkanDescriptorInfo();
		if (!textureInfo.imageView)
			return nullptr;

		return ImGui_ImplVulkan_AddTexture(textureInfo.sampler, textureInfo.imageView, textureInfo.imageLayout);
#else
		return (ImTextureID)(uint64)texture->GetRendererID();
#endif // HIGHLO_API_VULKAN
	}

	void Image(const Ref<Texture2D> &texture, const ImVec2 &size, const ImVec2 &uv0, const ImVec2 &uv1, const ImVec4 &tintColor, const ImVec4 &borderColor)
	{
#ifdef HIGHLO_API_VULKAN
		Ref<VulkanTexture2D> vulkanTexture = texture.As<VulkanTexture2D>();
		const auto &textureInfo = vulkanTexture->GetTextureInfo();
		if (!textureInfo.ImageView)
			return;

		const auto textureId = ImGui_ImplVulkan_AddTexture(textureInfo.Sampler, textureInfo.ImageView, vulkanTexture->GetVulkanDescriptorInfo().imageLayout);
		ImGui::Image(textureId, size, uv0, uv1, tintColor, borderColor);
#else
		ImGui::Image(GetTextureID(texture), size, uv0, uv1, tintColor, borderColor);
#endif // HIGHLO_API_VULKAN	
	}
	
	void Image(const Ref<Texture2D> &texture, uint32 layer, const ImVec2 &size, const ImVec2 &uv0, const ImVec2 &uv1, const ImVec4 &tintColor, const ImVec4 &borderColor)
	{
#ifdef HIGHLO_API_VULKAN
		Ref<VulkanTexture2D> vulkanTexture = texture.As<VulkanTexture2D>();
		auto &textureInfo = vulkanTexture->GetTextureInfo();
		textureInfo.ImageView = vulkanTexture->GetLayerImageView(layer);
		if (!textureInfo.ImageView)
			return;

		const auto textureId = ImGui_ImplVulkan_AddTexture(textureInfo.Sampler, textureInfo.ImageView, vulkanTexture->GetVulkanDescriptorInfo().imageLayout);
		return ImGui::Image(textureId, size, uv0, uv1, tintColor, borderColor);
#else
		ImGui::Image(GetTextureID(texture), size, uv0, uv1, tintColor, borderColor);
#endif // HIGHLO_API_VULKAN
	}
	
	void ImageMip(const Ref<Texture2D> &texture, uint32 mip, const ImVec2 &size, const ImVec2 &uv0, const ImVec2 &uv1, const ImVec4 &tintColor, const ImVec4 &borderColor)
	{
#ifdef HIGHLO_API_VULKAN
		Ref<VulkanTexture2D> vulkanTexture = texture.As<VulkanTexture2D>();
		auto &textureInfo = vulkanTexture->GetTextureInfo();
		textureInfo.ImageView = vulkanTexture->GetMipImageView(mip);
		if (!textureInfo.ImageView)
			return;

		const auto textureId = ImGui_ImplVulkan_AddTexture(textureInfo.Sampler, textureInfo.ImageView, vulkanTexture->GetVulkanDescriptorInfo().imageLayout);
		return ImGui::Image(textureId, size, uv0, uv1, tintColor, borderColor);
#else
		// TODO
#endif // HIGHLO_API_VULKAN
	}
	
	bool ImageButton(const Ref<Texture2D> &texture, const ImVec2 &size, const ImVec2 &uv0, const ImVec2 &uv1, int32 framePadding, const ImVec4 &bgColor, const ImVec4 &tintColor)
	{
		return ImageButton("", texture, size, uv0, uv1, framePadding, bgColor, tintColor);
	}
	
	bool ImageButton(const HLString &id, const Ref<Texture2D> &texture, const ImVec2 &size, const ImVec2 &uv0, const ImVec2 &uv1, int32 framePadding, const ImVec4 &bgColor, const ImVec4 &tintColor)
	{
#ifdef HIGHLO_API_VULKAN
		if (!texture)
			return false;

		Ref<VulkanTexture2D> vulkanTexture = texture.As<VulkanTexture2D>();

		const VkDescriptorImageInfo &imageInfo = vulkanTexture->GetVulkanDescriptorInfo();
		const auto textureId = ImGui_ImplVulkan_AddTexture(imageInfo.sampler, imageInfo.imageView, imageInfo.imageLayout);
		ImGuiID imguiId = (ImGuiID)((((uint64)imageInfo.imageView) >> 32) ^ (uint32)imageInfo.imageView);
		if (!id.IsEmpty())
		{
			const ImGuiID strID = ImGui::GetID(*id);
			imguiId = imguiId ^ strID;
		}

		return ImGui::ImageButtonEx(imguiId, textureId, size, uv0, uv1, ImVec2((float)framePadding, (float)framePadding), bgColor, tintColor);

#else
		if (!texture)
			return false;

		return ImGui::ImageButton(GetTextureID(texture), size, uv0, uv1, framePadding, bgColor, tintColor);
#endif // HIGHLO_API_VULKAN
	}

	void DrawButtonImage(const Ref<Texture2D> &texture, const Ref<Texture2D> &textureHovered, const Ref<Texture2D> &texturePressed, ImU32 tintNormal, ImU32 tintHovered, ImU32 tintPressed, ImVec2 rectMin, ImVec2 rectMax)
	{
		auto *drawList = ImGui::GetWindowDrawList();
		if (ImGui::IsItemActive())
			drawList->AddImage(GetTextureID(texturePressed), rectMin, rectMax, ImVec2(0, 0), ImVec2(1, 1), tintPressed);
		else if (ImGui::IsItemHovered())
			drawList->AddImage(GetTextureID(textureHovered), rectMin, rectMax, ImVec2(0, 0), ImVec2(1, 1), tintHovered);
		else
			drawList->AddImage(GetTextureID(texture), rectMin, rectMax, ImVec2(0, 0), ImVec2(1, 1), tintNormal);
	}
	
	void DrawButtonImage(const Ref<Texture2D> &texture, const Ref<Texture2D> &textureHovered, const Ref<Texture2D> &texturePressed, ImU32 tintNormal, ImU32 tintHovered, ImU32 tintPressed, ImRect rectangle)
	{
		DrawButtonImage(texture, textureHovered, texturePressed, tintNormal, tintHovered, tintPressed, rectangle.Min, rectangle.Max);
	}
	
	void DrawButtonImage(const Ref<Texture2D> &texture, ImU32 tintNormal, ImU32 tintHovered, ImU32 tintPressed, ImVec2 rectMin, ImVec2 rectMax)
	{
		DrawButtonImage(texture, texture, texture, tintNormal, tintHovered, tintPressed, rectMin, rectMax);
	}
	
	void DrawButtonImage(const Ref<Texture2D> &texture, ImU32 tintNormal, ImU32 tintHovered, ImU32 tintPressed, ImRect rectangle)
	{
		DrawButtonImage(texture, texture, texture, tintNormal, tintHovered, tintPressed, rectangle.Min, rectangle.Max);
	}
	
	void DrawButtonImage(const Ref<Texture2D> &texture, const Ref<Texture2D> &textureHovered, const Ref<Texture2D> &texturePressed, ImU32 tintNormal, ImU32 tintHovered, ImU32 tintPressed)
	{
		DrawButtonImage(texture, textureHovered, texturePressed, tintNormal, tintHovered, tintPressed, ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
	}
	
	void DrawButtonImage(const Ref<Texture2D> &texture, ImU32 tintNormal, ImU32 tintHovered, ImU32 tintPressed)
	{
		DrawButtonImage(texture, texture, texture, tintNormal, tintHovered, tintPressed, ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
	}
}

