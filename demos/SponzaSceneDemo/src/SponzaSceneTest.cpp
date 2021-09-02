#include "SponzaSceneTest.h"

void SponzaSceneTest::OnInitialize()
{
	m_Camera = Ref<FreeFlyCamera>::Create();

	m_Environment = Environment::Create("assets/textures/PBR_Scene_Apartment.hdr");
	m_Skybox = Skybox::Create();
	m_Skybox->SetTexture(m_Environment->GetSkyboxTexture());

	m_SponzaModel = AssetLoader::LoadStaticModel("assets/models/sponza.obj");
	for (int32 i = 0; i < m_SponzaModel->GetMeshCount(); ++i)
	{
		m_SponzaModel->GetMesh(i)->SetTexture(HL_MATERIAL_TEXTURE_IRRADIANCE_MAP, m_Environment->GetIrradianceMap());
		m_SponzaModel->GetMesh(i)->SetTexture(HL_MATERIAL_TEXTURE_PREFILTER_MAP, m_Environment->GetRadianceMap());
		m_SponzaModel->GetMesh(i)->SetTexture(HL_MATERIAL_TEXTURE_BRDF_MAP, Renderer::GetBRDFLutTexture());
	}

	HL_TRACE("SponzaSceneTest Initialized");
}

void SponzaSceneTest::OnUpdate(Timestep timestep)
{
	m_Camera->Update();

	for (int32 i = 0; i < m_SponzaModel->GetMeshCount(); ++i)
	{
		m_SponzaModel->GetMesh(i)->Update(timestep);
	}

	Renderer::ClearScreenBuffers();
	Renderer::ClearScreenColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

	m_Skybox->Render(*m_Camera);
	CoreRenderer::BeginScene(*m_Camera);

	for (int32 i = 0; i < m_SponzaModel->GetMeshCount(); ++i)
	{
		CoreRenderer::DrawMesh(m_SponzaModel->GetMesh(i), Transform::FromPosition({ 0, 0, 0 }));
	}
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

