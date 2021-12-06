// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoEditor.h"
#include "Core/MenuItems.h"

#define EDITOR_LOG_PREFIX "HighloEdit>   "

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

HighLoEditor::HighLoEditor(const ApplicationSettings &settings, const HLString &projectPath)
	: HLApplication(settings), m_ProjectPath(projectPath), m_UserSettings(Ref<UserSettings>::Create())
{
	if (projectPath.IsEmpty())
		m_ProjectPath = "SandboxProject/Sandbox.hlproj";

	// Register persistent storage
	FileSystemPath roamingPath = FileSystem::Get()->GetPersistentStoragePath();
	m_RoamingPath = roamingPath / "HighLo";

	HL_CORE_INFO(EDITOR_LOG_PREFIX "[+] Registering folder in roaming folder: {0} [+]", **m_RoamingPath);
	if (!FileSystem::Get()->FolderExists(m_RoamingPath))
	{
		FileSystem::Get()->CreateFolder(m_RoamingPath);
	}

	// TODO: Write UserPreferences into roaming path
	// The UserPreferences file should contain the following data:
	// - an array of the recent projects
	// - whether or not to display the splash screen
	// - whether or not to display the welcome popup
}

HighLoEditor::~HighLoEditor()
{
}

void HighLoEditor::OnInitialize()
{
	uint32 width = GetWindow().GetWidth();
	uint32 height = GetWindow().GetHeight();

	m_EditorCamera = EditorCamera(glm::perspectiveFov(glm::radians(45.0f), (float)width, (float)height, 0.1f, 1000.0f));

	// Project
	Ref<Project> project = Ref<Project>::Create();
	// TODO: Deserialize project data here
	Project::SetActive(project);

	// Editor Panels
	m_ViewportRenderer = Ref<SceneRenderer>::Create(m_CurrentScene);
	m_ViewportRenderer->SetLineWidth(m_LineWidth);

	m_AssetBrowserPanel = UniqueRef<AssetBrowserPanel>::Create(project);

	m_SceneHierarchyPanel = UniqueRef<SceneHierarchyPanel>::Create();
	m_SceneHierarchyPanel->SetEntityDeletedCallback(std::bind(&HighLoEditor::OnEntityDeleted, this, std::placeholders::_1));
	m_SceneHierarchyPanel->SetSelectionChangedCallback(std::bind(&HighLoEditor::SelectEntity, this, std::placeholders::_1));
	//m_SceneHierarchyPanel->SetInvalidAssetMetaDataCallback(std::bind(&HighLoEditor::OnInvalidMetaData, this, std::placeholders::_1));

	m_EditorConsolePanel = UniqueRef<EditorConsolePanel>::Create();
	m_EditorScene = Ref<Scene>::Create("Emtpy Scene", true);

	AssetEditorPanel::Init();

	GetWindow().Maximize();
	GetWindow().SetWindowIcon("assets/Resources/HighLoEngine.png");
	UpdateWindowTitle("Untitled Scene");

	FileSystemWatcher::Get()->Start(project->GetConfig().AssetDirectory);

	// Create FileMenu
	m_MenuBar = MenuBar::Create();

	Ref<FileMenu> importMenu = FileMenu::Create("Import");
	importMenu->AddMenuItem("Import .obj", "", MENU_ITEM_IMPORT_OBJ, [=](FileMenu *menu, MenuItem *item) { OnFileMenuPressed(menu, item); }, false);
	importMenu->AddMenuItem("Import .fbx", "", MENU_ITEM_IMPORT_FBX, [=](FileMenu *menu, MenuItem *item) { OnFileMenuPressed(menu, item); }, false);
	importMenu->AddMenuItem("Import .stl", "", MENU_ITEM_IMPORT_STL, [=](FileMenu *menu, MenuItem *item) { OnFileMenuPressed(menu, item); }, false);
	importMenu->AddMenuItem("Import .3ds", "", MENU_ITEM_IMPORT_3DS, [=](FileMenu *menu, MenuItem *item) { OnFileMenuPressed(menu, item); }, false);
	importMenu->AddMenuItem("Import .c4d", "", MENU_ITEM_IMPORT_C4D, [=](FileMenu *menu, MenuItem *item) { OnFileMenuPressed(menu, item); }, false);
	importMenu->AddMenuItem("Import .mb", "", MENU_ITEM_IMPORT_MB, [=](FileMenu *menu, MenuItem *item) { OnFileMenuPressed(menu, item); }, false);
	Ref<FileMenu> exportMenu = FileMenu::Create("Export");
	exportMenu->AddMenuItem("Export .obj", "", MENU_ITEM_EXPORT_OBJ, [=](FileMenu *menu, MenuItem *item) { OnFileMenuPressed(menu, item); }, false);
	exportMenu->AddMenuItem("Export .fbx", "", MENU_ITEM_EXPORT_FBX, [=](FileMenu *menu, MenuItem *item) { OnFileMenuPressed(menu, item); }, false);
	exportMenu->AddMenuItem("Export .stl", "", MENU_ITEM_EXPORT_STL, [=](FileMenu *menu, MenuItem *item) { OnFileMenuPressed(menu, item); }, false);
	exportMenu->AddSeparator();
	exportMenu->AddMenuItem("Export .mp4", "", MENU_ITEM_EXPORT_MP4, [=](FileMenu *menu, MenuItem *item) { OnFileMenuPressed(menu, item); }, false);
	exportMenu->AddMenuItem("Export .avi", "", MENU_ITEM_EXPORT_AVI, [=](FileMenu *menu, MenuItem *item) { OnFileMenuPressed(menu, item); }, false);
	exportMenu->AddMenuItem("Export .mov", "", MENU_ITEM_EXPORT_MOV, [=](FileMenu *menu, MenuItem *item) { OnFileMenuPressed(menu, item); }, false);

	Ref<FileMenu> fileMenu = FileMenu::Create("File");
	fileMenu->AddMenuItem("New Scene", "Strg+N", MENU_ITEM_NEW_SCENE, [=](FileMenu *menu, MenuItem *item) { OnFileMenuPressed(menu, item); });
	fileMenu->AddMenuItem("Open Scene...", "Strg+O", MENU_ITEM_OPEN_SCENE, [=](FileMenu *menu, MenuItem *item) { OnFileMenuPressed(menu, item); });
	fileMenu->AddSeparator();
	fileMenu->AddMenuItem("Save Scene", "Strg+S", MENU_ITEM_SAVE_SCENE, [=](FileMenu *menu, MenuItem *item) { OnFileMenuPressed(menu, item); }, false);
	fileMenu->AddMenuItem("Save Scene as...", "Strg+Shift+S", MENU_ITEM_SAVE_SCENE_AS, [=](FileMenu *menu, MenuItem *item) { OnFileMenuPressed(menu, item); });
	fileMenu->AddSeparator();
	fileMenu->AddMenuItem("Settings", "", MENU_ITEM_SETTINGS, [=](FileMenu *menu, MenuItem *item) { OnFileMenuPressed(menu, item); });
	fileMenu->AddSeparator();
	fileMenu->AddSubMenu(importMenu);
	fileMenu->AddSubMenu(exportMenu);
	fileMenu->AddSeparator();
	fileMenu->AddMenuItem("Quit", "Strg+Shift+Q", MENU_ITEM_QUIT, [=](FileMenu *menu, MenuItem *item) { OnFileMenuPressed(menu, item); });

	Ref<FileMenu> editMenu = FileMenu::Create("Edit");
	editMenu->AddMenuItem("Undo", "Strg+Z", MENU_ITEM_UNDO, [=](FileMenu *menu, MenuItem *item) { OnFileMenuPressed(menu, item); });
	editMenu->AddMenuItem("Redo", "Strg+Y", MENU_ITEM_REDO, [=](FileMenu *menu, MenuItem *item) { OnFileMenuPressed(menu, item); });
	editMenu->AddSeparator();

	Ref<FileMenu> modelSubMenu = FileMenu::Create("Create 3D Objects");
	modelSubMenu->AddMenuItem("Create Null Object", "", MENU_ITEM_ASSET_CREATE_NULL_OBJECT, [=](FileMenu *menu, MenuItem *item) { OnFileMenuPressed(menu, item); });
	modelSubMenu->AddSeparator();
	modelSubMenu->AddMenuItem("Create Cube", "", MENU_ITEM_ASSET_CREATE_CUBE, [=](FileMenu *menu, MenuItem *item) { OnFileMenuPressed(menu, item); });
	modelSubMenu->AddMenuItem("Create Sphere", "", MENU_ITEM_ASSET_CREATE_SPHERE, [=](FileMenu *menu, MenuItem *item) { OnFileMenuPressed(menu, item); });
	modelSubMenu->AddMenuItem("Create Capsule", "", MENU_ITEM_ASSET_CREATE_CAPSULE, [=](FileMenu *menu, MenuItem *item) { OnFileMenuPressed(menu, item); });
	modelSubMenu->AddMenuItem("Create Cylinder", "", MENU_ITEM_ASSET_CREATE_CYLINDER, [=](FileMenu *menu, MenuItem *item) { OnFileMenuPressed(menu, item); });

	Ref<FileMenu> gameObjectMenu = FileMenu::Create("Game Objects");
	gameObjectMenu->AddMenuItem("Create Folder", "", MENU_ITEM_ASSET_CREATE_FOLDER, [=](FileMenu *menu, MenuItem *item) { OnFileMenuPressed(menu, item); });
	gameObjectMenu->AddSubMenu(modelSubMenu);
	gameObjectMenu->AddMenuItem("Create Camera", "", MENU_ITEM_ASSET_CREATE_CAMERA, [=](FileMenu *menu, MenuItem *item) { OnFileMenuPressed(menu, item); });

	Ref<FileMenu> rendererMenu = FileMenu::Create("Renderer");
	rendererMenu->AddMenuItem("Rendering Settings", "", MENU_RENDERER_SETTINGS, [=](FileMenu *menu, MenuItem *item) { OnFileMenuPressed(menu, item); });
	rendererMenu->AddSeparator();
	rendererMenu->AddMenuItem("Offline Renderer", "", MENU_RENDERER_OFFLINE_RENDERER, [=](FileMenu *menu, MenuItem *item) { OnFileMenuPressed(menu, item); });

	m_WindowMenu = FileMenu::Create("Window");
	m_WindowMenu->AddMenuItem("Editor Console", "", MENU_ITEM_WINDOW_EDITOR_CONSOLE, [=](FileMenu *menu, MenuItem *item) { OnFileMenuPressed(menu, item); });

	Ref<FileMenu> helpMenu = FileMenu::Create("Help");
	helpMenu->AddMenuItem("About HighLo", "", MENU_ITEM_ABOUT, [=](FileMenu *menu, MenuItem *item) { OnFileMenuPressed(menu, item); });
	helpMenu->AddMenuItem("Documentation", "", MENU_ITEM_DOCUMENTATION, [=](FileMenu *menu, MenuItem *item) { OnFileMenuPressed(menu, item); });

	m_MenuBar->AddMenu(fileMenu);
	m_MenuBar->AddMenu(editMenu);
	m_MenuBar->AddMenu(gameObjectMenu);
	m_MenuBar->AddMenu(rendererMenu);
	m_MenuBar->AddMenu(m_WindowMenu);
	m_MenuBar->AddMenu(helpMenu);
	GetWindow().SetMenuBar(m_MenuBar);
}

void HighLoEditor::OnUpdate(Timestep ts)
{
	// TODO: For some reason this crashes right away
	/*else if (m_SceneState != SceneState::Simulate)
		OnSceneStop();*/

	UpdateUIFlags();

	switch (m_SceneState)
	{
		case SceneState::Edit:
		{
			m_EditorCamera.SetActive(m_AllowViewportCameraEvents);
			m_EditorCamera.Update();
			UI::SetMouseEnabled(true);
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

	AssetEditorPanel::OnUpdate(ts);
}

void HighLoEditor::UpdateUIFlags()
{
	// Make sure the "Show Console" window always
	// reflects the current opened state of the log tab.
	m_WindowMenu->GetMenuItemWithID(MENU_ITEM_WINDOW_EDITOR_CONSOLE)->IsSelected = m_ShowConsolePanel;
}

void HighLoEditor::OnShutdown()
{
	AssetEditorPanel::Shutdown();

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
		if (m_ViewportPanelMouseOver)
			m_EditorCamera.OnEvent(e);

		m_EditorScene->OnEvent(e);
	}
	else if (m_SceneState == SceneState::Play)
	{
		m_RuntimeScene->OnEvent(e);
	}

	AssetEditorPanel::OnEvent(e);
	m_AssetBrowserPanel->OnEvent(e);
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
	m_ViewportRenderer->SetClearColor(m_ClearColor);
	m_EditorScene->SetViewportSize((uint32)viewportSize.x, (uint32)viewportSize.y);

	if (m_RuntimeScene)
		m_RuntimeScene->SetViewportSize((uint32)viewportSize.x, (uint32)viewportSize.y);

	m_EditorCamera.SetProjectionMatrix(glm::perspectiveFov(glm::radians(45.0f), viewportSize.x, viewportSize.y, 0.1f, 1000.0f));
	m_EditorCamera.SetViewportSize((uint32)viewportSize.x, (uint32)viewportSize.y);

	// Render viewport image
	UI::Image(m_ViewportRenderer->GetFinalRenderTexture(), viewportSize, { 0, 1 }, { 1, 0 });
	UI::EndViewport();

	m_AssetBrowserPanel->OnUIRender();
	m_SceneHierarchyPanel->OnUIRender();
	m_EditorConsolePanel->OnUIRender(&m_ShowConsolePanel);

	// Object Properties Panel
	UI::BeginViewport("Object Properties");
	UI::EndViewport();

	UI::EndWindow();

	AssetEditorPanel::OnUIRender(timestep);

	// TODO: This breaks
	//AssetManager::Get()->OnUIRender(m_AssetManagerPanelOpen);
}

void HighLoEditor::OnResize(uint32 width, uint32 height)
{
}

void HighLoEditor::SelectEntity(Entity entity)
{
	if (!entity)
		return;

	SelectedMesh selection;
	if (entity.HasComponent<DynamicModelComponent>())
	{
		auto meshComp = entity.GetComponent<DynamicModelComponent>();
		if (meshComp->Model && meshComp->Model->GetAssetType() == AssetType::Mesh)
		{
			selection.MeshIndex = meshComp->Model->GetSubmeshIndices()[0];
		}
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

bool HighLoEditor::OnKeyPressedEvent(const KeyPressedEvent &e)
{
	if (m_ViewportPanelMouseOver)
		{
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
		&& m_ViewportPanelMouseOver
		&& !Input::IsKeyPressed(HL_KEY_LEFT_ALT)
		&& !UI::IsMouseOverGizmo()
		&& m_SceneState != SceneState::Play)
	{
	}

	return false;
}

void HighLoEditor::OnFileMenuPressed(FileMenu *menu, MenuItem *item)
{
	switch (item->ID)
	{
		case MENU_ITEM_QUIT:
		{
			HLApplication::Get().Close();
			break;
		}

		case MENU_ITEM_ABOUT:
		{
			FileSystem::Get()->OpenInBrowser("https://www.highlo-engine.com");
			break;
		}

		case MENU_ITEM_DOCUMENTATION:
		{
			FileSystem::Get()->OpenInBrowser("https://docs.highlo-engine.com");
			break;
		}

		case MENU_ITEM_NEW_SCENE:
		{
			HL_INFO("Create a new scene");
			auto newScene = Scene::CreateEmpty();
			m_AllScenes.push_back(newScene);

			// Update the editor scene
			m_EditorScene = newScene;

			// Update the current scene
			m_CurrentScene = m_EditorScene;
			break;
		}

		case MENU_ITEM_OPEN_SCENE:
		{
			OpenScene(menu, item);
			break;
		}

		case MENU_ITEM_SAVE_SCENE:
		{
			SaveScene(menu, item);
			break;
		}

		case MENU_ITEM_SAVE_SCENE_AS:
		{
			SaveSceneAs(menu, item);
			break;
		}

		case MENU_ITEM_SETTINGS:
		{
			HL_INFO("Open Settings dialog...");
			break;
		}

		case MENU_ITEM_ASSET_CREATE_CAMERA:
		{
			HL_INFO("Creating Camera...");
			break;
		}

		case MENU_ITEM_ASSET_CREATE_CAPSULE:
		{
			HL_INFO("Creating Capsule...");
			break;
		}

		case MENU_ITEM_ASSET_CREATE_CUBE:
		{
			HL_INFO("Creating cube...");
			break;
		}

		case MENU_ITEM_ASSET_CREATE_CYLINDER:
		{
			HL_INFO("Creating Cylinder...");
			break;
		}

		case MENU_ITEM_ASSET_CREATE_FOLDER:
		{
			HL_INFO("Creating Folder...");
			break;
		}

		case MENU_ITEM_ASSET_CREATE_NULL_OBJECT:
		{
			HL_INFO("Creating Null Object...");
			break;
		}

		case MENU_ITEM_ASSET_CREATE_SPHERE:
		{
			HL_INFO("Creating Sphere");
			break;
		}

		case MENU_ITEM_WINDOW_EDITOR_CONSOLE:
		{
			m_ShowConsolePanel = !m_ShowConsolePanel;
			break;
		}
	}
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
	Input::SetCursorMode(CursorMode::Normal);

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
	auto children = entity.Children();
	for (auto childId : children)
		DeleteEntity(m_EditorScene->FindEntityByUUID(childId));

	m_EditorScene->UnparentEntity(entity);
	m_EditorScene->DestroyEntity(entity);
}

