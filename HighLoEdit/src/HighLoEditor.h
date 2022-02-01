// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.1 (2021-10-04) refactored watcher functions from FileSystem to FileSystemWatcher class
//     - 1.0 (2021-09-15) initial release
//

#pragma once

#include <HighLo.h>
using namespace highlo;

class HighLoEditor : public HLApplication
	{
	public:

		HighLoEditor(const ApplicationSettings &settings);
		~HighLoEditor();

		virtual void OnInitialize() override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnShutdown() override;
		virtual void OnEvent(Event &e) override;
		virtual void OnUIRender(Timestep ts) override;
		virtual void OnResize(uint32 width, uint32 height) override;

		void UpdateUIFlags();

		void SelectEntity(Entity entity);

		void UpdateWindowTitle(const HLString &sceneName);
		void NewScene(FileMenu *menu, MenuItem *item);
		void OpenScene(FileMenu *menu, MenuItem *item);
		void OpenScene(const HLString &path);
		void SaveScene(FileMenu *menu, MenuItem *item);
		void SaveSceneAs(FileMenu *menu, MenuItem *item);

		enum class GizmoType
		{
			None		= 0,
			Translate	= 1,
			Rotate		= 2,
			Scale		= 3
		};

	private:

		struct SelectedMesh
		{
			float Distance = 0.0f;
			Mesh *Mesh = nullptr;
			uint32 MeshIndex = 0;
			Entity Entity;
		};

		enum class SceneState
		{
			Edit	 = 0,
			Play	 = 1,
			Pause	 = 2,
			Simulate = 3
		};

		enum class SelectionMode
		{
			None   = 0,
			Entity = 1,
			Mesh   = 2
		};

		bool OnKeyPressedEvent(const KeyPressedEvent &e);
		bool OnMouseButtonPressedEvent(const MouseButtonPressedEvent &e);
		void OnFileMenuPressed(FileMenu *menu, MenuItem *item);

		void OnSelected(const SelectedMesh &selectionContext);
		void OnEntityDeleted(Entity e);
		void OnObjectPropertiesChange(ObjectPropertiesActionType type);

		void OnScenePlay();
		void OnSceneStop();

		void OnSceneStartSimulation();
		void OnSceneEndSimulation();

		float GetSnapValue();
		void DeleteEntity(Entity entity);

	private:
		glm::vec4 m_ClearColor = { 0.4f, 0.5f, 0.4f, 1 };

		HLString m_ProjectPath;
		FileSystemPath m_RoamingPath;
		Ref<UserSettings> m_UserSettings;

		HLString m_LastSceneFilePath;
		int32 m_GizmoType = -1;
		EditorCamera m_EditorCamera;
		Camera m_OverlayCamera;

		float m_EnvironmentMapRotation = 0.0f;
		float m_SnapValue = 0.5f;
		float m_RotationSnapValue = 45.0f;
		float m_LineWidth = 2.0f;

		bool m_AllowViewportCameraEvents = true;
		bool m_ViewportPanelMouseOver = false;
		bool m_ViewportPanelFocused = false;
		bool m_DrawOnTopBoundingBoxes = false;
		bool m_SceneIsSaved = false;

		bool m_ShowAssetsPanel = true;
		bool m_ShowSceneHierarchyPanel = true;
		bool m_ShowViewportPanel = true;
		bool m_ShowObjectPropertiesPanel = true;
		bool m_ShowConsolePanel = false;
		bool m_ShowSelectedWireframe = false;
		bool m_AssetManagerPanelOpen = false;

		bool m_UIShowBoundingBoxes = false;
		bool m_UIShowBoundingBoxesOnTop = false;

		Ref<Texture> m_CheckerboardTex;
		Ref<Texture> m_PlayButtonTex, m_StopButtonTex, m_PauseButtonTex;
		Ref<MenuBar> m_MenuBar;

		SceneState m_SceneState = SceneState::Edit;
		SelectionMode m_SelectionMode = SelectionMode::Entity;

		std::vector<SelectedMesh> m_SelectionContext;
		glm::mat4 *m_RelativeTransform = nullptr;
		glm::mat4 *m_CurrentlySelectedTransform = nullptr;
		glm::vec2 m_ViewportBounds[2];

		// Main displayable scenes
		Ref<Scene> m_RuntimeScene, m_EditorScene, m_SimulationScene, m_CurrentScene;

		// List of all scene references
		std::vector<Ref<Scene>> m_AllScenes;

		// Editor Panels
		Ref<SceneRenderer> m_ViewportRenderer;
		UniqueRef<SceneHierarchyPanel> m_SceneHierarchyPanel;
		UniqueRef<ObjectPropertiesPanel> m_ObjectPropertiesPanel;
		UniqueRef<EditorConsolePanel> m_EditorConsolePanel;
		UniqueRef<AssetBrowserPanel> m_AssetBrowserPanel;

		// File Menu Panels
		Ref<FileMenu> m_WindowMenu = nullptr;
	};

