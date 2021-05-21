#pragma once

#include <HighLo.h>
using namespace highlo;

class HighLoEditor : public HLApplication
{
public:

	HighLoEditor();
	virtual ~HighLoEditor();

	virtual void OnInitialize() override;
	virtual void OnUpdate(Timestep timestep) override;
	virtual void OnShutdown() override;
	virtual void OnEvent(Event &e) override;
	virtual void OnUIRender(Timestep timestep) override;
	virtual void OnResize(uint32 width, uint32 height);

	void UpdateWindowTitle(const HLString &sceneName);

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

	HLString m_SceneFilePath;
	glm::vec2 m_ViewportBounds[2];

	float m_EnvironmentMapRotation = 0.0f;
	float m_SnapValue = 0.5f;
	float m_RotationSnapValue = 45.0f;
	bool m_AllowViewportCameraEvents = false;
	bool m_DrawOnTopBoundingBoxes = false;

	bool m_UIShowBoundingBoxes = false;
	bool m_UIShowBoundingBoxesOnTop = false;

	bool m_ViewportPanelMouseOver = false;
	bool m_ViewportPanelFocused = false;

	Ref<Texture> m_CheckerboardTex;
	Ref<Texture> m_PlayButtonTex, m_StopButtonTex, m_PauseButtonTex;

	SceneState m_SceneSate = SceneState::Edit;
	SelectionMode m_SelectionMode = SelectionMode::Model;

	glm::mat4 *m_RelativeTransform = nullptr;
	glm::mat4 *m_CurrentlySelectedTransform = nullptr;


};
