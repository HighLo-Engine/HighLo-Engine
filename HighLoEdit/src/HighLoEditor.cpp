#include "HighLoEditor.h"

#include "MenuItems.h"

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

	GetWindow().Maximize();
	GetWindow().SetWindowIcon("assets/Resources/HighLoEngine.png");
	UpdateWindowTitle("Untitled Scene");

	FileSystem::StartWatching();

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

	Ref<FileMenu> windowMenu = FileMenu::Create("Window");
	bool darkThemeActive = UI::GetCurrentWindowStyle() == UI::ImGuiWindowStyle::Dark;
	bool lightThemeActive = UI::GetCurrentWindowStyle() == UI::ImGuiWindowStyle::Light;

	windowMenu->AddMenuItem("Set Dark Theme", "", MENU_ITEM_DARK_THEME, [=](FileMenu *menu, MenuItem *item) { EnableDarkMode(menu, item); }, !darkThemeActive);
	windowMenu->AddMenuItem("Set Light Theme", "", MENU_ITEM_LIGHT_THEME, [=](FileMenu *menu, MenuItem *item) { EnableLightMode(menu, item); }, !lightThemeActive);

	Ref<FileMenu> helpMenu = FileMenu::Create("Help");
	helpMenu->AddMenuItem("About HighLo", "", MENU_ITEM_ABOUT, [=](FileMenu *menu, MenuItem *item) { VirtualFileSystem::Get()->OpenInBrowser("https://www.highlo-engine.com"); });
	helpMenu->AddMenuItem("Documentation", "", MENU_ITEM_DOCUMENTATION, [=](FileMenu *menu, MenuItem *item) { VirtualFileSystem::Get()->OpenInBrowser("https://www.highlo-engine.com"); });

	m_MenuBar->AddMenu(fileMenu);
	m_MenuBar->AddMenu(editMenu);
	m_MenuBar->AddMenu(windowMenu);
	m_MenuBar->AddMenu(helpMenu);
	GetWindow().SetMenuBar(m_MenuBar);

	// TEMP
	m_ViewportWidth = width;
	m_ViewportHeight = height;
	m_Camera = Ref<EditorCamera>::Create(glm::perspectiveFov(glm::radians(90.0f), (float)width, (float)height, 0.1f, 1000.0f));

	FramebufferSpecification spec;
	spec.Width = width;
	spec.Height = height;
	spec.Attachments = { TextureFormat::RGBA };
	spec.ClearColor = { 0.5f, 0.1f, 0.1f, 1.0f };
	m_ViewportContent = Framebuffer::Create(spec);

	Renderer::SetBlendMode(false);
}

void HighLoEditor::OnUpdate(Timestep timestep)
{
	// Render Framebuffer
	m_ViewportContent->Bind();

	m_Camera->Update();

	Renderer::ClearScreenColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Renderer::ClearScreenBuffers();

	m_ViewportContent->Unbind();

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
}

void HighLoEditor::OnEvent(Event &e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<KeyPressedEvent>(HL_BIND_EVENT_FUNCTION(HighLoEditor::OnKeyPressedEvent));
	dispatcher.Dispatch<MouseButtonPressedEvent>(HL_BIND_EVENT_FUNCTION(HighLoEditor::OnMouseButtonPressedEvent));
	
	if (m_Camera)
		m_Camera->OnEvent(e);

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

	// Render viewport image
	auto viewportSize = UI::GetContentRegion();
	m_ViewportWidth = (uint32)viewportSize.x;
	m_ViewportHeight = (uint32)viewportSize.y;
	if (viewportSize.x > 0.0f && viewportSize.y > 0.0f)
	{
		m_Camera->SetProjection(glm::perspectiveFov(glm::radians(90.0f), viewportSize.x, viewportSize.y, 0.1f, 1000.0f));
		m_Camera->SetViewportSize((uint32)viewportSize.x, (uint32)viewportSize.y);
	}

	UI::BeginViewport("Viewport");
	UI::Image(m_ViewportContent->GetImage().As<Texture2D>(), { viewportSize.x, viewportSize.y }, { 0, 1 }, { 1, 0 }, { 1, 1, 1, 1 }, { 0, 0, 0, 0 });
	UI::EndViewport();

	UI::BeginViewport("Assets");
	UI::EndViewport();

	UI::BeginViewport("SceneHierarchyPanel");
	UI::EndViewport();

	UI::BeginViewport("Object Properties");
	UI::EndViewport();

	UI::EndWindow();
}

void HighLoEditor::OnResize(uint32 width, uint32 height)
{
	m_ViewportWidth = width;
	m_ViewportHeight = height;

	if (m_Camera && m_ViewportWidth > 0 && m_ViewportHeight > 0)
	{
		m_Camera->SetProjection(glm::perspectiveFov(glm::radians(90.0f), (float)m_ViewportWidth, (float)m_ViewportHeight, 0.1f, 1000.0f));
	}
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

				Entity selectedEntity = m_SelectionContext[0].m_Entity;
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

				Entity selectedEntity = m_SelectionContext[0].m_Entity;
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

void HighLoEditor::OnSelected(const HighLoEditor::SelectedMesh &selectionContext)
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

	m_EditorScene->CopyTo(m_RuntimeScene);
	m_RuntimeScene->OnRuntimeStart();
	//m_SceneHierarchy->SetContext(m_RuntimeScene);

	m_CurrentScene = m_RuntimeScene;
}

void HighLoEditor::OnSceneStop()
{
	m_RuntimeScene->OnRuntimeStop();
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
	m_EditorScene->CopyTo(m_SimulationScene);

	m_SimulationScene->OnSimulationStart();
	//m_SceneHierarchy->SetContext(m_SimulationScene);
	m_CurrentScene = m_SimulationScene;
}

void HighLoEditor::OnSceneEndSimulation()
{
	m_SimulationScene->OnSimulationEnd();
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
	// TODO
}

