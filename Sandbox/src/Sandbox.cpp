#include "Sandbox.h"

void Sandbox::OnInitialize()
{
	// TO-DO: Fix for GLFW configuration.
	HLApplication::Get().GetWindow().SetWindowIcon("assets/textures/HighLoEngine.png");

	m_Camera = Ref<FPSCamera>::Create();
	m_Camera->SetYaw(90);
	
	ImGuiRenderer::ShouldDisplayDebugInformation(true);

	m_CabinTexture = Texture2D::LoadFromFile("assets/textures/WoodenCabin.jpg");
	m_CabinModel = AssetLoader::LoadStaticModel("assets/models/WoodenCabin.obj", false);
	m_CabinModel.AddTexture(m_CabinTexture);

	m_CabinTransform.Rotate(180.0f, { 0, 1, 0 });
	m_CabinTransform.Scale(0.3f);
	m_CabinTransform.Translate({ 20, -5, 75 });

	HL_TRACE("Sandbox Initialized");
}

void Sandbox::OnUpdate(Timestep timestep)
{
	m_Camera->Update();

	Renderer::ClearScreenBuffers();
	Renderer::ClearScreenColor(glm::vec4(0.2f, 0.06f, 0.06f, 1.0f));

	CoreRenderer::BeginScene(*m_Camera);
	Renderer::SetWireframe(true);
	CoreRenderer::DrawCube(Transform::FromPosition({ -1, 0, 5 }).Scale(2.0f), { 0.2f, 1.0f, 0.6f });
	CoreRenderer::DrawSphere(Transform::FromPosition({ 6, 0, 6 }).Scale(2.0f), { 0.2f, 0.5f, 1.0f });
	CoreRenderer::DrawCapsule(Transform::FromPosition({ 2, 0, 8 }).Rotate(45.0f, {0, 0, 1}), { 1.0f, 0.3f, 0.2f });

	Renderer::SetWireframe(false);
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
	ImGuiRenderer::BeginChild("Props", 400, 100);

	static float rotation = 180;
	ImGuiRenderer::SliderFloat("Rotation", rotation, 0, 360);
	m_CabinTransform.SetRotation({ m_CabinTransform.GetRotation().x, rotation, m_CabinTransform.GetRotation().z });

	ImGuiRenderer::EndChild();
}
