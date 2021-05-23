#include "HighLoEdit.h"

namespace editorutils
{
	static int32 ConvertToImGuizmoType(HighLoEditor::GizmoType type)
	{
		switch (type)
		{
			case HighLoEditor::GizmoType::None:			return -1;
			case HighLoEditor::GizmoType::Translate:	return 0;
			case HighLoEditor::GizmoType::Rotate:		return 1;
			case HighLoEditor::GizmoType::Scale:		return 2;
		}

		return -1;
	}
}

void HighLoEditor::OnInitialize()
{
	uint32 width = GetWindow().GetWidth();
	uint32 height = GetWindow().GetHeight();

	// Initialize panels
	m_Viewport = MakeUniqueRef<ViewportPanel>();
	m_Viewport->Initialize(width, height);

	m_Assets = MakeUniqueRef<AssetsPanel>();
	m_Assets->Initialize(width, height);

	m_SceneHierarchy = MakeUniqueRef<SceneHierarchyPanel>(m_EditorScene);
	m_SceneHierarchy->Initialize(width, height);
	m_SceneHierarchy->SetSelectionChangedCallback(HL_BIND_EVENT_FUNCTION(HighLoEditor::SelectEntity));
	m_SceneHierarchy->SetEntityDeletedCallback(HL_BIND_EVENT_FUNCTION(HighLoEditor::OnEntityDeleted));

	m_ObjectProperties = MakeUniqueRef<ObjectPropertiesPanel>();
	m_ObjectProperties->Initialize(width, height);

	m_SettingsViewer = MakeUniqueRef<SettingsViewerPanel>();
	m_SettingsViewer->Initialize(width, height);

	m_ModelViewer = MakeUniqueRef<ModelViewerPanel>();
	m_ModelViewer->Initialize(width, height);

	m_MaterialViewer = MakeUniqueRef<MaterialViewerPanel>();
	m_MaterialViewer->Initialize(width, height);

	GetWindow().Maximize();
	GetWindow().SetWindowIcon("assets/textures/HighLoEngine.png", true);
	UpdateWindowTitle("Untitled Scene");

	FileSystem::StartWatching();

	// Create FileMenu
	m_MenuBar = MenuBar::Create();

	Ref<FileMenu> importMenu = FileMenu::Create("Import");
	importMenu->AddMenuItem("Import .obj", "", 42, [=]() { HL_TRACE("Importing obj"); });
	importMenu->AddMenuItem("Import .fbx", "", 43, [=]() { HL_TRACE("Importing fbx"); });
	importMenu->AddMenuItem("Import .stl", "", 44, [=]() { HL_TRACE("Importing stl"); });
	importMenu->AddMenuItem("Import .3ds", "", 45, [=]() { HL_TRACE("Importing 3ds"); });
	importMenu->AddMenuItem("Import .c4d", "", 46, [=]() { HL_TRACE("Importing c4d"); });
	importMenu->AddMenuItem("Import .mb", "", 47, [=]() { HL_TRACE("Importing mb"); });
	Ref<FileMenu> exportMenu = FileMenu::Create("Export");
	exportMenu->AddMenuItem("Export .obj", "", 48, [=]() { HL_TRACE("Exporting obj"); });
	exportMenu->AddMenuItem("Export .fbx", "", 49, [=]() { HL_TRACE("Exporting fbx"); });
	exportMenu->AddMenuItem("Export .stl", "", 50, [=]() { HL_TRACE("Exporting stl"); });
	exportMenu->AddSeparator();
	exportMenu->AddMenuItem("Export .mp4", "", 51, [=]() { HL_TRACE("Exporting mp4"); });
	exportMenu->AddMenuItem("Export .avi", "", 52, [=]() { HL_TRACE("Exporting avi"); });
	exportMenu->AddMenuItem("Export .mov", "", 53, [=]() { HL_TRACE("Exporting mov"); });

	Ref<FileMenu> fileMenu = FileMenu::Create("File");
	fileMenu->AddMenuItem("New Scene", "Strg+N", 1, [=]() { NewScene(); });
	fileMenu->AddMenuItem("Open Scene...", "Strg+O", 2, [=]() { OpenScene(); });
	fileMenu->AddSeparator();
	fileMenu->AddMenuItem("Save Scene", "Strg+S", 3, [=]() { SaveScene(); }, false);
	fileMenu->AddMenuItem("Save Scene as...", "Strg+Shift+S", 4, [=]() { SaveSceneAs(); });
	fileMenu->AddSeparator();
	fileMenu->AddMenuItem("Settings", "", 5, [=]() { HL_TRACE("Settings..."); });
	fileMenu->AddSeparator();
	fileMenu->AddSubMenu(importMenu);
	fileMenu->AddSubMenu(exportMenu);
	fileMenu->AddSeparator();
	fileMenu->AddMenuItem("Quit", "Strg+Shift+Q", 6, [=]() { Close(); });
	
	Ref<FileMenu> editMenu = FileMenu::Create("Edit");
	editMenu->AddMenuItem("Undo", "Strg+Z", 7, [=]() { HL_TRACE("Undo..."); });
	editMenu->AddMenuItem("Redo", "Strg+Y", 8, [=]() { HL_TRACE("Redo..."); });
	editMenu->AddSeparator();

	Ref<FileMenu> windowMenu = FileMenu::Create("Window");
	windowMenu->AddMenuItem("Set Dark Theme", "", 9, [=]() { ImGuiRenderer::SetDarkThemeColors(); });
	windowMenu->AddMenuItem("Set Light Theme", "", 10, [=]() { ImGuiRenderer::SetLightThemeColors(); });

	Ref<FileMenu> helpMenu = FileMenu::Create("Help");
	helpMenu->AddMenuItem("About HighLo", "", 11, [=]() { VirtualFileSystem::Get()->OpenInBrowser("https://www.highlo-engine.com"); });
	helpMenu->AddMenuItem("Documentation", "", 12, [=]() { VirtualFileSystem::Get()->OpenInBrowser("https://www.highlo-engine.com"); });

	m_MenuBar->AddMenu(fileMenu);
	m_MenuBar->AddMenu(editMenu);
	m_MenuBar->AddMenu(windowMenu);
	m_MenuBar->AddMenu(helpMenu);
	GetWindow().SetMenuBar(m_MenuBar);
}

void HighLoEditor::OnUpdate(Timestep timestep)
{
	m_Viewport->Update(timestep);
	m_Assets->Update(timestep);
	m_SceneHierarchy->Update(timestep);
	m_ObjectProperties->Update(timestep);

	m_ModelViewer->Update(timestep);
	m_MaterialViewer->Update(timestep);
	m_SettingsViewer->Update(timestep);

	auto [x, y] = m_Viewport->GetMouseViewportSpace();
	switch (m_SceneState)
	{
		case SceneState::Edit:
		{
			break;
		}

		case SceneState::Play:
		{
			break;
		}

		case SceneState::Pause:
		{
			break;
		}

		case SceneState::Simulate:
		{
			break;
		}
	}
}

void HighLoEditor::OnShutdown()
{
	FileSystem::StopWatching();

	m_MaterialViewer->Destroy();
	m_ModelViewer->Destroy();
	m_SettingsViewer->Destroy();
	m_ObjectProperties->Destroy();
	m_SceneHierarchy->Destroy();
	m_Assets->Destroy();
	m_Viewport->Destroy();
}

void HighLoEditor::OnEvent(Event &e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<KeyPressedEvent>(HL_BIND_EVENT_FUNCTION(HighLoEditor::OnKeyPressedEvent));
	dispatcher.Dispatch<MouseButtonPressedEvent>(HL_BIND_EVENT_FUNCTION(HighLoEditor::OnMouseButtonPressedEvent));

	if (m_MenuBar)
		m_MenuBar->OnEvent(e);

	m_SceneHierarchy->OnEvent(e);
	m_Assets->OnEvent(e);
	m_ObjectProperties->OnEvent(e);

	m_ModelViewer->OnEvent(e);
	m_MaterialViewer->OnEvent(e);
	m_SettingsViewer->OnEvent(e);

	if (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate)
	{
		if (m_Viewport->IsMouseOver())
			m_Viewport->OnEvent(e);
	}
	else if (m_SceneState == SceneState::Play)
	{
		// TODO: call RuntimeScene onEvent
	}
}

void HighLoEditor::OnUIRender(Timestep timestep)
{
	ImGuiRenderer::StartWindow("RootWindow", true, true);
	
	m_Viewport->Render(timestep);
	m_Assets->Render(timestep);
	m_SceneHierarchy->Render(timestep);
	m_ObjectProperties->Render(timestep);
	m_ModelViewer->Render(timestep);
	m_MaterialViewer->Render(timestep);
	m_SettingsViewer->Render(timestep);

	ImGuiRenderer::EndWindow();
}

void HighLoEditor::OnResize(uint32 width, uint32 height)
{
	m_Viewport->OnResize(width, height);
	m_Assets->OnResize(width, height);
	m_SceneHierarchy->OnResize(width, height);
	m_ObjectProperties->OnResize(width, height);
	m_ModelViewer->OnResize(width, height);
	m_MaterialViewer->OnResize(width, height);
	m_SettingsViewer->OnResize(width, height);
}

void HighLoEditor::SelectEntity(Entity entity)
{
	if (!entity.ID)
		return;

	SelectedMesh selection;
	//if (entity.HasComponent<MeshComponent>())
	//{
	//	auto &meshComp = entity.GetComponent<MeshComponent>();
	//	if (meshComp.Mesh && meshComp.Mesh->Type == AssetType::Mesh)
	//	{
	//		selection.m_Mesh = &meshComp.Mesh->GetSubmeshes()[0];
	//	}
	//}

	selection.m_Entity = entity;
	m_SelectionContext.clear();
	m_SelectionContext.push_back(selection);

	if (m_SceneState == SceneState::Edit)
	{
	//	m_EditorScene->SetSelectedEntity(entity);
		m_CurrentScene = m_EditorScene;
	}
	else if (m_SceneState == SceneState::Simulate)
	{
	//	m_EditorScene->SetSelectedEntity(entity);
		m_CurrentScene = m_SimulationScene;
	}

}

void HighLoEditor::UpdateWindowTitle(const HLString &sceneName)
{
	HLString title = sceneName + " - HighLo Engine - Renderer: " + Renderer::GetCurrentRenderingAPI();
	GetWindow().SetTitle(title);
}

void HighLoEditor::NewScene()
{
	HL_TRACE("NewScene");
	UpdateWindowTitle("Untitled scene");
}

void HighLoEditor::OpenScene()
{
	Ref<FileDialogue> fd = FileDialogue::Create();
	FileDialogueFilter filter;
	filter.AddFilter("High-Lo Scene file", "*.hl");
	fd->SetFilter(filter);
	HLString result = fd->SelectFile();

	if (!result.IsEmpty())
	{
		OpenScene(m_LastSceneFilePath);
	}
}

void HighLoEditor::OpenScene(const HLString &path)
{
	m_LastSceneFilePath = path;
	HL_TRACE("Open scene {0}", *path);

	// Deserialize scene
}

void HighLoEditor::SaveScene()
{
	if (!m_LastSceneFilePath.IsEmpty())
	{
		// Serialize scene
	}
	else
	{
		SaveSceneAs();
	}
}

void HighLoEditor::SaveSceneAs()
{
	HL_TRACE("SaveSceneAs");
	Ref<FileDialogue> fd = FileDialogue::Create();
	FileDialogueFilter filter;
	filter.AddFilter("High-Lo Scene file", "*.hl");
	fd->SetFilter(filter);
	HLString result = fd->SaveFile();

	if (!result.IsEmpty())
	{
		// Serialize scene
	}
}

bool HighLoEditor::OnKeyPressedEvent(const KeyPressedEvent &e)
{
	if (m_Viewport->IsMouseOver())
	{
		switch (e.GetKeyCode())
		{
			case HL_KEY_F:
			{
				if (m_SelectionContext.size() == 0)
					break;

				Entity selectedEntity = m_SelectionContext[0].m_Entity;
				m_Viewport->Focus(selectedEntity._TransformComponent->Transform.GetPosition());
				break;
			}

			case HL_KEY_Q:
			{
				m_GizmoType = GizmoType::None;
				break;
			}

			case HL_KEY_W:
			{
				m_GizmoType = GizmoType::Translate;
				break;
			}

			case HL_KEY_E:
			{
				m_GizmoType = GizmoType::Rotate;
				break;
			}

			case HL_KEY_R:
			{
				m_GizmoType = GizmoType::Scale;
				break;
			}
		}
	}

	if (Input::IsKeyPressed(HL_KEY_LEFT_CONTROL))
	{
		if (Input::IsKeyPressed(HL_KEY_LEFT_SHIFT))
		{
			switch (e.GetKeyCode())
			{
				case HL_KEY_S:
				{
					SaveSceneAs();
					break;
				}

				case HL_KEY_Q:
				{
					Close();
					break;
				}
			}

			return false;
		}

		switch (e.GetKeyCode())
		{
			case HL_KEY_N:
			{
				NewScene();
				break;
			}

			case HL_KEY_O:
			{
				OpenScene();
				break;
			}

			case HL_KEY_S:
			{
				SaveScene();
				break;
			}

			case HL_KEY_Z:
			{
				HL_TRACE("Undo...");
				break;
			}

			case HL_KEY_Y:
			{
				HL_TRACE("Redo...");
				break;
			}

			case HL_KEY_D:
			{
				// duplicate selected entity
				if (m_SelectionContext.size() == 0)
					break;

				Entity selectedEntity = m_SelectionContext[0].m_Entity;
				break;
			}
		}
	}

	switch (e.GetKeyCode())
	{
		case HL_KEY_ESCAPE:
		{
			// Clear selection
			break;
		}

		case HL_KEY_DELETE:
		{
			// Delete selected entity
			break;
		}
	}

	return false;
}

bool HighLoEditor::OnMouseButtonPressedEvent(const MouseButtonPressedEvent &e)
{
	auto [mx, my] = Input::GetMousePosition();
	if (e.GetMouseButton() == HL_MOUSE_BUTTON_LEFT
		&& m_Viewport->IsMouseOver()
		&& !Input::IsKeyPressed(HL_KEY_LEFT_ALT)
		&& !ImGuiRenderer::IsMouseOverGizmo()
		&& m_SceneState != SceneState::Play)
	{
		auto [mouseX, mouseY] = m_Viewport->GetMouseViewportSpace();
		if (mouseX > -1.0f && mouseX < 1.0f && mouseY > -1.0f && mouseY < 1.0f)
		{
			auto [origin, direction] = m_Viewport->CastRay(mouseX, mouseY);
			m_SelectionContext.clear();
		//	m_CurrentScene->SetSelectedEntity({});

		//	auto meshEntities = m_CurrentScene->GetAllEntitiesWith<MeshComponent>();
		// TODO
		}
	}
	return false;
}

void HighLoEditor::OnSelected(const SelectedMesh &selectionContext)
{
	//m_SceneHierarchy->SetSelected(selectionContext.m_Entity);
	//m_CurrentScene->SetSelectedEntity(selectionContext.m_Entity);
}

void HighLoEditor::OnEntityDeleted(Entity e)
{
	if (m_SelectionContext.size() > 0 && m_SelectionContext[0].m_Entity == e)
	{
		m_SelectionContext.clear();
	//	m_EditorScene->SetSelectedEntity({});
	}
}

void HighLoEditor::OnScenePlay()
{
	m_SelectionContext.clear();

	m_SceneState = SceneState::Play;

	m_RuntimeScene = Ref<Scene>::Create();
	
	//m_EditorScene->CopyTo(m_RuntimeScene);
	//m_RuntimeScene->OnRuntimeStart();
	//m_SceneHierarchy->SetContext(m_RuntimeScene);
	
	m_CurrentScene = m_RuntimeScene;
}

void HighLoEditor::OnSceneStop()
{
	//m_RuntimeScene->OnRuntimeStop();
	m_SceneState = SceneState::Edit;

	// Unload runtime scene
	m_RuntimeScene = nullptr;

	m_SelectionContext.clear();
	//m_SceneHierarchy->SetContext(m_EditorScene);
	m_CurrentScene = m_EditorScene;
}

void HighLoEditor::OnSceneStartSimulation()
{
	m_SelectionContext.clear();

	m_SceneState = SceneState::Simulate;

	m_SimulationScene = Ref<Scene>::Create();
	//m_EditorScene->CopyTo(m_SimulationScene);

	//m_SimulationScene->OnSimulationStart();
	//m_SceneHierarchy->SetContext(m_SimulationScene);
	m_CurrentScene = m_SimulationScene;
}

void HighLoEditor::OnSceneEndSimulation()
{
	//m_SimulationScene->OnSimulationEnd();
	m_SceneState = SceneState::Edit;

	m_SimulationScene = nullptr;

	m_SelectionContext.clear();
	//m_SceneHierarchy->SetContext(m_EditorScene);
	m_CurrentScene = m_EditorScene;
}

float HighLoEditor::GetSnapValue()
{
	switch (m_GizmoType)
	{
		case GizmoType::Translate:	return 0.5f;
		case GizmoType::Rotate:		return 45.0f;
		case GizmoType::Scale:		return 0.5f;
	}
	return 0.0f;
}

void HighLoEditor::DeleteEntity(Entity entity)
{

}

