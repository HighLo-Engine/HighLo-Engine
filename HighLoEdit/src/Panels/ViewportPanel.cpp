#include "ViewportPanel.h"

void ViewportPanel::Initialize(uint32 width, uint32 height)
{
	m_ViewportWidth = width;
	m_ViewportHeight = height;
	m_Camera = Ref<EditorCamera>::Create(glm::perspectiveFov(glm::radians(90.0f), (float)width, (float)height, 0.1f, 1000.0f));

	FramebufferSpecification spec;
	spec.Width = 1280;
	spec.Height = 720;
	spec.Attachments = { TextureFormat::RGBA };
	spec.ClearColor = { 0.5f, 0.1f, 0.1f, 1.0f };
	m_ViewportContent = Framebuffer::Create(spec);

	// TEMP
	m_Environment = Environment::Create("assets/textures/PBR_Scene_Apartment.hdr");
	CreatePBRObjects();
	m_Skybox = Skybox::Create();
	m_Skybox->SetTexture(m_Environment->GetSkyboxTexture());
	m_Light.Color = { 1.0f, 0.2f, 0.3f };

	Renderer::SetBlendMode(false);
}

void ViewportPanel::Destroy()
{
}

void ViewportPanel::Render(Timestep ts)
{
	// Render Framebuffer
	m_ViewportContent->Bind();

	Renderer::ClearScreenBuffers();
	Renderer::ClearScreenColor(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));

	m_Camera->Update();
	m_Pistol->Update(ts);
	m_Cowboy->Update(ts);
	m_Skybox->Render(*m_Camera);
	CoreRenderer::BeginScene(*m_Camera, m_Light);

	static float y_rot = 0.0f;
	y_rot += 0.04f * ts;

	CoreRenderer::DrawCube(Transform::FromPosition(m_Light.Position));
	CoreRenderer::DrawMesh(m_PBR_Sphere, Transform::FromPosition({ 6, -2, 6 }).Scale(3.0f).Rotate(y_rot, { 0, 1, 0 }));
	CoreRenderer::DrawMesh(m_PBR_Gun, Transform::FromPosition({ 26, -6, 26 }).Scale(0.24f).Rotate(y_rot, { 0, 1, 0 }));

	for (uint64 i = 0; i < m_TestSpheres.size(); i++)
		CoreRenderer::DrawMesh(m_TestSpheres[i], Transform::FromPosition({ 6, -2, 14 + (float) i * 7 }).Scale(3.0f));

	CoreRenderer::DrawMesh(m_Pistol, Transform::FromPosition({ 30, -3, 0 }).Scale(0.5f));
	CoreRenderer::DrawMesh(m_Cowboy, Transform::FromPosition({ 26, -5, -12 }).Scale(0.8f).Rotate(-45.0f, { 0, 1, 0 }));

	m_ViewportContent->Unbind();
}

void ViewportPanel::RenderUI(Timestep ts)
{
	ImGuiRenderer::StartViewport("Viewport");

	m_ViewportPanelMouseOver = ImGuiRenderer::IsWindowHovered();
	m_ViewportPanelFocused = ImGuiRenderer::IsWindowFocused();

	auto viewportOffset = ImGuiRenderer::GetCursorPosition(); // Includes tab bar
	auto viewportSize = ImGuiRenderer::GetContentRegion();
	if (viewportSize.x > 0.0f && viewportSize.y > 0.0f)
	{
		m_Camera->SetProjection(glm::perspectiveFov(glm::radians(90.0f), viewportSize.x, viewportSize.y, 0.1f, 1000.0f));
		m_Camera->SetViewportSize((uint32)viewportSize.x, (uint32)viewportSize.y);
	}

	// Render viewport image
	ImGuiRenderer::DrawFramebuffer(m_ViewportContent, viewportSize, { 0, 1 }, { 1, 0 });

	auto windowSize = ImGuiRenderer::GetWindowSize();
	glm::vec2 minBound = ImGuiRenderer::GetWindowPos();
	minBound.x += viewportOffset.x;
	minBound.y += viewportOffset.y;

	glm::vec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
	m_ViewportBounds[0] = { minBound.x, minBound.y };
	m_ViewportBounds[1] = { maxBound.x, maxBound.y };
	m_AllowViewportCameraEvents = ImGuiRenderer::IsMouseHoveringRect(minBound, maxBound);

	ImGuiRenderer::EndViewport();
}

void ViewportPanel::OnEvent(Event &e)
{
	m_Camera->OnEvent(e);
}

void ViewportPanel::OnResize(uint32 width, uint32 height)
{
	m_ViewportWidth = width;
	m_ViewportHeight = height;

	if (m_ViewportWidth > 0 && m_ViewportHeight > 0)
		m_Camera->SetProjection(glm::perspectiveFov(glm::radians(90.0f), (float)m_ViewportWidth, (float)m_ViewportHeight, 0.1f, 1000.0f));
}

std::pair<float, float> ViewportPanel::GetMouseViewportSpace()
{
	glm::vec2 mousePos = ImGuiRenderer::GetMousePosition();
	float mx = mousePos.x;
	float my = mousePos.y;
	
	mx -= m_ViewportBounds[0].x;
	my -= m_ViewportBounds[0].y;
	auto viewportWidth = m_ViewportBounds[1].x - m_ViewportBounds[0].x;
	auto viewportHeight = m_ViewportBounds[1].y - m_ViewportBounds[0].y;
	return { (mx / viewportWidth) * 2.0f - 1.0f, ((my / viewportHeight) * 2.0f - 1.0f) * -1.0f };
}

std::pair<glm::vec3, glm::vec3> ViewportPanel::CastRay(float mx, float my)
{
	glm::vec4 mouseClipPos = { mx, my, -1.0f, 1.0f };
	auto inverseProj = glm::inverse(m_Camera->GetProjection());
	auto inverseView = glm::inverse(glm::mat3(m_Camera->GetViewMatrix()));

	glm::vec3 rayPos = m_Camera->GetPosition();

	glm::vec4 ray = inverseProj * mouseClipPos;
	glm::vec3 rayDir = inverseView * glm::vec3(ray);

	return { rayPos, rayDir };
}

Ray ViewportPanel::CastMouseRay()
{
	auto [mx, my] = GetMouseViewportSpace();
	if (mx > -1.0f && mx < 1.0f && my > -1.0f && my < 1.0f)
	{
		auto [origin, direction] = CastRay(mx, my);
		return Ray(origin, direction);
	}

	return Ray::Zero();
}

void ViewportPanel::CreatePBRObjects()
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

	auto SphereBlueprint = MeshFactory::CreateSphere(1.0f);
	SphereBlueprint->SetTexture(HL_MATERIAL_TEXTURE_IRRADIANCE_MAP, m_Environment->GetIrradianceMap());
	SphereBlueprint->SetTexture(HL_MATERIAL_TEXTURE_PREFILTER_MAP, m_Environment->GetRadianceMap());
	SphereBlueprint->SetTexture(HL_MATERIAL_TEXTURE_BRDF_MAP, Renderer::GetBRDFLutTexture());

	for (int32 i = 0; i < 10; i++)
	{
		auto newSphere = SphereBlueprint->Copy();
		newSphere->GetMaterialRenderProperties().Color = { (float) i * 0.02f, (float) i * 0.08f, (float) i * 0.05f, 1.0f };
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

	for (int32 i = 0; i < 10; i++)
	{
		auto newSphere = SphereBlueprint->Copy();
		newSphere->GetMaterialRenderProperties().Color = { (float) i * 0.02f, (float) i * 0.08f, (float) i * 0.05f, 1.0f };
		newSphere->GetMaterialRenderProperties().Metallic = 0.1f * i;

		m_TestSpheres.push_back(newSphere);
	}

	m_Pistol = AssetLoader::LoadAnimatedModel("assets/models/Pistol.fbx");
	m_Pistol->SetTexture(HL_MATERIAL_TEXTURE_ALBEDO, Texture2D::LoadFromFile("assets/textures/Pistol.png"));
	m_Pistol->SetTexture(HL_MATERIAL_TEXTURE_IRRADIANCE_MAP, m_Environment->GetIrradianceMap());
	m_Pistol->SetTexture(HL_MATERIAL_TEXTURE_PREFILTER_MAP, m_Environment->GetRadianceMap());
	m_Pistol->SetTexture(HL_MATERIAL_TEXTURE_BRDF_MAP, Renderer::GetBRDFLutTexture());
	m_Pistol->GetMaterialRenderProperties().Roughness = 0.4f;
	m_Pistol->GetMaterialRenderProperties().Metallic = 0.68f;

	m_Cowboy = AssetLoader::LoadAnimatedModel("assets/models/Cowboy.fbx", true);
	m_Cowboy->SetTexture(HL_MATERIAL_TEXTURE_ALBEDO, Texture2D::LoadFromFile("assets/textures/Cowboy.png"));
	m_Cowboy->m_Material->IsUsingPBR = false;

	m_Pistol->m_Animation->Play();
	m_Cowboy->m_Animation->Play();
}