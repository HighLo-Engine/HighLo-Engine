#pragma once

#include "Panel.h"

class SettingsViewerPanel : public Panel
{
public:

	virtual void Initialize(uint32 width, uint32 height) override;
	virtual void Destroy() override;
	virtual void Update(Timestep ts) override;
	virtual void Render(Timestep ts) override;
	virtual void OnEvent(Event &e) override;
	virtual void OnResize(uint32 width, uint32 height) override;

	virtual bool IsMouseOver() const override { return m_IsOver; }
	virtual bool IsSelected() const override { return m_IsSelected; }

private:

	bool m_IsOver = false;
	bool m_IsSelected = false;
};