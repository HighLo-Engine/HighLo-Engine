#pragma once

#include <HighLo.h>
using namespace highlo;

class Sandbox : public HLApplication
{
public:
	void OnInitialize() override;
	void OnUpdate(Timestep timestep) override;
	void OnShutdown() override;
	void OnEvent(Event& e) override;

private:
	Ref<FPSCamera> m_Camera;
	Ref<Mesh> m_Mesh;
};
