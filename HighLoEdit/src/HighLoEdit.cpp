#include "HighLoEdit.h"

void HighLoEditor::OnInitialize()
{
	// Initialize panels
	m_Viewport.Initialize(GetWindow().GetWidth(), GetWindow().GetHeight());

	GetWindow().Maximize();
	UpdateWindowTitle("Untitled Scene");

	FileSystem::StartWatching();

	// Create FileMenu
	m_MenuBar = MenuBar::Create();
	Ref<FileMenu> fileMenu = FileMenu::Create("File");
	fileMenu->AddMenuItem("New Scene", "Strg+N", 1, [=]() { NewScene(); });
	fileMenu->AddMenuItem("Open Scene...", "Strg+O", 2, [=]() { OpenScene(); });
	fileMenu->AddMenuItem("Save Scene", "Strg+S", 3, [=]() { SaveScene(); });
	fileMenu->AddMenuItem("Save Scene as...", "Strg+Shift+S", 4, [=]() { SaveSceneAs(); });
	fileMenu->AddMenuSeperator();

	m_MenuBar->AddMenu(fileMenu);
	GetWindow().SetMenuBar(m_MenuBar);
}

void HighLoEditor::OnUpdate(Timestep timestep)
{
	m_Viewport.Render(timestep);
}

void HighLoEditor::OnShutdown()
{
	FileSystem::StopWatching();
}

void HighLoEditor::OnEvent(Event &e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<KeyPressedEvent>(HL_BIND_EVENT_FUNCTION(HighLoEditor::OnKeyPressedEvent));
	dispatcher.Dispatch<MouseButtonPressedEvent>(HL_BIND_EVENT_FUNCTION(HighLoEditor::OnMouseButtonPressedEvent));

	if (m_MenuBar)
		m_MenuBar->OnEvent(e);

	if (m_SceneSate == SceneState::Edit || m_SceneSate == SceneState::Simulate)
	{
		if (m_Viewport.IsMouseOverViewport())
			m_Viewport.OnEvent(e);
	}
	else if (m_SceneSate == SceneState::Play)
	{
		// TODO: call RuntimeScene onEvent
	}

}

void HighLoEditor::OnUIRender(Timestep timestep)
{
	static bool pOpen = true;
	ImGuiRenderer::StartWindow("RootWindow", pOpen, true);
	
	m_Viewport.RenderUI(timestep);
	GetWindow().SetMenuBar(m_MenuBar);

	ImGuiRenderer::EndWindow();
}

void HighLoEditor::OnResize(uint32 width, uint32 height)
{
	m_Viewport.OnResize(width, height);
}

void HighLoEditor::UpdateWindowTitle(const HLString &sceneName)
{
	HLString title = sceneName + " - HighLo Engine - Renderer: " + Renderer::GetCurrentRenderingAPI();
	GetWindow().SetTitle(title);
}

void HighLoEditor::NewScene()
{
	HL_TRACE("NewScene");
}

void HighLoEditor::OpenScene()
{
	HL_TRACE("Open last known scene");
	// Read Scene file at m_LastSceneFilePath
	OpenScene(m_LastSceneFilePath);
}

void HighLoEditor::OpenScene(const HLString &path)
{
	Ref<FileDialogue> fd = FileDialogue::Create();
	FileDialogueFilter filter;
	filter.AddFilter("High-Lo Scene file", "*.hl");
	fd->SetFilter(filter);
	HLString result = fd->SelectFile();

	if (!result.IsEmpty())
	{
		HL_TRACE("Open specific scene {0}", *path);
	}
}

void HighLoEditor::SaveScene()
{
	HL_TRACE("SaveScene");
}

void HighLoEditor::SaveSceneAs()
{
	HL_TRACE("SaveSceneAs");
}

bool HighLoEditor::OnKeyPressedEvent(const KeyPressedEvent &e)
{
	return false;
}

bool HighLoEditor::OnMouseButtonPressedEvent(const MouseButtonPressedEvent &e)
{
	return false;
}

