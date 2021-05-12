#include "Sandbox.h"

struct HealthComponent
{
	int Health = 100;
};

void Sandbox::OnInitialize()
{
	// TODO: Fix for GLFW configuration.
	//HLApplication::Get().GetWindow().SetWindowIcon("assets/textures/HighLoEngine.ico");

	VirtualFileSystem::Get()->Mount("textures", "assets/textures/");
	VirtualFileSystem::Get()->Mount("models", "assets/models/");

	m_Scene = Ref<Scene>::Create();
	m_Scene->Camera = Ref<FreeFlyCamera>::Create();

	Model house = AssetLoader::LoadStaticModel("assets/models/WoodenCabin.obj");
	house.GetMesh(0)->m_Material->SetTexture(HL_MATERIAL_TEXTURE_ALBEDO, Texture2D::LoadFromFile("assets/textures/WoodenCabin.jpg"));
	house.GetMesh(1)->m_Material->SetTexture(HL_MATERIAL_TEXTURE_ALBEDO, Texture2D::LoadFromFile("assets/textures/WoodenCabin.jpg"));

	for (int i = -3; i < 3; i++)
	{
		for (int j = -3; j < 3; j++)
		{
			Entity entity = Entity();
			entity.AddComponent<RenderComponent>()->Model = house;
			entity._TransformComponent->Transform = Transform::FromPosition({ i * 5, -2, j * 5 }).Scale(0.03f);

			m_Scene->AddEntity(entity);
		}
	}

	m_RenderSystemRef = GetECSSystemManager().GetSystem("RenderSystem").As<RenderSystem>();
	ImGuiRenderer::ShouldDisplayDebugInformation(true);
	HL_TRACE("Sandbox Initialized");
}

void Sandbox::OnUpdate(Timestep timestep)
{
	Renderer::ClearScreenBuffers();
	Renderer::ClearScreenColor(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));

	m_Scene->Update(timestep);
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
	ImGuiRenderer::StartWindow("Scene Debug Information", 400, 200);

	HLString entities_rendered	= "Rendered Entities  : " + HLString::ToString(m_RenderSystemRef->FrameEntitiesRendered);
	HLString meshes_rendered	= "Rendered Meshes    : " + HLString::ToString(m_RenderSystemRef->FrameMeshesRendered);
	HLString triangles_rendered	= "Rendered Triangles : " + HLString::ToString(m_RenderSystemRef->FrameTrianglesRendered);

	ImGuiRenderer::Text(entities_rendered);
	ImGuiRenderer::Text(meshes_rendered);
	ImGuiRenderer::Text(triangles_rendered);
	ImGuiRenderer::Separator();

	ImGuiRenderer::EndWindow();
}

void Sandbox::OnResize(uint32 width, uint32 height)
{
	m_Scene->Camera->OnWindowResize(width, height);
}
