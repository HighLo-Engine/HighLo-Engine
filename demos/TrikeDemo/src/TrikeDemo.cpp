#include "TrikeDemo.h"

void TrikeDemo::OnInitialize()
{
	m_Camera = Ref<FreeFlyCamera>::Create();

	m_Environment = Environment::Create("assets/textures/PBR_Scene_Apartment.hdr");
	m_Skybox = Skybox::Create();
	m_Skybox->SetTexture(m_Environment->GetSkyboxTexture());

	auto mesh = AssetLoader::LoadStaticModel("assets/models/Test_Leg_Shield.obj");
	m_TrikeMesh = mesh->GetMesh(0);
	m_TrikeMesh->SetTexture(HL_MATERIAL_TEXTURE_ALBEDO, Renderer::GetWhiteTexture());
	m_TrikeMesh->SetTexture(HL_MATERIAL_TEXTURE_IRRADIANCE_MAP, m_Environment->GetIrradianceMap());
	m_TrikeMesh->SetTexture(HL_MATERIAL_TEXTURE_PREFILTER_MAP, m_Environment->GetRadianceMap());
	m_TrikeMesh->SetTexture(HL_MATERIAL_TEXTURE_BRDF_MAP, Renderer::GetBRDFLutTexture());
	m_TrikeMesh->GetMaterialRenderProperties().Metallic = 0.5f;
	m_TrikeMesh->GetMaterialRenderProperties().Roughness = 0.5f;

	HL_TRACE("TrikeDemo Initialized");
}

void TrikeDemo::OnUpdate(Timestep timestep)
{
	m_Camera->Update();
	m_TrikeMesh->Update(timestep);

	Renderer::ClearScreenBuffers();
	Renderer::ClearScreenColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	
	m_Skybox->Render(*m_Camera);
	CoreRenderer::BeginScene(*m_Camera);
	CoreRenderer::DrawMesh(m_TrikeMesh, Transform::FromPosition({ 0, 0, 0 }).Scale(0.125f));
}

void TrikeDemo::OnShutdown()
{
	HL_TRACE("TrikeDemo shutdown");
}

void TrikeDemo::OnEvent(Event &e)
{
}

void TrikeDemo::OnUIRender(Timestep timestep)
{
}

void TrikeDemo::OnResize(uint32 width, uint32 height)
{
	m_Camera->OnWindowResize(width, height);
}

