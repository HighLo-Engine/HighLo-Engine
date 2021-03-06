#include "PBRSceneDemo.h"

void PBRSceneDemo::OnInitialize()
{
	Renderer::SetBlendMode(false);
	m_Camera = Ref<FreeFlyCamera>::Create();

	m_Environment = Environment::Create("assets/textures/PBR_Scene_Apartment.hdr");

	//UI::ShouldDisplayDebugInformation(true);

	CreatePBRObjects();

	m_Skybox = Skybox::Create();
	m_Skybox->SetTexture(m_Environment->GetSkyboxTexture());

	GetWindow().Maximize();
	HL_TRACE("PBR Demo Initialized");
}

void PBRSceneDemo::OnUpdate(Timestep timestep)
{
	m_Pistol->Update(timestep);
	m_Cowboy->Update(timestep);
	m_DancingStormTrooper->Update(timestep);

	m_Camera->Update();

	Renderer::ClearScreenBuffers();
	Renderer::ClearScreenColor(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));

	m_Skybox->Render(*m_Camera);
	CoreRenderer::BeginScene(*m_Camera, m_Light);

	static float y_rot = 0.0f;
	y_rot += 0.04f * timestep;

	CoreRenderer::DrawCube(Transform::FromPosition(m_Light.Position));
	CoreRenderer::DrawMesh(m_PBR_Sphere, Transform::FromPosition({ 6, -2, 6 }).Scale(3.0f).Rotate(y_rot, { 0, 1, 0 }));
	CoreRenderer::DrawMesh(m_PBR_Gun, Transform::FromPosition({ 26, -6, 26 }).Scale(0.24f).Rotate(y_rot, { 0, 1, 0 }));
	CoreRenderer::DrawMesh(m_DancingStormTrooper, Transform::FromPosition({ 26, -10, 30 }).Scale(2.0f));

	for (uint64 i = 0; i < m_TestSpheres.size(); i++)
		CoreRenderer::DrawMesh(m_TestSpheres[i], Transform::FromPosition({ 6, -2, 14 + (float) i * 7 }).Scale(3.0f));

	CoreRenderer::DrawMesh(m_Pistol, Transform::FromPosition({ 30, -3, 0 }).Scale(0.5f));
	CoreRenderer::DrawMesh(m_Cowboy, Transform::FromPosition({ 26, -5, -12 }).Scale(0.8f).Rotate(-45.0f, { 0, 1, 0 }));
}

void PBRSceneDemo::OnShutdown()
{
	HL_TRACE("PBR Demo Shutdown");
}

void PBRSceneDemo::OnEvent(Event &e)
{

}

void PBRSceneDemo::OnResize(uint32 width, uint32 height)
{
	m_Camera->OnWindowResize(width, height);
}

void PBRSceneDemo::OnUIRender(Timestep timestep)
{
	HLString deviceText = "Device: ";
	deviceText += Renderer::GetCapabilities().Device;

	HLString rendererText = "Renderer: ";
	rendererText += Renderer::GetCurrentRenderingAPI();

	HLString vendorText = "Vendor: ";
	vendorText += Renderer::GetCapabilities().Vendor;

	HLString versionText = "Version: ";
	versionText += Renderer::GetCapabilities().Version;

	HLString maxTextureUnitsText = "MaxTextureUnits: ";
	maxTextureUnitsText += HLString::ToString(Renderer::GetCapabilities().MaxTextureUnits);

	HLString maxSamplesText = "MaxSamples: ";
	maxSamplesText += HLString::ToString(Renderer::GetCapabilities().MaxSamples);

	HLString maxAnisotropyText = "MaxAnisotropy: ";
	maxAnisotropyText += HLString::ToString(Renderer::GetCapabilities().MaxAnisotropy);

	HLString verticesText = "Vertices: ";
	verticesText += "";	verticesText += HLString::ToString(m_VertexCount);

	HLString indicesText = "Indices: ";
	indicesText += HLString::ToString(m_IndexCount);

	UI::BeginViewport("RendererInfo");
	UI::DrawText(deviceText);
	UI::DrawText(rendererText);
	UI::DrawText(vendorText);
	UI::DrawText(versionText);
	UI::DrawText(maxTextureUnitsText);
	UI::DrawText(maxSamplesText);
	UI::DrawText(maxAnisotropyText);
	UI::DrawText(verticesText);
	UI::DrawText(indicesText);
	UI::EndViewport();
}

void PBRSceneDemo::CreatePBRObjects()
{
	m_PBR_Sphere = AssetLoader::LoadStaticModel("assets/models/PBR_Sphere.obj")->GetMesh(0);
	m_PBR_Sphere->SetTexture(HL_MATERIAL_TEXTURE_ALBEDO, Texture2D::LoadFromFile("assets/textures/PBR_Sphere_Albedo.jpg"));
	m_PBR_Sphere->SetTexture(HL_MATERIAL_TEXTURE_NORMAL, Texture2D::LoadFromFile("assets/textures/PBR_Sphere_Normal.jpg"));
	m_PBR_Sphere->SetTexture(HL_MATERIAL_TEXTURE_METALLIC, Texture2D::LoadFromFile("assets/textures/PBR_Sphere_Metallic.jpg"));
	m_PBR_Sphere->SetTexture(HL_MATERIAL_TEXTURE_ROUGHNESS, Texture2D::LoadFromFile("assets/textures/PBR_Sphere_Roughness.jpg"));
	m_PBR_Sphere->SetTexture(HL_MATERIAL_TEXTURE_IRRADIANCE_MAP, m_Environment->GetIrradianceMap());
	m_PBR_Sphere->SetTexture(HL_MATERIAL_TEXTURE_PREFILTER_MAP, m_Environment->GetRadianceMap());
	m_PBR_Sphere->SetTexture(HL_MATERIAL_TEXTURE_BRDF_MAP, Renderer::GetBRDFLutTexture());
	m_PBR_Sphere->GetMaterialRenderProperties().Metallic = 1;
	m_PBR_Sphere->GetMaterialRenderProperties().Roughness = 1;
	m_VertexCount += GetVertexCountFromMesh(m_PBR_Sphere);
	m_IndexCount += m_PBR_Sphere->GetVertexArray()->GetIndexBuffer()->GetCount();

	m_DancingStormTrooper = AssetLoader::LoadAnimatedModel("assets/models/Stormtrooper.fbx");
	m_DancingStormTrooper->SetTexture(HL_MATERIAL_TEXTURE_ALBEDO, Texture2D::LoadFromFile("assets/textures/Stormtrooper.png"));

	m_DancingStormTrooper->SetTexture(HL_MATERIAL_TEXTURE_IRRADIANCE_MAP, m_Environment->GetIrradianceMap());
	m_DancingStormTrooper->SetTexture(HL_MATERIAL_TEXTURE_PREFILTER_MAP, m_Environment->GetRadianceMap());
	m_DancingStormTrooper->SetTexture(HL_MATERIAL_TEXTURE_BRDF_MAP, Renderer::GetBRDFLutTexture());
	m_DancingStormTrooper->GetMaterialRenderProperties().Metallic = 1.0f;
	m_DancingStormTrooper->GetMaterialRenderProperties().Roughness = 0.0f;
	m_VertexCount += GetVertexCountFromMesh(m_DancingStormTrooper);
	m_IndexCount += m_DancingStormTrooper->GetVertexArray()->GetIndexBuffer()->GetCount();

	auto m_PBR_Gun_Model = AssetLoader::LoadStaticModel("assets/models/PBR_AK47.obj");
	m_PBR_Gun = m_PBR_Gun_Model->GetMesh(0);

	m_PBR_Gun->SetTexture(HL_MATERIAL_TEXTURE_ALBEDO, Texture2D::LoadFromFile("assets/textures/AK47/AK47_albedo.tga"));
	m_PBR_Gun->SetTexture(HL_MATERIAL_TEXTURE_NORMAL, Texture2D::LoadFromFile("assets/textures/AK47/AK47_normal.tga"));
	m_PBR_Gun->SetTexture(HL_MATERIAL_TEXTURE_METALLIC, Texture2D::LoadFromFile("assets/textures/AK47/AK47_metalness.tga"));
	m_PBR_Gun->SetTexture(HL_MATERIAL_TEXTURE_ROUGHNESS, Texture2D::LoadFromFile("assets/textures/AK47/AK47_roughness.tga"));
	m_PBR_Gun->SetTexture(HL_MATERIAL_TEXTURE_AMBIENT_OCCLUSION, Texture2D::LoadFromFile("assets/textures/AK47/AK47_ao.tga"));
	m_PBR_Gun->SetTexture(HL_MATERIAL_TEXTURE_IRRADIANCE_MAP, m_Environment->GetIrradianceMap());
	m_PBR_Gun->SetTexture(HL_MATERIAL_TEXTURE_PREFILTER_MAP, m_Environment->GetRadianceMap());
	m_PBR_Gun->SetTexture(HL_MATERIAL_TEXTURE_BRDF_MAP, Renderer::GetBRDFLutTexture());
	m_PBR_Gun->GetMaterialRenderProperties().Metallic = 1;
	m_PBR_Gun->GetMaterialRenderProperties().Roughness = 1;
	m_VertexCount += GetVertexCountFromMesh(m_PBR_Gun);
	m_IndexCount += m_PBR_Gun->GetVertexArray()->GetIndexBuffer()->GetCount();

	auto SphereBlueprint = MeshFactory::CreateSphere(1.0f);
	SphereBlueprint->SetTexture(HL_MATERIAL_TEXTURE_IRRADIANCE_MAP, m_Environment->GetIrradianceMap());
	SphereBlueprint->SetTexture(HL_MATERIAL_TEXTURE_PREFILTER_MAP, m_Environment->GetRadianceMap());
	SphereBlueprint->SetTexture(HL_MATERIAL_TEXTURE_BRDF_MAP, Renderer::GetBRDFLutTexture());

	for (int32 i = 0; i < 10; i++)
	{
		auto newSphere = SphereBlueprint->Copy();
		newSphere->GetMaterialRenderProperties().Color = { (float) i * 0.02f, (float) i * 0.08f, (float) i * 0.05f, 1.0f };
		newSphere->GetMaterialRenderProperties().Roughness = 0.1f * i;
		m_VertexCount += GetVertexCountFromMesh(newSphere);
		m_IndexCount += newSphere->GetVertexArray()->GetIndexBuffer()->GetCount();

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

	for (int32 i = 0; i < 10; i++)
	{
		auto newSphere = SphereBlueprint->Copy();
		newSphere->GetMaterialRenderProperties().Color = { (float) i * 0.02f, (float) i * 0.08f, (float) i * 0.05f, 1.0f };
		newSphere->GetMaterialRenderProperties().Metallic = 0.1f * i;
		m_VertexCount += GetVertexCountFromMesh(newSphere);
		m_IndexCount += newSphere->GetVertexArray()->GetIndexBuffer()->GetCount();

		m_TestSpheres.push_back(newSphere);
	}

	m_Pistol = AssetLoader::LoadAnimatedModel("assets/models/Pistol.fbx");
	m_Pistol->SetTexture(HL_MATERIAL_TEXTURE_ALBEDO, Texture2D::LoadFromFile("assets/textures/Pistol.png"));
	m_Pistol->SetTexture(HL_MATERIAL_TEXTURE_IRRADIANCE_MAP, m_Environment->GetIrradianceMap());
	m_Pistol->SetTexture(HL_MATERIAL_TEXTURE_PREFILTER_MAP, m_Environment->GetRadianceMap());
	m_Pistol->SetTexture(HL_MATERIAL_TEXTURE_BRDF_MAP, Renderer::GetBRDFLutTexture());
	m_Pistol->GetMaterialRenderProperties().Roughness = 0.4f;
	m_Pistol->GetMaterialRenderProperties().Metallic = 0.68f;
	m_VertexCount += GetVertexCountFromMesh(m_Pistol);
	m_IndexCount += m_Pistol->GetVertexArray()->GetIndexBuffer()->GetCount();

	m_Cowboy = AssetLoader::LoadAnimatedModel("assets/models/Cowboy.fbx", true);
	m_Cowboy->SetTexture(HL_MATERIAL_TEXTURE_ALBEDO, Texture2D::LoadFromFile("assets/textures/Cowboy.png"));
	m_Cowboy->GetMaterial()->IsUsingPBR = false;
	m_VertexCount += GetVertexCountFromMesh(m_Cowboy);
	m_IndexCount += m_Cowboy->GetVertexArray()->GetIndexBuffer()->GetCount();

	m_Pistol->GetAnimation()->Play();
	m_Cowboy->GetAnimation()->Play();
	m_DancingStormTrooper->GetAnimation()->Play();
}

uint32 PBRSceneDemo::GetVertexCountFromModel(const Ref<Model> &model)
{
	uint32 count = 0;
	for (uint32 i = 0; i < model->GetMeshCount(); ++i)
	{
		Ref<Mesh> mesh = model->GetMesh(i);
		std::vector<Vertex> vertices = mesh->GetMeshData().Vertices;
		for (int32 j = 0; j < vertices.size(); ++j)
		{
			count += 3; // 3 vertex positions per vertex
		}
	}

	return count;
}

uint32 PBRSceneDemo::GetVertexCountFromMesh(Ref<Mesh> &mesh)
{
	uint32 count = 0;

	std::vector<Vertex> vertices = mesh->GetMeshData().Vertices;
	for (uint32 i = 0; i < vertices.size(); ++i)
	{
		count += 3; // 3 vertex positions per vertex
	}

	return count;
}

