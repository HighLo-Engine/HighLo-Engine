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

	auto albedo = Texture2D::LoadFromFile("assets/textures/PBR_Sphere_Albedo.jpg");
	auto normal = Texture2D::LoadFromFile("assets/textures/PBR_Sphere_Normal.jpg");
	auto metallic = Texture2D::LoadFromFile("assets/textures/PBR_Sphere_Metallic.jpg");
	auto roughness = Texture2D::LoadFromFile("assets/textures/PBR_Sphere_Roughness.jpg");
	auto ao = Texture2D::CreateFromColor({ 255, 255, 255 });

	m_PBR_Sphere = AssetLoader::LoadStaticModel("assets/models/PBR_Sphere.dae").GetMesh(0);
	m_PBR_Sphere->m_Material->SetTexture(HL_MATERIAL_TEXTURE_ALBEDO, albedo);
	m_PBR_Sphere->m_Material->SetTexture(HL_MATERIAL_TEXTURE_NORMAL, normal);
	m_PBR_Sphere->m_Material->SetTexture(HL_MATERIAL_TEXTURE_METALLIC, metallic);
	m_PBR_Sphere->m_Material->SetTexture(HL_MATERIAL_TEXTURE_ROUGHNESS, roughness);
	m_PBR_Sphere->m_Material->SetTexture(HL_MATERIAL_TEXTURE_AMBIENT_OCCLUSION, ao);

	m_Skybox = Skybox::Create();
	/*m_Skybox->SetTexture(Texture3D::LoadFromFiles(
		{
			("assets/textures/skyboxes/SB2_right.jpg"),
			("assets/textures/skyboxes/SB2_left.jpg"),
			("assets/textures/skyboxes/SB2_bottom.jpg"),
			("assets/textures/skyboxes/SB2_top.jpg"),
			("assets/textures/skyboxes/SB2_back.jpg"),
			("assets/textures/skyboxes/SB2_front.jpg"),
		}
	));*/
	m_Skybox->SetTexture(m_Environment->GetRadianceMap());
	HL_TRACE("Sandbox Initialized");
}

void Sandbox::OnUpdate(Timestep timestep)
{
	m_Camera->Update();

	Renderer::ClearScreenBuffers();
	Renderer::ClearScreenColor(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));

	CoreRenderer::BeginScene(*m_Camera, m_Light);

	static float y_rot = 0.0f;
	y_rot += 0.0004f * timestep;

	CoreRenderer::DrawCube(Transform::FromPosition(m_Light.Position));
	CoreRenderer::DrawMesh(m_PBR_Sphere, Transform::FromPosition({ 6, -2, 6 }).Scale(3.0f).Rotate(y_rot, { 0, 1, 0 }));
	CoreRenderer::DrawSphere(Transform::FromPosition({ -6, -2, 9 }).Scale(3.0f).Rotate(-y_rot, { 0, 1, 0 }), { 0, 0.7f, 0 });
	m_Skybox->Render(*m_Camera);

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
