#include "SponzaSceneTest.h"

void SponzaSceneTest::OnInitialize()
{
	m_Camera = Ref<FreeFlyCamera>::Create();

	m_Environment = Environment::Create("assets/textures/PBR_Scene_Apartment.hdr");
	m_Skybox = Skybox::Create();
	m_Skybox->SetTexture(m_Environment->GetSkyboxTexture());

	m_SponzaMesh = AssetLoader::LoadStaticModel("assets/models/sponza.obj");

	HL_TRACE("SponzaSceneTest Initialized");
}

void SponzaSceneTest::OnUpdate(Timestep timestep)
{
	m_Camera->Update();

	m_Skybox->Render(*m_Camera);
	for (int32 i = 0; i < m_SponzaMesh->GetMeshCount(); ++i)
	{
		m_SponzaMesh->GetMesh(i)->Update(timestep);
	}

	Renderer::ClearScreenBuffers();
	Renderer::ClearScreenColor(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));

	CoreRenderer::BeginScene(*m_Camera);
	
}

void SponzaSceneTest::OnShutdown()
{
	HL_TRACE("SponzaSceneTest shutdown");
}

void SponzaSceneTest::OnEvent(Event &e)
{
}

void SponzaSceneTest::OnUIRender(Timestep timestep)
{
}

void SponzaSceneTest::OnResize(uint32 width, uint32 height)
{
	m_Camera->OnWindowResize(width, height);
}

