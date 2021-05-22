#pragma once

#include <HighLo.h>
using namespace highlo;

#include "Panels/ViewportPanel.h"

class HighLoEditor : public HLApplication
{
public:

	virtual void OnInitialize() override;
	virtual void OnUpdate(Timestep timestep) override;
	virtual void OnShutdown() override;
	virtual void OnEvent(Event &e) override;
	virtual void OnUIRender(Timestep timestep) override;
	virtual void OnResize(uint32 width, uint32 height);

	void UpdateWindowTitle(const HLString &sceneName);
	void NewScene();
	void OpenScene();
	void OpenScene(const HLString &path);
	void SaveScene();
	void SaveSceneAs();

private:

	enum class SceneState
	{
		Edit = 0,
		Play = 1,
		Pause = 2,
		Simulate = 3
	};

	enum class SelectionMode
	{
		None = 0,
		Model = 1,
		Mesh = 2
	};

	bool OnKeyPressedEvent(const KeyPressedEvent &e);
	bool OnMouseButtonPressedEvent(const MouseButtonPressedEvent &e);

private:

	ViewportPanel m_Viewport;

	HLString m_LastSceneFilePath;

	float m_EnvironmentMapRotation = 0.0f;
	float m_SnapValue = 0.5f;
	float m_RotationSnapValue = 45.0f;
	bool m_AllowViewportCameraEvents = false;
	bool m_DrawOnTopBoundingBoxes = false;

	bool m_UIShowBoundingBoxes = false;
	bool m_UIShowBoundingBoxesOnTop = false;

	Ref<Texture> m_CheckerboardTex;
	Ref<Texture> m_PlayButtonTex, m_StopButtonTex, m_PauseButtonTex;
	Ref<MenuBar> m_MenuBar;

	SceneState m_SceneSate = SceneState::Edit;
	SelectionMode m_SelectionMode = SelectionMode::Model;

	glm::mat4 *m_RelativeTransform = nullptr;
	glm::mat4 *m_CurrentlySelectedTransform = nullptr;


};
