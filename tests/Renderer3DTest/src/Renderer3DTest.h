#pragma once

#include <HighLo.h>
using namespace highlo;

class Renderer3DTest : public HLApplication
{
public:
	void OnInitialize() override;
	void OnUpdate(Timestep timestep) override;
	void OnShutdown() override;
	void OnEvent(Event &e) override;

private:

	Ref<Camera> m_Camera;
	Ref<Mesh> m_Mesh;
};

