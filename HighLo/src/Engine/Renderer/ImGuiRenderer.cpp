#include "HighLoPch.h"
#include "ImGuiRenderer.h"

#include "Engine/Application/HLApplication.h"
#include "Engine/Window/Window.h"
#include "Engine/Core/HLTime.h"
#include "Engine/ImGui/imgui.h"

#ifdef HIGHLO_API_OPENGL
#include "Engine/ImGui/imgui_impl_opengl3.h"
#define IMGUI_RENDER_API_IMPL_FN_NAME(fn) ImGui_ImplOpenGL3_##fn
#endif // HIGHLO_API_OPENGL
#ifdef HIGHLO_API_DX11
#include "Engine/Platform/DX11/DX11Resources.h"
#include "Engine/ImGui/imgui_impl_dx11.h"
#define IMGUI_RENDER_API_IMPL_FN_NAME(fn) ImGui_ImplDX11_##fn
#endif // HIGHLO_API_DX11

#ifdef HIGHLO_API_GLFW
#include <GLFW/glfw3.h>
#include "Engine/ImGui/imgui_impl_glfw.h"
#define IMGUI_WINDOW_IMPL_FN_NAME(fn) ImGui_ImplGlfw_##fn
#else
#include "Engine/ImGui/imgui_impl_win32.h"
#define IMGUI_WINDOW_IMPL_FN_NAME(fn) ImGui_ImplWin32_##fn
#endif // HIGHLO_API_DX11

namespace highlo
{
    bool ImGuiRenderer::s_ShouldDisplayDebugInformation = false;
    bool ImGuiRenderer::s_ShouldUseCustomConsole = false;
    bool ImGuiRenderer::s_CanDraw = false;
    std::shared_ptr<ImGuiTextBuffer> ImGuiRenderer::s_ImGuiTextBuffer = std::make_shared<ImGuiTextBuffer>();

    static int32 s_CheckboxCount = 0;

    void ImGuiRenderer::Init(Window* window, ImGuiWindowStyle windowStyle)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepage controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Enable docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;     // Enable Multi-Viewport / Platform Windows
        io.ConfigViewportsNoAutoMerge = true;
        io.ConfigViewportsNoTaskBarIcon = true;

        ImFont *font = io.Fonts->AddFontFromFileTTF("assets/fonts/BarlowSemiCondensedFontFamily/BarlowSemiCondensed-Bold.ttf", 18.0f);
        io.FontDefault = io.Fonts->Fonts.back();

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
        ImGui_ImplWin32_Init(window->GetNativeHandle());
#endif // HIGHLO_API_DX11

#ifdef HIGHLO_API_OPENGL
        ImGui_ImplOpenGL3_Init("#version 410");
#endif // HIGHLO_API_OPENGL

#ifdef HIGHLO_API_DX11
        ImGui_ImplDX11_Init(DX11Resources::s_Device.Get(), DX11Resources::s_DeviceContext.Get());
#endif // HIGHLO_API_DX11
    }

    void ImGuiRenderer::Shutdown()
    {
        IMGUI_RENDER_API_IMPL_FN_NAME(Shutdown)();
        IMGUI_WINDOW_IMPL_FN_NAME(Shutdown)();
        ImGui::DestroyContext();
    }

    void ImGuiRenderer::StartScene()
    {
        IMGUI_RENDER_API_IMPL_FN_NAME(NewFrame)();
        IMGUI_WINDOW_IMPL_FN_NAME(NewFrame)();
        ImGui::NewFrame();

        s_CanDraw = true;

        if (s_ShouldDisplayDebugInformation)
            DisplayDebugInformation();

        if (s_ShouldUseCustomConsole)
            DisplayCustomConsole();
    }

    void ImGuiRenderer::EndScene()
    {
        s_CanDraw = false;

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

    void ImGuiRenderer::ShowDemoWindow()
    {
        HL_ASSERT(s_CanDraw, "Unable to draw into a ImGui Window, maybe you forgot to call ImGuiRenderer::BeginScene() ?");

        ImGui::Begin("HighLo Demo Window");
        {
            static float f = 0.0f;
            static float col = 0.0f;
            int32 show = 1;
            ImGui::Text("Hello, world!");
            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
            ImGui::ColorEdit3("clear color", (float*)&col);
            if (ImGui::Button("Test Window")) show ^= 1;
            if (ImGui::Button("Another Window")) show ^= 1;
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }

        ImVec2 v = ImGui::GetWindowSize();
        ImGui::Text("%f %f", v.x, v.y);
        ImGui::End();
    }

    void ImGuiRenderer::ShouldDisplayDebugInformation(bool show)
    {
        s_ShouldDisplayDebugInformation = show;
    }

    void ImGuiRenderer::ShouldUseCustomConsole(bool use)
    {
        s_ShouldUseCustomConsole = use;

        // TODO: Modify spdlog stdout channel to redirect to the ImGui console buffer.
    }

    void ImGuiRenderer::StartWindow(const HLString &title, bool pOpen, bool fullscreen)
    {
        static ImGuiDockNodeFlags optFlags = ImGuiDockNodeFlags_None;
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

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
    }

    void ImGuiRenderer::EndWindow()
    {
        ImGui::End();
    }

    void ImGuiRenderer::StartViewport(const HLString &title)
    {
        ImGui::Begin(*title);
    }

    void ImGuiRenderer::EndViewport()
    {
        ImGui::End();
    }

    void ImGuiRenderer::StartChild(const HLString &id, uint32 width, uint32 height)
    {
        ImGui::BeginChild(*id, ImVec2((float)width, (float)height));
    }

    void ImGuiRenderer::EndChild()
    {
        ImGui::EndChild();
    }

    bool ImGuiRenderer::StartTreeNode(const HLString &name, bool defaultOpen)
    {
        ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;
        if (defaultOpen)
            treeNodeFlags |= ImGuiTreeNodeFlags_DefaultOpen;

        return ImGui::TreeNodeEx(*name, treeNodeFlags);
    }

    void ImGuiRenderer::EndTreeNode()
    {
        ImGui::TreePop();
    }

    void ImGuiRenderer::StartCheckboxGroup(const HLString &label)
    {
        ImGui::Text(*label);
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);
    }

    void ImGuiRenderer::EndCheckboxGroup()
    {
        ImGui::PopItemWidth();
        ImGui::NextColumn();
        s_CheckboxCount = 0;
    }

    void ImGuiRenderer::StartPopupModal(const HLString &text)
    {
    }

    void ImGuiRenderer::EndPopupModal()
    {
    }

    void ImGuiRenderer::NewLine()
    {
        ImGui::NewLine();
    }

    void ImGuiRenderer::Separator()
    {
        ImGui::Separator();
    }

    void ImGuiRenderer::SameLine()
    {
        ImGui::SameLine();
    }

    void ImGuiRenderer::Text(const HLString &text)
    {
        HL_ASSERT(s_CanDraw, "Unable to draw into a ImGui Window, maybe you forgot to call ImGuiRenderer::BeginScene() ?");
        ImGui::Text(*text);
    }

    void ImGuiRenderer::SliderInt(const HLString &text, int32 &value, int32 min, int32 max)
    {
        HL_ASSERT(s_CanDraw, "Unable to draw into a ImGui Window, maybe you forgot to call ImGuiRenderer::BeginScene() ?");
        ImGui::SliderInt(*text, &value, min, max);
    }

    void ImGuiRenderer::SliderFloat(const HLString &text, float &value, float min, float max)
    {
        HL_ASSERT(s_CanDraw, "Unable to draw into a ImGui Window, maybe you forgot to call ImGuiRenderer::BeginScene() ?");
        ImGui::SliderFloat(*text, &value, min, max);
    }

    bool ImGuiRenderer::Button(const HLString &text, float width, float height)
    {
        HL_ASSERT(s_CanDraw, "Unable to draw into a ImGui Window, maybe you forgot to call ImGuiRenderer::BeginScene() ?");
        return ImGui::Button(*text, ImVec2(width, height));
    }

    bool ImGuiRenderer::Combobox(const HLString &text, std::vector<HLString> &items, int32 &selected_index)
    {
        HL_ASSERT(s_CanDraw, "Unable to draw into a ImGui Window, maybe you forgot to call ImGuiRenderer::BeginScene() ?");
        static auto vector_getter = [](void* vec, int32 idx, const char** out_text)
        {
            auto &vector = *static_cast<std::vector<HLString>*>(vec);
            if (idx < 0 || idx >= static_cast<int32>(vector.size())) { return false; }
            *out_text = *vector.at(idx);
            return true;
        };

        return ImGui::ListBox(*text, &selected_index, vector_getter, static_cast<void*>(&items), (int32)items.size());
    }

    bool ImGuiRenderer::Checkbox(const HLString &text, bool &checked)
    {
        HL_ASSERT(s_CanDraw, "Unable to draw into a ImGui Window, maybe you forgot to call ImGuiRenderer::BeginScene() ?");
        return ImGui::Checkbox(*text, &checked);
    }

    void ImGuiRenderer::InputText(const HLString &label, const HLString &text)
    {
        HL_ASSERT(s_CanDraw, "Unable to draw into a ImGui Window, maybe you forgot to call ImGuiRenderer::BeginScene() ?");
        ImGui::InputText(*label, (char*)&(*text)[0], text.Length());
    }

    void ImGuiRenderer::ColorPicker(const HLString& label, glm::vec3& color)
    {
        ImGui::ColorPicker3(label.C_Str(), &color[0], ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip);
    }

    void ImGuiRenderer::ColorPicker(const HLString& label, glm::vec4& color)
    {
        ImGui::ColorPicker3(label.C_Str(), &color[0], ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip);
    }

    bool ImGuiRenderer::Property(const HLString &name, bool &value)
    {
        HL_ASSERT(s_CanDraw, "Unable to draw into a ImGui Window, maybe you forgot to call ImGuiRenderer::BeginScene() ?");

        ImGui::Text(*name);
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        bool result = ImGui::Checkbox(*GenerateID(name), &value);

        ImGui::PopItemWidth();
        ImGui::NextColumn();

        return result;
    }

    bool ImGuiRenderer::Property(const HLString &name, float &value, float min, float max, PropertyFlag flags)
    {
        HL_ASSERT(s_CanDraw, "Unable to draw into a ImGui Window, maybe you forgot to call ImGuiRenderer::BeginScene() ?");

        ImGui::Text(*name);
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        bool changed = false;
        if (flags == PropertyFlag::Slider)
            changed = ImGui::SliderFloat(*GenerateID(name), &value, min, max);
        else
            changed = ImGui::DragFloat(*GenerateID(name), &value, 0.1f, min, max);

        ImGui::PopItemWidth();
        ImGui::NextColumn();

        return changed;
    }

    bool ImGuiRenderer::Property(const HLString &name, glm::vec2 &value, PropertyFlag flags)
    {
        return Property(name, value, -1.0, 1.0f, flags);
    }

    bool ImGuiRenderer::Property(const HLString &name, glm::vec2 &value, float min, float max, PropertyFlag flags)
    {
        HL_ASSERT(s_CanDraw, "Unable to draw into a ImGui Window, maybe you forgot to call ImGuiRenderer::BeginScene() ?");

        ImGui::Text(*name);
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        bool changed = false;
        if (flags == PropertyFlag::Slider)
            changed = ImGui::SliderFloat2(*GenerateID(name), &value[0], min, max);
        else
            changed = ImGui::DragFloat2(*GenerateID(name), &value[0], 0.1f, min, max);

        ImGui::PopItemWidth();
        ImGui::NextColumn();

        return changed;
    }

    bool ImGuiRenderer::Property(const HLString &name, glm::vec3 &value, PropertyFlag flags)
    {
        return Property(name, value, -1.0f, 1.0f, flags);
    }

    bool ImGuiRenderer::Property(const HLString &name, glm::vec3 &value, float min, float max, PropertyFlag flags)
    {
        HL_ASSERT(s_CanDraw, "Unable to draw into a ImGui Window, maybe you forgot to call ImGuiRenderer::BeginScene() ?");

        ImGui::Text(*name);
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        bool changed = false;
        if ((int32) flags & (int32) PropertyFlag::Color)
            changed = ImGui::ColorPicker3(*GenerateID(name), &value[0], ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip);
        else if (flags == PropertyFlag::Slider)
            changed = ImGui::SliderFloat3(*GenerateID(name), &value[0], min, max);
        else
            changed = ImGui::DragFloat3(*GenerateID(name), &value[0], 0.1f, min, max);

        ImGui::PopItemWidth();
        ImGui::NextColumn();

        return changed;
    }

    bool ImGuiRenderer::Property(const HLString &name, glm::vec4 &value, PropertyFlag flags)
    {
        return Property(name, value, -1.0f, 1.0f, flags);
    }

    bool ImGuiRenderer::Property(const HLString &name, glm::vec4 &value, float min, float max, PropertyFlag flags)
    {
        HL_ASSERT(s_CanDraw, "Unable to draw into a ImGui Window, maybe you forgot to call ImGuiRenderer::BeginScene() ?");

        ImGui::Text(*name);
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        bool changed = false;
        if ((int32) flags & (int32) PropertyFlag::Color)
            changed = ImGui::ColorPicker4(*GenerateID(name), &value[0], ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip);
        else if (flags == PropertyFlag::Slider)
            changed = ImGui::SliderFloat4(*GenerateID(name), &value[0], min, max);
        else
            changed = ImGui::DragFloat4(*GenerateID(name), &value[0], 0.1f, min, max);

        ImGui::PopItemWidth();
        ImGui::NextColumn();

        return changed;
    }

    void ImGuiRenderer::DrawFramebuffer(const Ref<Framebuffer> &framebuffer, const glm::vec2 &size, const glm::vec2 &uv0, const glm::vec2 &uv1, const glm::vec4 &tintColor, const glm::vec4 &borderColor)
    {
        HL_ASSERT(s_CanDraw, "");
        ImGui::Image((ImTextureID)framebuffer->GetImage()->GetRendererID(), ImVec2(size.x, size.y), ImVec2(uv0.x, uv0.y), ImVec2(uv1.x, uv1.y), ImVec4(tintColor.x, tintColor.y, tintColor.z, tintColor.w), ImVec4(borderColor.x, borderColor.y, borderColor.z, borderColor.w));
    }

    void ImGuiRenderer::DrawMenu(const Ref<MenuBar> &menubar)
    {
        HL_ASSERT(s_CanDraw, "");
        std::vector<Ref<FileMenu>> fileMenus = menubar->GetMenus();
        if (ImGui::BeginMenuBar())
        {
            for (int32 i = 0; i < fileMenus.size(); ++i)
            {
                Ref<FileMenu> menu = fileMenus[i];
                std::vector<MenuItem> menuItems = menu->GetMenuItems();
                if (ImGui::BeginMenu(*menu->GetName()))
                {
                    for (int32 j = 0; j < menuItems.size(); ++j)
                    {
                        MenuItem currentItem = menuItems[j];
                        if (currentItem.Seperator)
                        {
                            ImGui::Separator();
                            continue;
                        }

                        if (ImGui::MenuItem(*currentItem.Name, *currentItem.Shortcut))
                            currentItem.Callback();
                    }

                    ImGui::EndMenu();
                }
            }

            ImGui::EndMenuBar();
        }
    }

    bool ImGuiRenderer::IsWindowHovered()
    {
        return ImGui::IsWindowHovered();
    }

    bool ImGuiRenderer::IsWindowFocused()
    {
        return ImGui::IsWindowFocused();
    }

    bool ImGuiRenderer::CanDraw()
    {
        return s_CanDraw;
    }

    glm::vec2 ImGuiRenderer::GetCursorPosition()
    {
        ImVec2 cursorPos = ImGui::GetCursorPos();
        return { cursorPos.x, cursorPos.y };
    }

    glm::vec2 ImGuiRenderer::GetMousePosition()
    {
        ImVec2 pos = ImGui::GetMousePos();
        return { pos.x, pos.y };
    }

    glm::vec2 ImGuiRenderer::GetContentRegion()
    {
        ImVec2 region = ImGui::GetContentRegionAvail();
        return { region.x, region.y };
    }

    glm::vec2 ImGuiRenderer::GetWindowSize()
    {
        ImVec2 size = ImGui::GetWindowSize();
        return { size.x, size.y };
    }

    glm::vec2 ImGuiRenderer::GetWindowPos()
    {
        ImVec2 windowPos = ImGui::GetWindowPos();
        return { windowPos.x, windowPos.y };
    }

    bool ImGuiRenderer::IsMouseHoveringRect(const glm::vec2 &min, const glm::vec2 &max)
    {
        return ImGui::IsMouseHoveringRect({ min.x, min.y }, { max.x, max.y });
    }

    void ImGuiRenderer::OnWindowResize(uint32 width, uint32 height)
    {
        
    }

    void ImGuiRenderer::DisplayDebugInformation()
    {
        HL_ASSERT(s_CanDraw, "Unable to draw into a ImGui Window, maybe you forgot to call ImGuiRenderer::BeginScene() ?");
        static std::vector<float> frames;

        auto fps = ImGui::GetIO().Framerate;
        if (frames.size() > 40) // Max seconds to show
        {
            for (size_t i = 1; i < frames.size(); i++)
                frames[i - 1] = frames[i];

            frames[frames.size() - 1] = fps;
        }
        else
        {
            frames.push_back(fps);
        }

        ImGui::Begin("FPS Graph", 0, ImGuiWindowFlags_NoBackground);

        char fps_text[60];
        sprintf_s(fps_text, 60, "Frames: %f", fps);
        ImGui::Text(fps_text);

        char timestep_text[36];
        sprintf_s(timestep_text, 36, "Timestep: %f ms", Time::GetTimestep());
        ImGui::Text(timestep_text);

        ImGui::PlotHistogram("Framerate", &frames[0], (int32)frames.size(), 0, NULL, 0.0f, 600, ImVec2(300, 100));
        ImGui::End();
    }

    void ImGuiRenderer::DisplayCustomConsole()
    {
        HL_ASSERT(s_CanDraw, "Unable to draw into a ImGui Window, maybe you forgot to call ImGuiRenderer::BeginScene() ?");

        ImGui::SetNextWindowPos(ImVec2(10, (float)Window::Get().GetHeight() - 362));
        ImGui::SetNextWindowSize(ImVec2((float)Window::Get().GetWidth() * (2 / 5), 360));
        ImGui::Begin("Console");

        ImGui::TextUnformatted(s_ImGuiTextBuffer->Buf.begin());
        ImGui::SetScrollHereY(1.0f);

        ImGui::End();
    }

    HLString ImGuiRenderer::GenerateID(const HLString &name)
    {
        HLString id = "##" + name;
        return id;
    }

    void ImGuiRenderer::SetDarkThemeColors()
    {
        auto &colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

        // Headers
        colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Buttons
        colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Frame BG
        colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Tabs
        colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
        colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
        colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

        // Title
        colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Resize Grip
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.91f, 0.91f, 0.91f, 0.25f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.81f, 0.81f, 0.81f, 0.67f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.46f, 0.46f, 0.46f, 0.95f);

        // Scrollbar
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.0f);

        // Check Mark
        colors[ImGuiCol_CheckMark] = ImVec4(0.94f, 0.94f, 0.94f, 1.0f);

        // Slider
        colors[ImGuiCol_SliderGrab] = ImVec4(0.51f, 0.51f, 0.51f, 0.7f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.66f, 0.66f, 0.66f, 1.0f);
    }
    
    void ImGuiRenderer::SetLightThemeColors()
    {
        auto &colors = ImGui::GetStyle().Colors;

    }
}
