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
	m_Pistol->Update(timestep);
	m_Cowboy->Update(timestep);

	m_Camera->Update();

	Renderer::ClearScreenBuffers();
	Renderer::ClearScreenColor(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));

	m_Skybox->Render(*m_Camera);
	CoreRenderer::BeginScene(*m_Camera, m_Light);

	static float y_rot = 0.0f;
	y_rot += 0.0004f * timestep;

	CoreRenderer::DrawCube(Transform::FromPosition(m_Light.Position));
	CoreRenderer::DrawMesh(m_PBR_Sphere, Transform::FromPosition({ 6, -2, 6 }).Scale(3.0f).Rotate(y_rot, { 0, 1, 0 }));
	CoreRenderer::DrawMesh(m_PBR_Gun, Transform::FromPosition({ 26, -6, 26 }).Scale(0.24f).Rotate(y_rot, { 0, 1, 0 }));

	for (uint64 i = 0; i < m_TestSpheres.size(); i++)
		CoreRenderer::DrawMesh(m_TestSpheres[i], Transform::FromPosition({ 6, -2, 14 + (float)i * 7 }).Scale(3.0f));

	CoreRenderer::DrawMesh(m_Pistol,		Transform::FromPosition({ 30, -3, 0 }).Scale(0.5f));
	CoreRenderer::DrawMesh(m_Cowboy,		Transform::FromPosition({ 26, -5, -12 }).Scale(0.8f).Rotate(-45.0f, {0, 1, 0}));

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

	m_PBR_Sphere = AssetLoader::LoadStaticModel("assets/models/PBR_Sphere.obj").GetMesh(0);
	m_PBR_Sphere->SetTexture(HL_MATERIAL_TEXTURE_ALBEDO, Texture2D::LoadFromFile("assets/textures/PBR_Sphere_Albedo.jpg"));
	m_PBR_Sphere->SetTexture(HL_MATERIAL_TEXTURE_NORMAL, Texture2D::LoadFromFile("assets/textures/PBR_Sphere_Normal.jpg"));
	m_PBR_Sphere->SetTexture(HL_MATERIAL_TEXTURE_METALLIC, Texture2D::LoadFromFile("assets/textures/PBR_Sphere_Metallic.jpg"));
	m_PBR_Sphere->SetTexture(HL_MATERIAL_TEXTURE_ROUGHNESS, Texture2D::LoadFromFile("assets/textures/PBR_Sphere_Roughness.jpg"));
	m_PBR_Sphere->SetTexture(HL_MATERIAL_TEXTURE_IRRADIANCE_MAP, m_Environment->GetIrradianceMap());
	m_PBR_Sphere->SetTexture(HL_MATERIAL_TEXTURE_PREFILTER_MAP, m_Environment->GetRadianceMap());
	m_PBR_Sphere->SetTexture(HL_MATERIAL_TEXTURE_BRDF_MAP, sampleBRDF);
	m_PBR_Sphere->GetMaterialRenderProperties().Metallic = 1;
	m_PBR_Sphere->GetMaterialRenderProperties().Roughness = 1;

	auto m_PBR_Gun_Model = AssetLoader::LoadStaticModel("assets/models/PBR_AK47.obj");
	m_PBR_Gun = m_PBR_Gun_Model.GetMesh(0);

	m_PBR_Gun->SetTexture(HL_MATERIAL_TEXTURE_ALBEDO, Texture2D::LoadFromFile("assets/textures/AK47/AK47_albedo.tga"));
	m_PBR_Gun->SetTexture(HL_MATERIAL_TEXTURE_NORMAL, Texture2D::LoadFromFile("assets/textures/AK47/AK47_normal.tga"));
	m_PBR_Gun->SetTexture(HL_MATERIAL_TEXTURE_METALLIC, Texture2D::LoadFromFile("assets/textures/AK47/AK47_metalness.tga"));
	m_PBR_Gun->SetTexture(HL_MATERIAL_TEXTURE_ROUGHNESS, Texture2D::LoadFromFile("assets/textures/AK47/AK47_roughness.tga"));
	m_PBR_Gun->SetTexture(HL_MATERIAL_TEXTURE_AMBIENT_OCCLUSION, Texture2D::LoadFromFile("assets/textures/AK47/AK47_ao.tga"));
	m_PBR_Gun->SetTexture(HL_MATERIAL_TEXTURE_IRRADIANCE_MAP, m_Environment->GetIrradianceMap());
	m_PBR_Gun->SetTexture(HL_MATERIAL_TEXTURE_PREFILTER_MAP, m_Environment->GetRadianceMap());
	m_PBR_Gun->SetTexture(HL_MATERIAL_TEXTURE_BRDF_MAP, sampleBRDF);
	m_PBR_Gun->GetMaterialRenderProperties().Metallic = 1;
	m_PBR_Gun->GetMaterialRenderProperties().Roughness = 1;

	auto SphereBlueprint = MeshFactory::CreateSphere(1.0f);
	SphereBlueprint->SetTexture(HL_MATERIAL_TEXTURE_IRRADIANCE_MAP, m_Environment->GetIrradianceMap());
	SphereBlueprint->SetTexture(HL_MATERIAL_TEXTURE_PREFILTER_MAP, m_Environment->GetRadianceMap());
	SphereBlueprint->SetTexture(HL_MATERIAL_TEXTURE_BRDF_MAP, sampleBRDF);

	for (int i = 0; i < 10; i++)
	{
		auto newSphere = SphereBlueprint->Copy();
		newSphere->GetMaterialRenderProperties().Color = { (float)i * 0.02f, (float)i * 0.08f, (float)i * 0.05f, 1.0f };
		newSphere->GetMaterialRenderProperties().Roughness = 0.1f * i;

		if (i == 0)
		{
			newSphere->GetMaterialRenderProperties().Color = glm::vec4(0.0f);
			newSphere->GetMaterialRenderProperties().Roughness = 0;
		}
		if (i == 1)
		{
			newSphere->GetMaterialRenderProperties().Color = glm::vec4(1.0f);
			newSphere->GetMaterialRenderProperties().Roughness = 0;
		}

		m_TestSpheres.push_back(newSphere);
	}

	for (int i = 0; i < 10; i++)
	{
		auto newSphere = SphereBlueprint->Copy();
		newSphere->GetMaterialRenderProperties().Color = { (float)i * 0.02f, (float)i * 0.08f, (float)i * 0.05f, 1.0f };
		newSphere->GetMaterialRenderProperties().Metallic = 0.1f * i;

		m_TestSpheres.push_back(newSphere);
	}

	m_Pistol = AssetLoader::LoadAnimatedModel("assets/models/Pistol.fbx");
	m_Pistol->SetTexture(HL_MATERIAL_TEXTURE_ALBEDO, Texture2D::LoadFromFile("assets/textures/Pistol.png"));
	m_Pistol->SetTexture(HL_MATERIAL_TEXTURE_IRRADIANCE_MAP, m_Environment->GetIrradianceMap());
	m_Pistol->SetTexture(HL_MATERIAL_TEXTURE_PREFILTER_MAP, m_Environment->GetRadianceMap());
	m_Pistol->SetTexture(HL_MATERIAL_TEXTURE_BRDF_MAP, sampleBRDF);
	m_Pistol->GetMaterialRenderProperties().Roughness = 0.4f;
	m_Pistol->GetMaterialRenderProperties().Metallic = 0.68f;

	m_Cowboy = AssetLoader::LoadAnimatedModel("assets/models/Cowboy.fbx", true);
	m_Cowboy->SetTexture(HL_MATERIAL_TEXTURE_ALBEDO, Texture2D::LoadFromFile("assets/textures/Cowboy.png"));
	m_Cowboy->m_Material->IsUsingPBR = false;

	m_Pistol->m_Animation->Play();
	m_Cowboy->m_Animation->Play();
}
