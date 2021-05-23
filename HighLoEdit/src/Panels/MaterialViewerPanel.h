#pragma once

#include "Panel.h"

class MaterialViewerPanel : public Panel
{
public:

	virtual void Initialize(uint32 width, uint32 height) override;
	virtual void Destroy() override;
	virtual void Update(Timestep ts) override;
	virtual void Render(Timestep ts) override;
	virtual void OnEvent(Event &e) override;
	virtual void OnResize(uint32 width, uint32 height) override;

	// TODO
	virtual bool IsMouseOver() const override { return false; }
	virtual bool IsSelected() const override { return false; }

private:


};

