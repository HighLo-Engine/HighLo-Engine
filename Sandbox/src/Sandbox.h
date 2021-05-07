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
	void OnImGuiRender() override;
	void OnResize(uint32 width, uint32 height) override;

private:
	Light m_Light;
	Ref<FPSCamera> m_Camera;
	Ref<Environment> m_Environment;
	
	Ref<Mesh> m_PBR_Sphere;
};
