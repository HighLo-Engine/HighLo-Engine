#pragma once

#include <HighLo.h>
using namespace highlo;

class SponzaSceneTest : public HLApplication
{
public:

	virtual void OnInitialize() override;
	virtual void OnUpdate(Timestep timestep) override;
	virtual void OnShutdown() override;
	virtual void OnEvent(Event &e) override;
	virtual void OnUIRender(Timestep timestep) override;
	virtual void OnResize(uint32 width, uint32 height) override;

private:

	Ref<Model> m_SponzaMesh;
	Ref<Environment> m_Environment;
	Ref<Skybox> m_Skybox;
	Ref<FreeFlyCamera> m_Camera;
};