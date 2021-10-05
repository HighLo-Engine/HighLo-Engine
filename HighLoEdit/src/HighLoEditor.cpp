// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoEditor.h"
#include "Core/MenuItems.h"

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

	m_EditorCamera = EditorCamera(glm::perspectiveFov(glm::radians(45.0f), (float) width, (float) height, 0.1f, 1000.0f));

	// Editor Panels
	m_ViewportRenderer = Ref<SceneRenderer>::Create(m_CurrentScene);
	m_ViewportRenderer->SetLineWidth(m_LineWidth);

	m_SceneHierarchyPanel = MakeUniqueRef<SceneHierarchyPanel>();
	m_SceneHierarchyPanel->SetEntityDeletedCallback(std::bind(&HighLoEditor::OnEntityDeleted, this, std::placeholders::_1));
	m_SceneHierarchyPanel->SetSelectionChangedCallback(std::bind(&HighLoEditor::SelectEntity, this, std::placeholders::_1));
	//m_SceneHierarchyPanel->SetInvalidAssetMetaDataCallback(std::bind(&HighLoEditor::OnInvalidMetaData, this, std::placeholders::_1));

	m_EditorConsolePanel = MakeUniqueRef<EditorConsolePanel>();
	m_EditorScene = Ref<Scene>::Create("Emtpy Scene", true);

	GetWindow().Maximize();
	GetWindow().SetWindowIcon("assets/Resources/HighLoEngine.png");
	UpdateWindowTitle("Untitled Scene");

	FileSystemWatcher::Get()->Start("assets");

	// Create FileMenu
	m_MenuBar = MenuBar::Create();

	Ref<FileMenu> importMenu = FileMenu::Create("Import");
	importMenu->AddMenuItem("Import .obj", "", MENU_ITEM_IMPORT_OBJ, [=](FileMenu *menu, MenuItem *item) { HL_TRACE("Importing obj"); }, false);
	importMenu->AddMenuItem("Import .fbx", "", MENU_ITEM_IMPORT_FBX, [=](FileMenu *menu, MenuItem *item) { HL_TRACE("Importing fbx"); }, false);
	importMenu->AddMenuItem("Import .stl", "", MENU_ITEM_IMPORT_STL, [=](FileMenu *menu, MenuItem *item) { HL_TRACE("Importing stl"); }, false);
	importMenu->AddMenuItem("Import .3ds", "", MENU_ITEM_IMPORT_3DS, [=](FileMenu *menu, MenuItem *item) { HL_TRACE("Importing 3ds"); }, false);
	importMenu->AddMenuItem("Import .c4d", "", MENU_ITEM_IMPORT_C4D, [=](FileMenu *menu, MenuItem *item) { HL_TRACE("Importing c4d"); }, false);
	importMenu->AddMenuItem("Import .mb", "", MENU_ITEM_IMPORT_MB, [=](FileMenu *menu, MenuItem *item) { HL_TRACE("Importing mb"); }, false);
	Ref<FileMenu> exportMenu = FileMenu::Create("Export");
	exportMenu->AddMenuItem("Export .obj", "", MENU_ITEM_EXPORT_OBJ, [=](FileMenu *menu, MenuItem *item) { HL_TRACE("Exporting obj"); }, false);
	exportMenu->AddMenuItem("Export .fbx", "", MENU_ITEM_EXPORT_FBX, [=](FileMenu *menu, MenuItem *item) { HL_TRACE("Exporting fbx"); }, false);
	exportMenu->AddMenuItem("Export .stl", "", MENU_ITEM_EXPORT_STL, [=](FileMenu *menu, MenuItem *item) { HL_TRACE("Exporting stl"); }, false);
	exportMenu->AddSeparator();
	exportMenu->AddMenuItem("Export .mp4", "", MENU_ITEM_EXPORT_MP4, [=](FileMenu *menu, MenuItem *item) { HL_TRACE("Exporting mp4"); }, false);
	exportMenu->AddMenuItem("Export .avi", "", MENU_ITEM_EXPORT_AVI, [=](FileMenu *menu, MenuItem *item) { HL_TRACE("Exporting avi"); }, false);
	exportMenu->AddMenuItem("Export .mov", "", MENU_ITEM_EXPORT_MOV, [=](FileMenu *menu, MenuItem *item) { HL_TRACE("Exporting mov"); }, false);

	Ref<FileMenu> fileMenu = FileMenu::Create("File");
	fileMenu->AddMenuItem("New Scene", "Strg+N", MENU_ITEM_NEW_SCENE, [=](FileMenu *menu, MenuItem *item) { NewScene(menu, item); });
	fileMenu->AddMenuItem("Open Scene...", "Strg+O", MENU_ITEM_OPEN_SCENE, [=](FileMenu *menu, MenuItem *item) { OpenScene(menu, item); });
	fileMenu->AddSeparator();
	fileMenu->AddMenuItem("Save Scene", "Strg+S", MENU_ITEM_SAVE_SCENE, [=](FileMenu *menu, MenuItem *item) { SaveScene(menu, item); }, false);
	fileMenu->AddMenuItem("Save Scene as...", "Strg+Shift+S", MENU_ITEM_SAVE_SCENE_AS, [=](FileMenu *menu, MenuItem *item) { SaveSceneAs(menu, item); });
	fileMenu->AddSeparator();
	fileMenu->AddMenuItem("Settings", "", MENU_ITEM_SETTINGS, [=](FileMenu *menu, MenuItem *item) { HL_TRACE("Settings..."); });
	fileMenu->AddSeparator();
	fileMenu->AddSubMenu(importMenu);
	fileMenu->AddSubMenu(exportMenu);
	fileMenu->AddSeparator();
	fileMenu->AddMenuItem("Quit", "Strg+Shift+Q", MENU_ITEM_QUIT, [=](FileMenu *menu, MenuItem *item) { HLApplication::Get().Close(); });

	Ref<FileMenu> editMenu = FileMenu::Create("Edit");
	editMenu->AddMenuItem("Undo", "Strg+Z", MENU_ITEM_UNDO, [=](FileMenu *menu, MenuItem *item) { HL_TRACE("Undo..."); });
	editMenu->AddMenuItem("Redo", "Strg+Y", MENU_ITEM_REDO, [=](FileMenu *menu, MenuItem *item) { HL_TRACE("Redo..."); });
	editMenu->AddSeparator();

	Ref<FileMenu> modelSubMenu = FileMenu::Create("Create 3D Objects");
	modelSubMenu->AddMenuItem("Create Null Object", "", MENU_ITEM_ASSET_CREATE_NULL_OBJECT, [=](FileMenu *menu, MenuItem *item) {  });
	modelSubMenu->AddSeparator();
	modelSubMenu->AddMenuItem("Create Cube", "", MENU_ITEM_ASSET_CREATE_CUBE, [=](FileMenu *menu, MenuItem *item) {  });
	modelSubMenu->AddMenuItem("Create Sphere", "", MENU_ITEM_ASSET_CREATE_SPHERE, [=](FileMenu *menu, MenuItem *item) {  });
	modelSubMenu->AddMenuItem("Create Capsule", "", MENU_ITEM_ASSET_CREATE_CAPSULE, [=](FileMenu *menu, MenuItem *item) {  });
	modelSubMenu->AddMenuItem("Create Cylinder", "", MENU_ITEM_ASSET_CREATE_CYLINDER, [=](FileMenu *menu, MenuItem *item) {  });

	Ref<FileMenu> gameObjectMenu = FileMenu::Create("Game Objects");
	gameObjectMenu->AddMenuItem("Create Folder", "", MENU_ITEM_ASSET_CREATE_FOLDER, [=](FileMenu *menu, MenuItem *item) {  });
	gameObjectMenu->AddSubMenu(modelSubMenu);
	gameObjectMenu->AddMenuItem("Create Camera", "", MENU_ITEM_ASSET_CREATE_CAMERA, [=](FileMenu *menu, MenuItem *item) {  });

	Ref<FileMenu> rendererMenu = FileMenu::Create("Renderer");
	rendererMenu->AddMenuItem("Rendering Settings", "", MENU_RENDERER_SETTINGS, [=](FileMenu *menu, MenuItem *item) {});
	rendererMenu->AddSeparator();
	rendererMenu->AddMenuItem("Offline Renderer", "", MENU_RENDERER_OFFLINE_RENDERER, [=](FileMenu *menu, MenuItem *item) {});

	Ref<FileMenu> windowMenu = FileMenu::Create("Window");
	bool darkThemeActive = UI::GetCurrentWindowStyle() == UI::ImGuiWindowStyle::Dark;
	bool lightThemeActive = UI::GetCurrentWindowStyle() == UI::ImGuiWindowStyle::Light;

	windowMenu->AddMenuItem("Set Dark Theme", "", MENU_ITEM_DARK_THEME, [=](FileMenu *menu, MenuItem *item) { EnableDarkMode(menu, item); }, !darkThemeActive);
	windowMenu->AddMenuItem("Set Light Theme", "", MENU_ITEM_LIGHT_THEME, [=](FileMenu *menu, MenuItem *item) { EnableLightMode(menu, item); }, !lightThemeActive);
	windowMenu->AddSeparator();
	windowMenu->AddMenuItem("Editor Console", "", MENU_ITEM_WINDOW_EDITOR_CONSOLE, [=](FileMenu *menu, MenuItem *item) { m_ShowConsolePanel = !m_ShowConsolePanel; });

	Ref<FileMenu> helpMenu = FileMenu::Create("Help");
	helpMenu->AddMenuItem("About HighLo", "", MENU_ITEM_ABOUT, [=](FileMenu *menu, MenuItem *item) { VirtualFileSystem::Get()->OpenInBrowser("https://www.highlo-engine.com"); });
	helpMenu->AddMenuItem("Documentation", "", MENU_ITEM_DOCUMENTATION, [=](FileMenu *menu, MenuItem *item) { VirtualFileSystem::Get()->OpenInBrowser("https://www.highlo-engine.com"); });

	m_MenuBar->AddMenu(fileMenu);
	m_MenuBar->AddMenu(editMenu);
	m_MenuBar->AddMenu(gameObjectMenu);
	m_MenuBar->AddMenu(rendererMenu);
	m_MenuBar->AddMenu(windowMenu);
	m_MenuBar->AddMenu(helpMenu);
	GetWindow().SetMenuBar(m_MenuBar);
}

void HighLoEditor::OnUpdate(Timestep ts)
{
	if (m_SceneState == SceneState::Edit)
		OnScenePlay();

	// TODO: For some reason this crashes right away
	/*else if (m_SceneState != SceneState::Simulate)
		OnSceneStop();*/

	switch (m_SceneState)
	{
		case SceneState::Edit:
		{
			UI::SetMouseEnabled(true);

			m_EditorCamera.Update();
			m_EditorScene->OnUpdateEditor(m_ViewportRenderer, ts, m_EditorCamera);
			break;
		}

		case SceneState::Play:
		{
			m_RuntimeScene->OnUpdate(ts);
			m_RuntimeScene->OnUpdateRuntime(m_ViewportRenderer, ts);
			break;
		}

		case SceneState::Pause:
		{
			UI::SetMouseEnabled(true);

			m_EditorCamera.Update();
			m_RuntimeScene->OnUpdateRuntime(m_ViewportRenderer, ts);
			break;
		}

		case SceneState::Simulate:
		{
			m_EditorCamera.Update();
			m_SimulationScene->OnUpdate(ts);
			m_SimulationScene->OnSimulate(m_ViewportRenderer, ts, m_EditorCamera);
			break;
		}
	}
}

void HighLoEditor::OnShutdown()
{
	FileSystemWatcher::Get()->Stop();
}

void HighLoEditor::OnEvent(Event &e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<KeyPressedEvent>(HL_BIND_EVENT_FUNCTION(HighLoEditor::OnKeyPressedEvent));
	dispatcher.Dispatch<MouseButtonPressedEvent>(HL_BIND_EVENT_FUNCTION(HighLoEditor::OnMouseButtonPressedEvent));
	
	if (m_MenuBar)
		m_MenuBar->OnEvent(e);

	if (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate)
	{
	//	if (m_Viewport->IsMouseOver())
	//		m_Viewport->OnEvent(e);
	}
	else if (m_SceneState == SceneState::Play)
	{
		m_RuntimeScene->OnEvent(e);
	}
}

void HighLoEditor::OnUIRender(Timestep timestep)
{
	UI::BeginWindow("RootWindow", true, true);

	// Viewport
	UI::BeginViewport("Viewport");

	m_ViewportPanelMouseOver = ImGui::IsWindowHovered();
	m_ViewportPanelFocused = ImGui::IsWindowFocused();

	auto viewportOffset = ImGui::GetCursorPos(); // includes tab bar
	auto viewportSize = ImGui::GetContentRegionAvail();
	m_ViewportRenderer->SetViewportSize((uint32)viewportSize.x, (uint32)viewportSize.y);
	m_EditorScene->SetViewportSize((uint32)viewportSize.x, (uint32)viewportSize.y);

	/*
	if (m_RuntimeScene)
		m_RuntimeScene->SetViewportSize((uint32)viewportSize.x, (uint32)viewportSize.y);
	*/

	m_EditorCamera.SetProjectionMatrix(glm::perspectiveFov(glm::radians(45.0f), viewportSize.x, viewportSize.y, 0.1f, 1000.0f));
	m_EditorCamera.SetViewportSize((uint32)viewportSize.x, (uint32)viewportSize.y);

	// Render viewport image
	UI::Image(m_ViewportRenderer->GetFinalRenderTexture(), viewportSize, { 0, 1 }, { 1, 0 });

	UI::EndViewport();

	// Asset Window
	UI::BeginViewport("Assets");
	UI::EndViewport();

	// Scene Hierarchy Panel
	m_SceneHierarchyPanel->OnUIRender();

	// Editor Console Panel
	m_EditorConsolePanel->OnUIRender(&m_ShowConsolePanel);

	// Object Properties Panel
	UI::BeginViewport("Object Properties");
	UI::EndViewport();

	UI::EndWindow();
}

void HighLoEditor::OnResize(uint32 width, uint32 height)
{
}

void HighLoEditor::SelectEntity(Entity entity)
{
	if (!entity.ID)
		return;

	SelectedMesh selection;
	if (entity.HasComponent<ModelComponent>())
	{
		auto meshComp = entity.GetComponent<ModelComponent>();
		/*if (meshComp.Model && meshComp.Model->Type == AssetType::Mesh)
		{
			selection.Mesh = &meshComp.Mesh->GetSubmeshes()[0];
		}*/
	}

	selection.Entity = entity;
	m_SelectionContext.clear();
	m_SelectionContext.push_back(selection);

	if (m_SceneState == SceneState::Edit)
	{
		m_EditorScene->SetSelectedEntity(entity);
		m_CurrentScene = m_EditorScene;
	}
	else if (m_SceneState == SceneState::Simulate)
	{
		m_EditorScene->SetSelectedEntity(entity);
		m_CurrentScene = m_SimulationScene;
	}
}

void HighLoEditor::UpdateWindowTitle(const HLString &sceneName)
{
	HLString title = sceneName + " - HighLo Engine - Renderer: " + Renderer::GetCurrentRenderingAPI();
	GetWindow().SetTitle(title);
}

void HighLoEditor::NewScene(FileMenu *menu, MenuItem *item)
{
	HL_TRACE("NewScene");
	UpdateWindowTitle("Untitled Scene");
}

void HighLoEditor::OpenScene(FileMenu *menu, MenuItem *item)
{
	HL_TRACE("TEST OPEN");
	Ref<FileDialogue> fd = FileDialogue::Create();
	FileDialogueFilter filter;
	filter.AddFilter("High-Lo Scene file", "*.hl");
	fd->SetFilter(filter);
	HLString result = fd->SelectFile();

	if (!result.IsEmpty())
	{
		HL_TRACE("TEST NOT EMPTY");
		OpenScene(m_LastSceneFilePath);
	}
}

void HighLoEditor::OpenScene(const HLString &path)
{
	m_LastSceneFilePath = path;
	HL_TRACE("Open scene {0}", *path);

	// Deserialize scene
}

void HighLoEditor::SaveScene(FileMenu *menu, MenuItem *item)
{
	if (!m_LastSceneFilePath.IsEmpty())
	{
		// Serialize scene
	}
	else
	{
		SaveSceneAs(menu, item);
	}
}

void HighLoEditor::SaveSceneAs(FileMenu *menu, MenuItem *item)
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

void HighLoEditor::EnableDarkMode(FileMenu *menu, MenuItem *item)
{
	UI::SetDarkThemeColors();
	menu->EnableMenuItem(item->ID, false);
	menu->EnableMenuItem(item->ID + 1, true);
}

void HighLoEditor::EnableLightMode(FileMenu *menu, MenuItem *item)
{
	UI::SetLightThemeColors();
	menu->EnableMenuItem(item->ID, false);
	menu->EnableMenuItem(item->ID - 1, true);
}

bool HighLoEditor::OnKeyPressedEvent(const KeyPressedEvent &e)
{
//	if (m_Viewport->IsMouseOver())
//		{
		switch (e.GetKeyCode())
			{
			case HL_KEY_F:
			{
				if (m_SelectionContext.size() == 0)
					break;

				Entity selectedEntity = m_SelectionContext[0].Entity;
		//		m_Viewport->Focus(selectedEntity._TransformComponent->Transform.GetPosition());
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
//		}
	}

	if (Input::IsKeyPressed(HL_KEY_LEFT_CONTROL))
	{
		if (Input::IsKeyPressed(HL_KEY_LEFT_SHIFT))
			{
			switch (e.GetKeyCode())
			{
				case HL_KEY_S:
				{
					SaveSceneAs(nullptr, nullptr);
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
				NewScene(nullptr, nullptr);
				break;
			}

			case HL_KEY_O:
			{
				OpenScene(nullptr, nullptr);
				break;
			}

			case HL_KEY_S:
			{
				SaveScene(nullptr, nullptr);
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

				Entity selectedEntity = m_SelectionContext[0].Entity;
				// TODO
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
//		&& m_Viewport->IsMouseOver()
		&& !Input::IsKeyPressed(HL_KEY_LEFT_ALT)
		&& !UI::IsMouseOverGizmo()
		&& m_SceneState != SceneState::Play)
	{
	}

	return false;
}

void HighLoEditor::OnSelected(const SelectedMesh &selectionContext)
{
	m_SceneHierarchyPanel->SetSelected(selectionContext.Entity);
	m_EditorScene->SetSelectedEntity(selectionContext.Entity);
}

void HighLoEditor::OnEntityDeleted(Entity e)
{
	if (m_SelectionContext.size() > 0 && m_SelectionContext[0].Entity == e)
	{
		m_SelectionContext.clear();
		m_EditorScene->SetSelectedEntity({});
	}
}

void HighLoEditor::OnScenePlay()
{
	m_SelectionContext.clear();

	m_SceneState = SceneState::Play;
	UI::SetMouseEnabled(true);
	//Input::SetCursorMode(CursorMode::Normal);

	// Copy current scene to be able to reset after stop button has been pressed
	m_RuntimeScene = Ref<Scene>::Create();
	// m_EditorScene->CopyTo(m_RuntimeScene);

	m_RuntimeScene->OnRuntimeStart();
	m_SceneHierarchyPanel->SetContext(m_RuntimeScene);

	m_EditorConsolePanel->OnScenePlay();

	m_CurrentScene = m_RuntimeScene;
}

void HighLoEditor::OnSceneStop()
{
	m_RuntimeScene->OnRuntimeStop();
	m_SceneState = SceneState::Edit;

	// Unload runtime scene
	m_RuntimeScene = nullptr;

	m_SelectionContext.clear();
	m_SceneHierarchyPanel->SetContext(m_EditorScene);
	m_CurrentScene = m_EditorScene;
}

void HighLoEditor::OnSceneStartSimulation()
{
	m_SelectionContext.clear();

	m_SceneState = SceneState::Simulate;

	// Copy current scene to be able to reset after stop button has been pressed
	m_SimulationScene = Ref<Scene>::Create();
	m_EditorScene->CopyTo(m_SimulationScene);

	m_SimulationScene->OnSimulationStart();
	m_SceneHierarchyPanel->SetContext(m_SimulationScene);
	m_CurrentScene = m_SimulationScene;
}

void HighLoEditor::OnSceneEndSimulation()
{
	m_SimulationScene->OnSimulationStop();
	m_SceneState = SceneState::Edit;

	m_SimulationScene = nullptr;

	m_SelectionContext.clear();
	m_SceneHierarchyPanel->SetContext(m_EditorScene);
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
	/*
	auto children = entity.Children();
	for (auto childId : children)
		DeleteEntity(m_EditorScene->FindEntityByUUID(childId));
	*/

	m_EditorScene->UnparentEntity(entity);
	m_EditorScene->DestroyEntity(entity);
}

