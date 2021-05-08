#include "Sandbox.h"

void Sandbox::OnInitialize()
{
	// TO-DO: Fix for GLFW configuration.
	//HLApplication::Get().GetWindow().SetWindowIcon("assets/textures/HighLoEngine.ico");

	VirtualFileSystem::Get()->Mount("textures", "assets/textures/");
	VirtualFileSystem::Get()->Mount("models", "assets/models/");

	m_Camera = FPSCamera::Create();
	m_Camera->SetYaw(90);
	
	m_Environment = Environment::Create("assets/textures/PBR_Scene_Apartment.hdr");

	ImGuiRenderer::ShouldDisplayDebugInformation(true);

	CreatePBRObjects();

	m_Skybox = Skybox::Create();
	m_Skybox->SetTexture(m_Environment->GetSkyboxTexture());
	HL_TRACE("Sandbox Initialized");
}

void Sandbox::OnUpdate(Timestep timestep)
{
	m_Camera->Update();

	Renderer::ClearScreenBuffers();
	Renderer::ClearScreenColor(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));

	m_Skybox->Render(*m_Camera);
	CoreRenderer::BeginScene(*m_Camera, m_Light);

	static float y_rot = 0.0f;
	y_rot += 0.0004f * timestep;

	CoreRenderer::DrawCube(Transform::FromPosition(m_Light.Position));
	CoreRenderer::DrawMesh(m_PBR_Sphere, Transform::FromPosition({ 6, -2, 6 }).Scale(3.0f).Rotate(y_rot, { 0, 1, 0 }));
	CoreRenderer::DrawMesh(m_PBR_Gun, Transform::FromPosition({ 18, -6, 26 }).Scale(0.24f).Rotate(y_rot, { 0, 1, 0 }));

	for (uint64 i = 0; i < m_TestSpheres.size(); i++)
		CoreRenderer::DrawMesh(m_TestSpheres[i], Transform::FromPosition({ 6, -2, 14 + (float)i * 7 }).Scale(3.0f));

	CoreRenderer::EndScene();
}

void Sandbox::OnShutdown()
{
	HL_TRACE("Sandbox Shutdown");
}

void Sandbox::OnEvent(Event& e)
{
	if (e.IsInCategory(EventCategory::EventCategoryApplication))
		HL_TRACE(e.ToString());
}

void Sandbox::OnImGuiRender()
{
}

void Sandbox::OnResize(uint32 width, uint32 height)
{
	m_Camera->OnWindowResize(width, height);
}

void Sandbox::CreatePBRObjects()
{
	auto sampleBRDF = Texture2D::LoadFromFile("assets/textures/brdfMap.png");

	m_PBR_Sphere = AssetLoader::LoadStaticModel("assets/models/PBR_Sphere.dae").GetMesh(0);
	m_PBR_Sphere->m_Material->SetTexture(HL_MATERIAL_TEXTURE_ALBEDO, Texture2D::LoadFromFile("assets/textures/PBR_Sphere_Albedo.jpg"));
	m_PBR_Sphere->m_Material->SetTexture(HL_MATERIAL_TEXTURE_NORMAL, Texture2D::LoadFromFile("assets/textures/PBR_Sphere_Normal.jpg"));
	m_PBR_Sphere->m_Material->SetTexture(HL_MATERIAL_TEXTURE_METALLIC, Texture2D::LoadFromFile("assets/textures/PBR_Sphere_Metallic.jpg"));
	m_PBR_Sphere->m_Material->SetTexture(HL_MATERIAL_TEXTURE_ROUGHNESS, Texture2D::LoadFromFile("assets/textures/PBR_Sphere_Roughness.jpg"));
	m_PBR_Sphere->m_Material->SetTexture(HL_MATERIAL_TEXTURE_AMBIENT_OCCLUSION, Texture2D::CreateFromColor({ 255, 255, 255 }));
	m_PBR_Sphere->m_Material->SetTexture(HL_MATERIAL_TEXTURE_IRRADIANCE_MAP, m_Environment->GetIrradianceMap());
	m_PBR_Sphere->m_Material->SetTexture(HL_MATERIAL_TEXTURE_PREFILTER_MAP, m_Environment->GetRadianceMap());
	m_PBR_Sphere->m_Material->SetTexture(HL_MATERIAL_TEXTURE_BRDF_MAP, sampleBRDF);
	m_PBR_Sphere->m_Material->Properties.m_RenderProperties.m_Metallic = 1;
	m_PBR_Sphere->m_Material->Properties.m_RenderProperties.m_Roughness = 1;

	auto m_PBR_Gun_Model = AssetLoader::LoadStaticModel("assets/models/PBR_AK47.obj");
	m_PBR_Gun = m_PBR_Gun_Model.GetMesh(0);

	m_PBR_Gun->m_Material->SetTexture(HL_MATERIAL_TEXTURE_ALBEDO, Texture2D::LoadFromFile("assets/textures/AK47/AK47_albedo.tga"));
	m_PBR_Gun->m_Material->SetTexture(HL_MATERIAL_TEXTURE_NORMAL, Texture2D::LoadFromFile("assets/textures/AK47/AK47_normal.tga"));
	m_PBR_Gun->m_Material->SetTexture(HL_MATERIAL_TEXTURE_METALLIC, Texture2D::LoadFromFile("assets/textures/AK47/AK47_metalness.tga"));
	m_PBR_Gun->m_Material->SetTexture(HL_MATERIAL_TEXTURE_ROUGHNESS, Texture2D::LoadFromFile("assets/textures/AK47/AK47_roughness.tga"));
	m_PBR_Gun->m_Material->SetTexture(HL_MATERIAL_TEXTURE_AMBIENT_OCCLUSION, Texture2D::LoadFromFile("assets/textures/AK47/AK47_ao.tga"));
	m_PBR_Gun->m_Material->SetTexture(HL_MATERIAL_TEXTURE_IRRADIANCE_MAP, m_Environment->GetIrradianceMap());
	m_PBR_Gun->m_Material->SetTexture(HL_MATERIAL_TEXTURE_PREFILTER_MAP, m_Environment->GetRadianceMap());
	m_PBR_Gun->m_Material->SetTexture(HL_MATERIAL_TEXTURE_BRDF_MAP, sampleBRDF);
	m_PBR_Gun->m_Material->Properties.m_RenderProperties.m_Metallic = 1;
	m_PBR_Gun->m_Material->Properties.m_RenderProperties.m_Roughness = 1;

	for (int i = 0; i < 10; i++)
	{
		auto newSphere = MeshFactory::CreateSphere(1.0f);
		newSphere->m_Material->Properties.m_RenderProperties.m_Color = { (float)i * 0.02f, (float)i * 0.08f, (float)i * 0.05f, 1.0f };
		newSphere->m_Material->Properties.m_RenderProperties.m_Roughness = 0.1f * i;
		newSphere->m_Material->SetTexture(HL_MATERIAL_TEXTURE_IRRADIANCE_MAP, m_Environment->GetIrradianceMap());
		newSphere->m_Material->SetTexture(HL_MATERIAL_TEXTURE_PREFILTER_MAP, m_Environment->GetRadianceMap());
		newSphere->m_Material->SetTexture(HL_MATERIAL_TEXTURE_BRDF_MAP, sampleBRDF);

		if (i == 0)
		{
			newSphere->m_Material->Properties.m_RenderProperties.m_Color = glm::vec4(0.0f);
			newSphere->m_Material->Properties.m_RenderProperties.m_Roughness = 0;
		}
		if (i == 1)
		{
			newSphere->m_Material->Properties.m_RenderProperties.m_Color = glm::vec4(1.0f);
			newSphere->m_Material->Properties.m_RenderProperties.m_Roughness = 0;
		}

		m_TestSpheres.push_back(newSphere);
	}

	for (int i = 0; i < 10; i++)
	{
		auto newSphere = MeshFactory::CreateSphere(1.0f);
		newSphere->m_Material->Properties.m_RenderProperties.m_Color = { (float)i * 0.02f, (float)i * 0.08f, (float)i * 0.05f, 1.0f };
		newSphere->m_Material->Properties.m_RenderProperties.m_Metallic = 0.1f * i;
		newSphere->m_Material->SetTexture(HL_MATERIAL_TEXTURE_IRRADIANCE_MAP, m_Environment->GetIrradianceMap());
		newSphere->m_Material->SetTexture(HL_MATERIAL_TEXTURE_PREFILTER_MAP, m_Environment->GetRadianceMap());
		newSphere->m_Material->SetTexture(HL_MATERIAL_TEXTURE_BRDF_MAP, sampleBRDF);

		m_TestSpheres.push_back(newSphere);
	}
}
