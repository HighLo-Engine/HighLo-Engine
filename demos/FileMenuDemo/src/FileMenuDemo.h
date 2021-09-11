#pragma once

#include <HighLo.h>
using namespace highlo;

class FileMenuDemo : public HLApplication
{
public:

	virtual void OnInitialize() override;
	virtual void OnUpdate(Timestep timestep) override;
	virtual void OnShutdown() override;
	virtual void OnEvent(Event &e) override;
	virtual void OnUIRender(Timestep timestep) override;
	virtual void OnResize(uint32 width, uint32 height) override;

private:

	Ref<MenuBar> m_Menubar;
};