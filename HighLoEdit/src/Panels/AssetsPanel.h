#pragma once

#include "Panel.h"

class AssetsPanel : public Panel
{
public:

	virtual void Initialize(uint32 width, uint32 height) override;
	virtual void Destroy() override;
	virtual void Update(Timestep ts) override;
	virtual void Render(Timestep ts) override;
	virtual void OnEvent(Event &e) override;
	virtual void OnResize(uint32 width, uint32 height) override;

	virtual bool IsMouseOver() const override { return m_IsHovered; }
	virtual bool IsSelected() const override { return m_IsSelected; }

private:

	Ref<PopupMenu> m_Popup;
	bool m_IsHovered = false;
	bool m_IsSelected = false;
};

