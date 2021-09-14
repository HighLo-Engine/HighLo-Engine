// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "ImGui.h"

#include "Engine/Application/Application.h"
#include "ImGuizmo.h"

#ifdef HIGHLO_API_OPENGL
	#include "Engine/ImGui/ImGui/backends/imgui_impl_opengl3.h"
	#include "Engine/Platform/OpenGL/OpenGLTexture.h"
	#define IMGUI_RENDER_API_IMPL_FN_NAME(fn) ImGui_ImplOpenGL3_##fn
#endif // HIGHLO_API_OPENGL

#ifdef HIGHLO_API_DX11
	#include "Engine/ImGui/ImGui/backends/imgui_impl_dx11.h"
	#include "Engine/Platform/DX11/DX11Resources.h"
	#define IMGUI_RENDER_API_IMPL_FN_NAME(fn) ImGui_ImplDX11_##fn
#endif // HIGHLO_API_DX11

#ifdef HIGHLO_API_GLFW
	#include <GLFW/glfw3.h>
	#include "Engine/ImGui/ImGui/backends/imgui_impl_glfw.h"
	#define IMGUI_WINDOW_IMPL_FN_NAME(fn) ImGui_ImplGlfw_##fn
#else
	#include "Engine/ImGui/ImGui/backends/imgui_impl_win32.h"
	#define IMGUI_WINDOW_IMPL_FN_NAME(fn) ImGui_ImplWin32_##fn
#endif // HIGHLO_API_GLFW

namespace highlo::UI
{
	namespace utils
	{
		static void DrawSubPopupMenuInternal(Ref<PopupMenu> &parentMenu, const HLString &name, const std::vector<PopupMenuItem> &items)
		{
			// TODO
		}

		static void DrawSubMenuInternal(Ref<FileMenu> &parentMenu, const HLString &menuName, const std::vector<MenuItem> &items)
		{
		if (ImGui::BeginMenu(*menuName))
		{
			for (MenuItem currentItem : items)
			{
				if (currentItem.IsSubmenu)
					DrawSubMenuInternal(parentMenu, currentItem.Name, currentItem.SubmenuItems);

				if (currentItem.Separator)
				{
					ImGui::Separator();
					continue;
				}

				if (ImGui::MenuItem(*currentItem.Name, *currentItem.Shortcut, false, currentItem.Visible))
					currentItem.Callback(parentMenu.Get(), &currentItem);
			}

			ImGui::EndMenu();
			}
		}

		static void DrawMenuInternal(Ref<FileMenu> &fileMenu)
		{
			if (ImGui::BeginMenu(*fileMenu->GetName()))
			{
				const std::vector<MenuItem> items = fileMenu->GetMenuItems();
				for (int32 i = 0; i < items.size(); ++i)
				{
					MenuItem currentItem = items[i];
					if (currentItem.IsSubmenu)
						DrawSubMenuInternal(fileMenu, currentItem.Name, currentItem.SubmenuItems);

					if (currentItem.Separator)
					{
						ImGui::Separator();
						continue;
					}

					if (ImGui::MenuItem(*currentItem.Name, *currentItem.Shortcut, false, currentItem.Visible))
						currentItem.Callback(fileMenu.Get(), &currentItem);
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
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepage controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Enable docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;     // Enable Multi-Viewport / Platform Windows
		io.ConfigViewportsNoAutoMerge = true;
		io.ConfigViewportsNoTaskBarIcon = true;

		ImFont *font = io.Fonts->AddFontFromFileTTF("assets/fonts/BarlowSemiCondensedFontFamily/BarlowSemiCondensed-Regular.ttf", 18.0f);
		ImFont *boldFont = io.Fonts->AddFontFromFileTTF("assets/fonts/BarlowSemiCondensedFontFamily/BarlowSemiCondensed-Bold.ttf", 18.0f);
		io.FontDefault = io.Fonts->Fonts[0];

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

	#ifdef HIGHLO_API_GLFW
		ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window->GetNativeHandle(), true);
	#else
		ImGui_ImplWin32_Init(window->GetNativeHandle(), window->GetNativeContext());
	#endif // HIGHLO_API_DX11

	#ifdef HIGHLO_API_OPENGL
		ImGui_ImplOpenGL3_Init("#version 410");
	#endif // HIGHLO_API_OPENGL

	#ifdef HIGHLO_API_DX11
		ImGui_ImplDX11_Init(DX11Resources::s_Device.Get(), DX11Resources::s_DeviceContext.Get());
	#endif // HIGHLO_API_DX11
	}

	void ShutdownImGui()
	{
		IMGUI_RENDER_API_IMPL_FN_NAME(Shutdown)();
		IMGUI_WINDOW_IMPL_FN_NAME(Shutdown)();
		ImGui::DestroyContext();
	}

	void BeginScene()
	{
		IMGUI_RENDER_API_IMPL_FN_NAME(NewFrame)();
		IMGUI_WINDOW_IMPL_FN_NAME(NewFrame)();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();

		if (s_ShouldDisplayDebugInformation)
			DisplayDebugInformation();
	}

	void EndScene()
	{
		ImGuiIO &io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)HLApplication::Get().GetWindow().GetWidth(), (float)HLApplication::Get().GetWindow().GetHeight());

		ImGui::Render();
		IMGUI_RENDER_API_IMPL_FN_NAME(RenderDrawData)(ImGui::GetDrawData());

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

	void BeginViewport(const HLString &title, const Ref<PopupMenu> &popupMenu)
	{
		ImGui::Begin(*title);

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

	void DrawHelpMarker(const HLString &description)
	{
		ImGui::TextDisabled("(?)");
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

	bool DrawInputText(const HLString &label, HLString &value)
	{
		bool modified = false;

		ImGui::Text(*label);
		ImGui::NextColumn();
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

	bool DrawCheckbox(const HLString &label, bool &value)
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

		if (ImGui::Checkbox(s_ImGuiIDBuffer, &value))
			modified = true;

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

			if (IsItemDisabled())
				ImGui::PopStyleVar();

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	bool DrawVec2(const HLString &label, glm::vec2 &values, float resetValue, float columnWidth)
	{
		bool modified = false;

		ImGui::PushID(*label);
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(*label);
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });

		if (ImGui::Button("X", buttonSize))
		{
			values.x = resetValue;
			modified = true;
		}

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		modified |= ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		if (ImGui::Button("Y", buttonSize))
		{
			values.y = resetValue;
			modified = true;
		}

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		modified |= ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "#.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();

		return modified;
	}

	bool DrawVec3(const HLString &label, glm::vec3 &values, float resetValue, float columnWidth)
	{
		bool modified = false;

		ImGui::PushID(*label);
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(*label);
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		if (ImGui::Button("X", buttonSize))
		{
			values.x = resetValue;
			modified = true;
		}

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		modified |= ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		if (ImGui::Button("Y", buttonSize))
		{
			values.y = resetValue;
			modified = true;
		}

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		modified |= ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		if (ImGui::Button("Z", buttonSize))
		{
			values.z = resetValue;
			modified = true;
		}

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		modified |= ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();

		return modified;
	}

	bool DrawVec4(const HLString &label, glm::vec4 &values, float resetValue, float columnWidth)
	{
		bool modified = false;

		ImGui::PushID(*label);
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(*label);
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		if (ImGui::Button("X", buttonSize))
		{
			values.x = resetValue;
			modified = true;
		}

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		modified |= ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		if (ImGui::Button("Y", buttonSize))
		{
			values.y = resetValue;
			modified = true;
		}

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		modified |= ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		if (ImGui::Button("Z", buttonSize))
		{
			values.z = resetValue;
			modified = true;
		}

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		modified |= ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		// TODO: edit these colors
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		if (ImGui::Button("W", buttonSize))
		{
			values.w = resetValue;
			modified = true;
		}

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		modified |= ImGui::DragFloat("##W", &values.w, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();

		return modified;
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

	bool DrawDropdown(const HLString &label, const std::vector<HLString> &options, int32 optionsCount, int32 *selected)
	{
		const char *current = options[*selected].C_Str();
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

	void Image(const Ref<Texture2D> &texture, const ImVec2 &size, const ImVec2 &uv0, const ImVec2 &uv1, const ImVec4 &tintColor, const ImVec4 &borderColor)
	{
	#ifdef HIGHLO_API_OPENGL
		Ref<OpenGLTexture2D> glTexture = texture.As<OpenGLTexture2D>();
		ImGui::Image((ImTextureID)(size_t)glTexture->GetRendererID(), size, uv0, uv1, tintColor, borderColor);
	#endif // HIGHLO_API_OPENGL
	}
	
	void Image(const Ref<Texture2D> &texture, uint32 layer, const ImVec2 &size, const ImVec2 &uv0, const ImVec2 &uv1, const ImVec4 &tintColor, const ImVec4 &borderColor)
	{
	#ifdef HIGHLO_API_OPENGL
		Ref<OpenGLTexture2D> glTexture = texture.As<OpenGLTexture2D>();
		ImGui::Image((ImTextureID)(size_t)glTexture->GetRendererID(), size, uv0, uv1, tintColor, borderColor);
	#endif // HIGHLO_API_OPENGL
	}
	
	void ImageMip(const Ref<Texture2D> &texture, uint32 mip, const ImVec2 &size, const ImVec2 &uv0, const ImVec2 &uv1, const ImVec4 &tintColor, const ImVec4 &borderColor)
	{
	#ifdef HIGHLO_API_OPENGL
		// TODO
	#endif // HIGHLO_API_OPENGL
	}
	
	bool ImageButton(const Ref<Texture2D> &texture, const ImVec2 &size, const ImVec2 &uv0, const ImVec2 &uv1, int32 framePadding, const ImVec4 &bgColor, const ImVec4 &tintColor)
	{
	#ifdef HIGHLO_API_OPENGL
		return false;
	#else
		return false;
	#endif // HIGHLO_API_OPENGL
	}
	
	bool ImageButton(const HLString &id, const Ref<Texture2D> &texture, const ImVec2 &size, const ImVec2 &uv0, const ImVec2 &uv1, int32 framePadding, const ImVec4 &bgColor, const ImVec4 &tintColor)
	{
	#ifdef HIGHLO_API_OPENGL
		if (!texture)
			return false;

		Ref<OpenGLTexture2D> glTexture = texture.As<OpenGLTexture2D>();
		return ImGui::ImageButton((ImTextureID)(size_t)glTexture->GetRendererID(), size, uv0, uv1, framePadding, bgColor, tintColor);
	#else
		return false;
	#endif // HIGHLO_API_OPENGL
	}
}

