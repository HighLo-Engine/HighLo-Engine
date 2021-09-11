#pragma once

#include <HighLo.h>
using namespace highlo;

class PBRSceneDemo : public HLApplication
{
public:

	virtual void OnInitialize() override;
	virtual void OnUpdate(Timestep timestep) override;
	virtual void OnShutdown() override;
	virtual void OnEvent(Event &e) override;
	virtual void OnResize(uint32 width, uint32 height) override;
	virtual void OnUIRender(Timestep timestep) override;

private:

	void CreatePBRObjects();

	uint32 GetVertexCountFromModel(const Ref<Model> &model);
	uint32 GetVertexCountFromMesh(Ref<Mesh> &mesh);

	Light m_Light;
	Ref<FreeFlyCamera> m_Camera;
	Ref<Environment> m_Environment;
	Ref<Skybox> m_Skybox;

	Ref<Mesh> m_PBR_Sphere;
	Ref<Mesh> m_PBR_Gun;
	Ref<Mesh> m_Pistol;
	Ref<Mesh> m_Cowboy;
	Ref<Mesh> m_DancingStormTrooper;

	uint32 m_VertexCount = 0;
	uint32 m_IndexCount = 0;
	std::vector<Ref<Mesh>> m_TestSpheres;
};

