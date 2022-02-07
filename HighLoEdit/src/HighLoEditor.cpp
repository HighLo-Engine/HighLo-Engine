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

	static HighLoEditor::GizmoType ConvertFromImGuizmoType(int32 type)
	{
		switch (type)
		{
			case -1: return HighLoEditor::GizmoType::None;
			case 0: return HighLoEditor::GizmoType::Translate;
			case 1: return HighLoEditor::GizmoType::Rotate;
			case 2: return HighLoEditor::GizmoType::Scale;
		}

		return HighLoEditor::GizmoType::None;
	}
}

HighLoEditor::HighLoEditor(const ApplicationSettings &settings)
	: HLApplication(settings), m_ProjectPath(settings.ProjectPath.String()), m_UserSettings(Ref<UserSettings>::Create())
{
	if (m_ProjectPath.IsEmpty())
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
	m_OverlayCamera.SetOrthographic((float)width, (float)height, 10.0f, -1.0f, 1.0f);

	// Project
	Ref<Project> project = Ref<Project>::Create();
	// TODO: Deserialize project data here
	Project::SetActive(project);

	AssetManager::Get()->Init();

	m_EditorScene = Ref<Scene>::Create("Emtpy Scene", true);
	m_CurrentScene = m_EditorScene;

	// Editor Panels
	m_ViewportRenderer = Ref<SceneRenderer>::Create(m_CurrentScene);

	m_AssetBrowserPanel = UniqueRef<AssetBrowserPanel>::Create(project);

	m_SceneHierarchyPanel = UniqueRef<SceneHierarchyPanel>::Create(m_CurrentScene, true);
	m_SceneHierarchyPanel->SetEntityDeletedCallback(std::bind(&HighLoEditor::OnEntityDeleted, this, std::placeholders::_1));
	m_SceneHierarchyPanel->SetSelectionChangedCallback(std::bind(&HighLoEditor::SelectEntity, this, std::placeholders::_1));
	m_SceneHierarchyPanel->SetEntityAddedCallback(std::bind(&HighLoEditor::OnEntityAdded, this, std::placeholders::_1));
	//m_SceneHierarchyPanel->SetInvalidAssetMetaDataCallback(std::bind(&HighLoEditor::OnInvalidMetaData, this, std::placeholders::_1));

	m_ObjectPropertiesPanel = UniqueRef<ObjectPropertiesPanel>::Create(m_CurrentScene, true);
	m_ObjectPropertiesPanel->SetSelectionChangedCallback(std::bind(&HighLoEditor::OnEntityChanged, this, std::placeholders::_1));

	m_EditorConsolePanel = UniqueRef<EditorConsolePanel>::Create();

	m_SettingsPanel = UniqueRef<SettingsPanel>::Create();

	GetWindow().Maximize();
	GetWindow().SetWindowIcon("assets/Resources/HighLoEngine.png");
	UpdateWindowTitle(m_SceneName);

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

	m_FileMenu = FileMenu::Create("File");
	m_FileMenu->AddMenuItem("New Scene", "Strg+N", MENU_ITEM_NEW_SCENE, [=](FileMenu *menu, MenuItem *item) { OnFileMenuPressed(menu, item); });
	m_FileMenu->AddMenuItem("Open Scene...", "Strg+O", MENU_ITEM_OPEN_SCENE, [=](FileMenu *menu, MenuItem *item) { OnFileMenuPressed(menu, item); });
	m_FileMenu->AddSeparator();
	m_FileMenu->AddMenuItem("Save Scene", "Strg+S", MENU_ITEM_SAVE_SCENE, [=](FileMenu *menu, MenuItem *item) { OnFileMenuPressed(menu, item); }, false);
	m_FileMenu->AddMenuItem("Save Scene as...", "Strg+Shift+S", MENU_ITEM_SAVE_SCENE_AS, [=](FileMenu *menu, MenuItem *item) { OnFileMenuPressed(menu, item); });
	m_FileMenu->AddSeparator();
	m_FileMenu->AddMenuItem("Settings", "", MENU_ITEM_SETTINGS, [=](FileMenu *menu, MenuItem *item) { OnFileMenuPressed(menu, item); });
	m_FileMenu->AddSeparator();
	m_FileMenu->AddSubMenu(importMenu);
	m_FileMenu->AddSubMenu(exportMenu);
	m_FileMenu->AddSeparator();
	m_FileMenu->AddMenuItem("Quit", "Strg+Shift+Q", MENU_ITEM_QUIT, [=](FileMenu *menu, MenuItem *item) { OnFileMenuPressed(menu, item); });

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
	m_WindowMenu->AddMenuItem("Asset Manager", "", MENU_ITEM_WINDOW_ASSET_MANAGER, [=](FileMenu *menu, MenuItem *item) { OnFileMenuPressed(menu, item); });

	Ref<FileMenu> helpMenu = FileMenu::Create("Help");
	helpMenu->AddMenuItem("About HighLo", "", MENU_ITEM_ABOUT, [=](FileMenu *menu, MenuItem *item) { OnFileMenuPressed(menu, item); });
	helpMenu->AddMenuItem("Documentation", "", MENU_ITEM_DOCUMENTATION, [=](FileMenu *menu, MenuItem *item) { OnFileMenuPressed(menu, item); });

	m_MenuBar->AddMenu(m_FileMenu);
	m_MenuBar->AddMenu(editMenu);
	m_MenuBar->AddMenu(gameObjectMenu);
	m_MenuBar->AddMenu(rendererMenu);
	m_MenuBar->AddMenu(m_WindowMenu);
	m_MenuBar->AddMenu(helpMenu);
	GetWindow().SetMenuBar(m_MenuBar);

	// Temp: Try to create a new asset and submit it for rendering
	AssetHandle cubeHandle = AssetFactory::CreateCube({ 1.0f, 1.0f, 1.0f });
	Ref<StaticModel> model = AssetManager::Get()->GetAsset<StaticModel>(cubeHandle);
	m_ViewportRenderer->SubmitStaticModel(model, model->GetMaterials());
}

void HighLoEditor::OnUpdate(Timestep ts)
{
	UpdateUIFlags();

	switch (m_SceneState)
	{
		case SceneState::Edit:
		{
			m_EditorCamera.SetActive(m_AllowViewportCameraEvents);
			m_EditorCamera.Update();
			UI::SetMouseEnabled(true);

			// Update Scene entities
			m_EditorScene->UpdateScene(ts);

			// Render scene content
			m_EditorScene->OnUpdateEditor(m_ViewportRenderer, ts, m_EditorCamera);

			// Render overlay
			m_EditorScene->OnUpdateOverlay(m_ViewportRenderer, ts, m_OverlayCamera);
			break;
		}

		case SceneState::Play:
		{
			// Update scene entities
			m_RuntimeScene->UpdateScene(ts);

			// Render scene content
			m_RuntimeScene->OnUpdateRuntime(m_ViewportRenderer, ts);
			break;
		}

		case SceneState::Pause:
		{
			m_EditorCamera.Update();
			UI::SetMouseEnabled(true);
			
			// Render last scene content without updating any transforms or attributes
			m_RuntimeScene->OnUpdateRuntime(m_ViewportRenderer, ts);
			break;
		}

		case SceneState::Simulate:
		{
			m_EditorCamera.Update();

			// Render scene content
			m_SimulationScene->UpdateScene(ts);

			// Run physics simulation
			m_SimulationScene->OnSimulate(m_ViewportRenderer, ts, m_EditorCamera);
			break;
		}
	}

	// Get current mouse positon and mouse pick on the viewport to detect if the user clicked on one of the rendererd objects
	auto [mx, my] = ImGui::GetMousePos();
	mx -= m_ViewportBounds[0].x;
	my -= m_ViewportBounds[0].y;
	
	glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
	my = viewportSize.y - my;

	int32 mouseX = (int32)mx;
	int32 mouseY = (int32)my;
	if (mouseX >= 0 && mouseY >= 0 && mouseX < (int32)viewportSize.x && mouseY < (int32)viewportSize.y)
	{
	//	HL_CORE_ERROR("TEST: PIXEL SELECTED: {0}", m_ViewportRenderer->GetPixel(TextureFormat::RED_INTEGER, mouseX, mouseY));
	}

	SceneRenderer::WaitForThreads();
}

void HighLoEditor::UpdateUIFlags()
{
	// Make sure the "Show Console" window always
	// reflects the current opened state of the log tab.
	m_WindowMenu->GetMenuItemWithID(MENU_ITEM_WINDOW_EDITOR_CONSOLE)->IsSelected = m_ShowConsolePanel;
	m_WindowMenu->GetMenuItemWithID(MENU_ITEM_WINDOW_ASSET_MANAGER)->IsSelected = m_AssetManagerPanelOpen;
	m_FileMenu->GetMenuItemWithID(MENU_ITEM_SETTINGS)->IsSelected = m_ShowSettingsPanel;

	if (!m_SceneIsSaved)
	{
		// Append a * to the window title
		UpdateWindowTitle(m_SceneName + " *");
	}
	else
	{
		UpdateWindowTitle(m_SceneName);
	}
}

void HighLoEditor::OnShutdown()
{
	FileSystemWatcher::Get()->Stop();

	AssetManager::Get()->Shutdown();
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
		{
			m_EditorCamera.OnEvent(e);
			m_OverlayCamera.OnEvent(e);
		}

		m_EditorScene->OnEvent(e);
	}
	else if (m_SceneState == SceneState::Play)
	{
		m_RuntimeScene->OnEvent(e);
	}

	m_AssetBrowserPanel->OnEvent(e);
}

void HighLoEditor::OnUIRender(Timestep timestep)
{
	UI::BeginWindow("RootWindow", true, true);

	// Viewport
	UI::BeginViewport("Viewport");

	m_ViewportPanelMouseOver = ImGui::IsWindowHovered();
	m_ViewportPanelFocused = ImGui::IsWindowFocused();
	auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
	auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
	auto viewportOffset = ImGui::GetWindowPos();

	m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
	m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

	auto viewportSize = ImGui::GetContentRegionAvail();
	if (viewportSize.x < 0.0f)
		viewportSize.x = (float)GetWindow().GetWidth();
	if (viewportSize.y < 0.0f)
		viewportSize.y = (float)GetWindow().GetHeight();

	m_ViewportRenderer->SetViewportSize((uint32)viewportSize.x, (uint32)viewportSize.y);
	m_ViewportRenderer->SetClearColor(m_ClearColor);
	m_EditorScene->SetViewportSize((uint32)viewportSize.x, (uint32)viewportSize.y);

	if (m_RuntimeScene)
		m_RuntimeScene->SetViewportSize((uint32)viewportSize.x, (uint32)viewportSize.y);

	m_EditorCamera.SetProjectionMatrix(glm::perspectiveFov(glm::radians(45.0f), viewportSize.x, viewportSize.y, 0.1f, 1000.0f));
	m_EditorCamera.SetViewportSize((uint32)viewportSize.x, (uint32)viewportSize.y);

	// Render viewport image
	Ref<Texture2D> viewportImage = m_ViewportRenderer->GetFinalRenderTexture();
	UI::Image(viewportImage, viewportSize, { 0, 1 }, { 1, 0 });

	// TODO: Add Guizmo Toolbar to be able to select the current guizmo type

	// Draw gizmo on top of viewport image
	if (m_GizmoType != -1 && m_SelectionContext.size())
	{
		auto &selection = m_SelectionContext[0];

		float width = (float)ImGui::GetWindowWidth();
		float height = (float)ImGui::GetWindowHeight();

		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, width, height);
		bool snap = Input::IsKeyPressed(HL_KEY_LEFT_CONTROL);

		Transform &entityTransform = selection.Entity.Transform();
		glm::mat4 rawTransform = m_CurrentScene->GetWorldSpaceTransformMatrix(selection.Entity);

		float snapValue = GetSnapValue();
		float snapValues[3] = { snapValue, snapValue, snapValue };

		if (m_SelectionMode == SelectionMode::Entity)
		{
			ImGuizmo::Manipulate(glm::value_ptr(m_EditorCamera.GetViewMatrix()), glm::value_ptr(m_EditorCamera.GetProjection()), (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(rawTransform), nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing())
			{
				Entity parent = m_CurrentScene->FindEntityByUUID(selection.Entity.GetParentUUID());
				if (parent)
				{
					glm::mat4 parentTransform = m_CurrentScene->GetWorldSpaceTransformMatrix(parent);
					rawTransform = glm::inverse(parentTransform) * rawTransform;

					glm::vec3 translation, rotation, scale;
					Decompose(rawTransform, translation, scale, rotation);

					glm::vec3 deltaRotation = rotation - entityTransform.GetRotation();
					entityTransform.SetPosition(translation);
					entityTransform.SetRotation(entityTransform.GetRotation() + deltaRotation);
					entityTransform.SetScale(scale);
					selection.Entity.SetTransform(entityTransform);
				}
				else
				{
					glm::vec3 translation, rotation, scale;
					Decompose(rawTransform, translation, scale, rotation);

					glm::vec3 deltaRotation = rotation - entityTransform.GetRotation();
					entityTransform.SetPosition(translation);
					entityTransform.SetRotation(entityTransform.GetRotation() + deltaRotation);
					entityTransform.SetScale(scale);
					selection.Entity.SetTransform(entityTransform);
				}
			}
		}
		else
		{
			glm::mat4 transformBase = rawTransform * selection.Mesh->LocalTransform.GetTransform();
			ImGuizmo::Manipulate(glm::value_ptr(m_EditorCamera.GetViewMatrix()), glm::value_ptr(m_EditorCamera.GetProjection()), (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transformBase), nullptr, snap ? snapValues : nullptr);

			Transform newTransform;
			newTransform.SetTransform(glm::inverse(rawTransform) * transformBase);
			selection.Mesh->LocalTransform = newTransform;
			selection.Entity.SetTransform(entityTransform);
		}
	}

	// TODO: Add Drag- and drop onto viewport

	UI::EndViewport();

	m_AssetBrowserPanel->OnUIRender();
	m_SceneHierarchyPanel->OnUIRender();
	m_EditorConsolePanel->OnUIRender(&m_ShowConsolePanel);
	m_ObjectPropertiesPanel->OnUIRender(&m_ShowObjectPropertiesPanel);
	m_SettingsPanel->OnUIRender(&m_ShowSettingsPanel);
	
	m_ViewportRenderer->OnUIRender();

	AssetManager::Get()->OnUIRender(m_AssetManagerPanelOpen);

	UI::EndWindow();
}

void HighLoEditor::OnResize(uint32 width, uint32 height)
{
	m_OverlayCamera.OnWindowResize(width, height);
	m_EditorCamera.OnWindowResize(width, height);
}

void HighLoEditor::SelectEntity(Entity entity)
{
	if (!entity)
	{
		// Invalid entity, de-select everything
		m_SelectionContext.clear();
		m_CurrentScene->SetSelectedEntity({});
		m_ObjectPropertiesPanel->SetSelected({});
		return;
	}

	SelectedMesh selection;
	if (entity.HasComponent<DynamicModelComponent>())
	{
		DynamicModelComponent *meshComp = entity.GetComponent<DynamicModelComponent>();
		Ref<DynamicModel> model = AssetManager::Get()->GetAsset<DynamicModel>(meshComp->Model);
		if (model && !model->IsFlagSet(AssetFlag::Missing))
			selection.Mesh = &model->Get()->GetSubmeshes()[0];

		selection.MeshIndex = meshComp->SubmeshIndex;
	}
	else if (entity.HasComponent<StaticModelComponent>())
	{
		StaticModelComponent *meshComp = entity.GetComponent<StaticModelComponent>();
		Ref<StaticModel> model = AssetManager::Get()->GetAsset<StaticModel>(meshComp->Model);
		if (model && !model->IsFlagSet(AssetFlag::Missing))
			selection.Mesh = &model->Get()->GetSubmeshes()[0];

		selection.MeshIndex = 0;
	}

	selection.Entity = entity;
	m_SelectionContext.clear();
	m_SelectionContext.push_back(selection);

	m_EditorScene->SetSelectedEntity(entity);
	m_ObjectPropertiesPanel->SetSelected(entity);
	m_CurrentScene = m_EditorScene;
}

void HighLoEditor::UpdateWindowTitle(const HLString &sceneName)
{
	HLString mode = "Debug";
#ifdef HL_RELEASE
	mode = "Release";
#endif // HL_RELEASE

	HLString title = sceneName + " - HighLo Engine - Renderer: " + Renderer::GetCurrentRenderingAPI() + " (" + mode + ")";
	GetWindow().SetTitle(title);
}

void HighLoEditor::NewScene(FileMenu *menu, MenuItem *item)
{
	HL_TRACE("NewScene");
	UpdateWindowTitle(m_SceneName + " *");
}

void HighLoEditor::OpenScene(FileMenu *menu, MenuItem *item)
{
	HL_TRACE("TEST OPEN");
	Ref<FileDialogue> fd = FileDialogue::Create();
	FileDialogueFilter filter;
	filter.AddFilter("HighLo Scene file", "*.hl");
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
	m_SceneIsSaved = true;

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
	m_SceneIsSaved = true;

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
				m_EditorCamera.Focus(selectedEntity.Transform().GetPosition());
				break;
			}

			case HL_KEY_Q:
			{
				m_GizmoType = editorutils::ConvertToImGuizmoType(GizmoType::None);
				break;
			}

			case HL_KEY_W:
			{
				m_GizmoType = editorutils::ConvertToImGuizmoType(GizmoType::Translate);
				break;
			}

			case HL_KEY_E:
			{
				m_GizmoType = editorutils::ConvertToImGuizmoType(GizmoType::Rotate);
				break;
			}

			case HL_KEY_R:
			{
				m_GizmoType = editorutils::ConvertToImGuizmoType(GizmoType::Scale);
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
			m_ShowSettingsPanel = !m_ShowSettingsPanel;
			break;
		}

		case MENU_ITEM_ASSET_CREATE_CAMERA:
		{
			Entity e = m_CurrentScene->CreateEntity("Camera");
			e.AddComponent<CameraComponent>();
			m_CurrentScene->SetSelectedEntity(e);
			OnEntityAdded(e);
			break;
		}

		case MENU_ITEM_ASSET_CREATE_CAPSULE:
		{
			Entity e = m_CurrentScene->CreateEntity("Capsule");
			StaticModelComponent *component = e.AddComponent<StaticModelComponent>();
			component->Model = AssetFactory::CreateCapsule(4.0f, 8.0f);
			m_CurrentScene->SetSelectedEntity(e);
			OnEntityAdded(e);
			break;
		}

		case MENU_ITEM_ASSET_CREATE_CUBE:
		{
			Entity e = m_CurrentScene->CreateEntity("Cube");
			StaticModelComponent *component = e.AddComponent<StaticModelComponent>();
			component->Model = AssetFactory::CreateCube({ 1.0f, 1.0f, 1.0f });
			m_CurrentScene->SetSelectedEntity(e);
			OnEntityAdded(e);
			break;
		}

		case MENU_ITEM_ASSET_CREATE_SPHERE:
		{
			Entity e = m_CurrentScene->CreateEntity("Sphere");
			StaticModelComponent *component = e.AddComponent<StaticModelComponent>();
			component->Model = AssetFactory::CreateSphere(4.0f);
			m_CurrentScene->SetSelectedEntity(e);
			OnEntityAdded(e);
			break;
		}

		case MENU_ITEM_ASSET_CREATE_CYLINDER:
		{
			Entity e = m_CurrentScene->CreateEntity("Cylinder");
			StaticModelComponent *component = e.AddComponent<StaticModelComponent>();
			// TODO: Add Cylinders to AssetFactory and MeshFactory
		//	component->Model = AssetFactory::CreateCylinder();
			m_CurrentScene->SetSelectedEntity(e);
			OnEntityAdded(e);
			break;
		}

		case MENU_ITEM_ASSET_CREATE_FOLDER:
		{
			FileSystem::Get()->CreateFolder(Project::GetAssetDirectory() / "New Folder");
			break;
		}

		case MENU_ITEM_ASSET_CREATE_NULL_OBJECT:
		{
			Entity e = m_CurrentScene->CreateEntity("Null Object");
			e.SetAsNullObject();
			m_CurrentScene->SetSelectedEntity(e);
			OnEntityAdded(e);
			break;
		}

		case MENU_ITEM_WINDOW_EDITOR_CONSOLE:
		{
			m_ShowConsolePanel = !m_ShowConsolePanel;
			break;
		}

		case MENU_ITEM_WINDOW_ASSET_MANAGER:
		{
			m_AssetManagerPanelOpen = !m_AssetManagerPanelOpen;
			break;
		}
	}
}

void HighLoEditor::OnSelected(const SelectedMesh &selectionContext)
{
	m_SceneHierarchyPanel->SetSelected(selectionContext.Entity);
	m_ObjectPropertiesPanel->SetSelected(selectionContext.Entity);
	m_EditorScene->SetSelectedEntity(selectionContext.Entity);
}

void HighLoEditor::OnEntityAdded(Entity &e)
{
	m_SceneIsSaved = false;
}

void HighLoEditor::OnEntityDeleted(Entity &e)
{
	m_SceneIsSaved = false;

	if (m_SelectionContext.size() > 0 && m_SelectionContext[0].Entity == e)
	{
		m_SelectionContext.clear();
		m_EditorScene->SetSelectedEntity({});
		m_ObjectPropertiesPanel->SetSelected({});
	}
}

void HighLoEditor::OnEntityChanged(Entity &e)
{
	m_SceneIsSaved = false;
	m_EditorScene->SetSelectedEntity(e);
	m_SceneHierarchyPanel->SetSelected(e);
}

void HighLoEditor::OnScenePlay()
{
	m_SelectionContext.clear();

	m_SceneState = SceneState::Play;
	UI::SetMouseEnabled(true);
	Input::SetCursorMode(CursorMode::Normal);

	// Copy current scene to be able to reset after stop button has been pressed
	m_RuntimeScene = Ref<Scene>::Create();
	m_EditorScene->CopyTo(m_RuntimeScene);

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
	GizmoType type = editorutils::ConvertFromImGuizmoType(m_GizmoType);
	switch (type)
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
	m_SceneIsSaved = false;
}

