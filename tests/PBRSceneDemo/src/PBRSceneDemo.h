#pragma once

#include <HighLo.h>
using namespace highlo;

class PBRSceneDemo : public HLApplication
{
public:
	void OnInitialize() override;
	void OnUpdate(Timestep timestep) override;
	void OnShutdown() override;
	void OnEvent(Event &e) override;
	void OnResize(uint32 width, uint32 height) override;

private:
	Light m_Light;
	Ref<Camera> m_Camera;
	Ref<Environment> m_Environment;
	Ref<Skybox> m_Skybox;

	Ref<Mesh> m_PBR_Sphere;
	Ref<Mesh> m_PBR_Gun;
	Ref<Mesh> m_Pistol;
	Ref<Mesh> m_Cowboy;

	std::vector<Ref<Mesh>> m_TestSpheres;

	void CreatePBRObjects();
};

