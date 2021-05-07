#include "Sandbox.h"

void Sandbox::OnInitialize()
{
	// TO-DO: Fix for GLFW configuration.
	//HLApplication::Get().GetWindow().SetWindowIcon("assets/textures/HighLoEngine.ico");

	VirtualFileSystem::Get()->Mount("textures", "assets/textures/");
	VirtualFileSystem::Get()->Mount("models", "assets/models/");

	m_Camera = FPSCamera::Create();
	m_Camera->SetYaw(90);
	
	ImGuiRenderer::ShouldDisplayDebugInformation(true);

	m_CabinTexture = Texture2D::LoadFromFile("assets/textures/WoodenCabin.jpg");
	m_CabinModel = AssetLoader::LoadStaticModel("assets/models/WoodenCabin.obj", false);
	m_CabinModel.AddTexture(m_CabinTexture);

	m_CabinTransform.Rotate(180.0f, { 0, 1, 0 });
	m_CabinTransform.Scale(0.3f);
	m_CabinTransform.Translate({ 20, -5, 75 });

	m_Sphere = MeshFactory::CreateSphere(1.0f);
	m_Sphere->m_Material->Properties.m_RenderProperties.m_Color = { 1, 0, 0, 1 };
	m_Sphere->m_Material->AddTexture(Texture2D::CreateFromColor({ 255, 255, 255 }));

	HL_TRACE("Sandbox Initialized");
}

void Sandbox::OnUpdate(Timestep timestep)
{
	m_Camera->Update();

	Renderer::ClearScreenBuffers();
	Renderer::ClearScreenColor(glm::vec4(0.02f, 0.02f, 0.02f, 1.0f));

	CoreRenderer::BeginScene(*m_Camera);

	CoreRenderer::DrawMesh(m_Sphere, Transform::FromPosition({ 6, -2, 6 }).Scale(3.0f));
		
	for (uint64 i = 0; i < m_CabinModel.GetMeshCount(); i++)
		CoreRenderer::DrawMesh(m_CabinModel.GetMesh(i), m_CabinTransform);

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
	ImGuiRenderer::StartWindow("Material Editing");
	ImGuiRenderer::BeginChild("Material Properties", 400, 400);

	ImGuiRenderer::Property("Color", m_Sphere->m_Material->Properties.m_RenderProperties.m_Color, PropertyFlag::Color);
	ImGuiRenderer::Property("Roughness", m_Sphere->m_Material->Properties.m_RenderProperties.m_Roughness, 0, 1);
	ImGuiRenderer::Property("Metallic", m_Sphere->m_Material->Properties.m_RenderProperties.m_Metallic, 0, 1);

	ImGuiRenderer::EndChild();
	ImGuiRenderer::EndWindow();
}

void Sandbox::OnResize(uint32 width, uint32 height)
{
	m_Camera->OnWindowResize(width, height);
}
