#pragma once

#include "Panel.h"

class ViewportPanel : public Panel
{
public:

	virtual void Initialize(uint32 width, uint32 height) override;
	virtual void Destroy() override;
	virtual void Update(Timestep ts) override;
	virtual void Render(Timestep ts) override;
	virtual void OnEvent(Event &e) override;
	virtual void OnResize(uint32 width, uint32 height) override;

	virtual bool IsMouseOver() const override { return m_ViewportPanelMouseOver; }
	virtual bool IsSelected() const override { return m_ViewportPanelFocused; }

	void Focus(const glm::vec3 &translation);

	const glm::vec2 &GetViewportMinBounds() const { return m_ViewportBounds[0]; }
	const glm::vec2 &GetViewportMaxBounds() const { return m_ViewportBounds[1]; }
	uint32 GetViewportWidth() const { return m_ViewportWidth; }
	uint32 GetViewportHeight() const { return m_ViewportHeight; }
	bool SkipCameraEvents() const { return !m_AllowViewportCameraEvents; }

	std::pair<float, float> GetMouseViewportSpace();
	std::pair<glm::vec3, glm::vec3> CastRay(float mx, float my);
	Ray CastMouseRay();

private:

	Ref<EditorCamera> m_Camera;
	Ref<Framebuffer> m_ViewportContent;
	uint32 m_ViewportWidth = 0, m_ViewportHeight = 0;
	glm::vec2 m_ViewportBounds[2];

	bool m_AllowViewportCameraEvents = false;
	bool m_ViewportPanelMouseOver = false;
	bool m_ViewportPanelFocused = false;

	// TEMP: this is just to render the content in the viewport -> will be replaced with a scene
	void CreatePBRObjects();
	Light m_Light;
	Ref<Environment> m_Environment;
	Ref<Skybox> m_Skybox;
	Ref<Mesh> m_PBR_Sphere;
	Ref<Mesh> m_PBR_Gun;
	Ref<Mesh> m_Pistol;
	Ref<Mesh> m_Cowboy;
	std::vector<Ref<Mesh>> m_TestSpheres;
};

