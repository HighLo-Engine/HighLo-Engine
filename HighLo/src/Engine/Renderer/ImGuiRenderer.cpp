#include "HighLoPch.h"
#include "ImGuiRenderer.h"

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
    std::shared_ptr<ImGuiTextBuffer> ImGuiRenderer::s_ImGuiTextBuffer = std::make_shared<ImGuiTextBuffer>();

    void ImGuiRenderer::Init(Window* window)
    {
        ImGui::CreateContext();

#ifdef HIGHLO_API_GLFW
        ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window->GetNativeHandle(), true);
#else
        ImGui_ImplWin32_Init(window->GetNativeHandle());
#endif // HIGHLO_API_DX11

#ifdef HIGHLO_API_OPENGL
        ImGui_ImplOpenGL3_Init("#version 130");
#endif // HIGHLO_API_OPENGL
#ifdef HIGHLO_API_DX11
        ImGui_ImplDX11_Init(DX11Resources::s_Device.Get(), DX11Resources::s_DeviceContext.Get());
#endif // HIGHLO_API_DX11

        ImGui::StyleColorsDark();
    }

    void ImGuiRenderer::Shutdown()
    {
        IMGUI_RENDER_API_IMPL_FN_NAME(Shutdown)();
        IMGUI_WINDOW_IMPL_FN_NAME(Shutdown)();
        ImGui::DestroyContext();
    }

    void ImGuiRenderer::BeginScene()
    {
        IMGUI_RENDER_API_IMPL_FN_NAME(NewFrame)();
        IMGUI_WINDOW_IMPL_FN_NAME(NewFrame)();
        ImGui::NewFrame();

        if (s_ShouldDisplayDebugInformation)
            DisplayDebugInformation();

        if (s_ShouldUseCustomConsole)
            DisplayCustomConsole();
    }

    void ImGuiRenderer::EndScene()
    {
        ImGui::Render();
        IMGUI_RENDER_API_IMPL_FN_NAME(RenderDrawData)(ImGui::GetDrawData());
    }

    void ImGuiRenderer::ShowDemoWindow()
    {
        ImGui::Begin("HighLo Demo Window");
        {
            static float f = 0.0f;
            static float col = 0.f;
            int show = 1;
            ImGui::Text(u8"Hello, world!");
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

        // TO-DO: Modify spdlog stdout channel to redirect to the ImGui console buffer.
    }

    void ImGuiRenderer::StartWindow(const HLString& title, uint32_t width, uint32_t height)
    {
        if (width != 0 && height != 0)
            ImGui::SetNextWindowSize(ImVec2((float)width, (float)height));

        ImGui::Begin(title.C_Str());
    }

    void ImGuiRenderer::EndWindow()
    {
        ImGui::End();
    }

    void ImGuiRenderer::BeginChild(const HLString& id, uint32_t width, uint32_t height)
    {
        ImGui::BeginChild(id.C_Str(), ImVec2((float)width, (float)height));
    }

    void ImGuiRenderer::EndChild()
    {
        ImGui::EndChild();
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

    void ImGuiRenderer::Text(const HLString& text)
    {
        ImGui::Text(text.C_Str());
    }

    void ImGuiRenderer::SliderInt(const HLString& text, int& val, int min, int max)
    {
        ImGui::SliderInt(text.C_Str(), &val, min, max);
    }

    void ImGuiRenderer::SliderFloat(const HLString& text, float& val, float min, float max)
    {
        ImGui::SliderFloat(text.C_Str(), &val, min, max);
    }

    bool ImGuiRenderer::Button(const HLString& text, float width, float height)
    {
        return ImGui::Button(text.C_Str(), ImVec2(width, height));
    }

    bool ImGuiRenderer::Combobox(const HLString& text, std::vector<HLString>& items, int& selected_index)
    {
        static auto vector_getter = [](void* vec, int idx, const char** out_text)
        {
            auto& vector = *static_cast<std::vector<HLString>*>(vec);
            if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
            *out_text = vector.at(idx).C_Str();
            return true;
        };

        return ImGui::ListBox(text.C_Str(), &selected_index, vector_getter, static_cast<void*>(&items), (int)items.size());
    }

    bool ImGuiRenderer::Checkbox(const HLString& text, bool& checked)
    {
        return ImGui::Checkbox(text.C_Str(), &checked);
    }

    void ImGuiRenderer::InputText(const HLString& label, char* text, size_t buffersize)
    {
        ImGui::InputText(label.C_Str(), &text[0], buffersize);
    }

    void ImGuiRenderer::ColorPicker(const HLString& label, glm::vec3& color)
    {
        ImGui::ColorPicker3(label.C_Str(), &color[0], ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip);
    }

    void ImGuiRenderer::ColorPicker(const HLString& label, glm::vec4& color)
    {
        ImGui::ColorPicker3(label.C_Str(), &color[0], ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip);
    }

    void ImGuiRenderer::AddToTextBuffer(const HLString& text)
    {
        if (s_ShouldDisplayDebugInformation)
            s_ImGuiTextBuffer->appendf("%s", text.C_Str());
    }

    void ImGuiRenderer::DisplayDebugInformation()
    {
        static std::vector<float> frames;

        auto fps = ImGui::GetIO().Framerate;
        if (frames.size() > 40) // Max seconds to show
        {
            for (size_t i = 1; i < frames.size(); i++)
                frames[i - 1] = frames[i];

            frames[frames.size() - 1] = fps;
        }
        else
            frames.push_back(fps);

        ImGui::SetNextWindowPos(ImVec2(10, 10));
        ImGui::Begin("FPS Graph", 0, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

        char fps_text[60];
        sprintf_s(fps_text, 60, "Frames: %f", fps);
        ImGui::Text(fps_text);

        char timestep_text[36];
        sprintf_s(timestep_text, 36, "Timestep: %f ms", Time::GetTimestep());
        ImGui::Text(timestep_text);

        ImGui::PlotHistogram("Framerate", &frames[0], (int)frames.size(), 0, NULL, 0.0f, 600, ImVec2(300, 100));
        ImGui::End();
    }

    void ImGuiRenderer::DisplayCustomConsole()
    {
        ImGui::SetNextWindowPos(ImVec2(10, (float)Window::Get().GetHeight() - 362));
        ImGui::SetNextWindowSize(ImVec2((float)Window::Get().GetWidth() * (2 / 5), 360));
        ImGui::Begin("Console");

        ImGui::TextUnformatted(s_ImGuiTextBuffer->Buf.begin());
        ImGui::SetScrollHereY(1.0f);

        ImGui::End();
    }
}
