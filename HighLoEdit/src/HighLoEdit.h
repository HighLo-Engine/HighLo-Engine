#pragma once

#include <HighLo.h>
using namespace highlo;

#include "Panels/ViewportPanel.h"
#include "Panels/AssetsPanel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ModelViewerPanel.h"
#include "Panels/MaterialViewerPanel.h"
#include "Panels/ObjectPropertiesPanel.h"

class HighLoEditor : public HLApplication
{
public:

	virtual void OnInitialize() override;
	virtual void OnUpdate(Timestep timestep) override;
	virtual void OnShutdown() override;
	virtual void OnEvent(Event &e) override;
	virtual void OnUIRender(Timestep timestep) override;
	virtual void OnResize(uint32 width, uint32 height);

	void SelectEntity(Entity entity);

	void UpdateWindowTitle(const HLString &sceneName);
	void NewScene();
	void OpenScene();
	void OpenScene(const HLString &path);
	void SaveScene();
	void SaveSceneAs();

	enum class GizmoType
	{
		None = 0,
		Translate = 1,
		Rotate = 2,
		Scale = 3
	};

private:

	struct SelectedMesh
	{
		float m_Distance = 0.0f;
		Mesh *m_Mesh = nullptr;
		Entity m_Entity;
	};

	enum class SceneState
	{
		Edit		= 0,
		Play		= 1,
		Pause		= 2,
		Simulate	= 3
	};

	enum class SelectionMode
	{
		None		= 0,
		Model		= 1,
		Mesh		= 2
	};

	bool OnKeyPressedEvent(const KeyPressedEvent &e);
	bool OnMouseButtonPressedEvent(const MouseButtonPressedEvent &e);

	void OnSelected(const SelectedMesh &selectionContext);
	void OnEntityDeleted(Entity e);

	void OnScenePlay();
	void OnSceneStop();

	void OnSceneStartSimulation();
	void OnSceneEndSimulation();

	float GetSnapValue();
	void DeleteEntity(Entity entity);

private:

	UniqueRef<ViewportPanel> m_Viewport;
	UniqueRef<AssetsPanel> m_Assets;
	UniqueRef<SceneHierarchyPanel> m_SceneHierarchy;
	UniqueRef<ObjectPropertiesPanel> m_ObjectProperties;

	HLString m_LastSceneFilePath;
	GizmoType m_GizmoType = GizmoType::None;

	float m_EnvironmentMapRotation = 0.0f;
	float m_SnapValue = 0.5f;
	float m_RotationSnapValue = 45.0f;

	bool m_AllowViewportCameraEvents = false;
	bool m_DrawOnTopBoundingBoxes = false;
	bool m_SceneIsSaved = false;

	bool m_UIShowBoundingBoxes = false;
	bool m_UIShowBoundingBoxesOnTop = false;

	Ref<Texture> m_CheckerboardTex;
	Ref<Texture> m_PlayButtonTex, m_StopButtonTex, m_PauseButtonTex;
	Ref<MenuBar> m_MenuBar;

	SceneState m_SceneState = SceneState::Edit;
	SelectionMode m_SelectionMode = SelectionMode::Model;

	glm::mat4 *m_RelativeTransform = nullptr;
	glm::mat4 *m_CurrentlySelectedTransform = nullptr;

	Ref<Scene> m_RuntimeScene, m_EditorScene, m_SimulationScene, m_CurrentScene;
	std::vector<SelectedMesh> m_SelectionContext;
};

