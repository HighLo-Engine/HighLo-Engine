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
	Ref<FPSCamera> m_Camera;
	Ref<Mesh> m_Mesh;

	Ref<Material> m;
	Ref<Texture> m_CabinTexture;
	Model m_CabinModel;
	Transform m_CabinTransform;

	Ref<Mesh> m_Sphere;
};
