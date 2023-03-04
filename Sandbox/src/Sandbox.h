#pragma once

#include <HighLo.h>
using namespace highlo;

class Sandbox : public HLApplication
{
public:

	Sandbox(highlo::ApplicationSettings &settings);

	void OnInitialize() override;
	void OnUpdate(Timestep timestep) override;
	void OnShutdown() override;
	void OnEvent(Event &e) override;
	void OnUIRender(Timestep timestep) override;
	void OnResize(uint32 width, uint32 height) override;

private:

	Ref<RenderSystem> m_RenderSystemRef;
	Ref<Scene> m_Scene;
};

