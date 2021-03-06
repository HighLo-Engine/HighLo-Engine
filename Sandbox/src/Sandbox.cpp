#include "Sandbox.h"

// This Sandbox application will render nothing,
// because the underlying API is currently under heavy development!

void Sandbox::OnInitialize()
{
	// TODO: Fix for GLFW configuration.
	// HLApplication::Get().GetWindow().SetWindowIcon("assets/Resources/HighLoEngine.ico");

	VirtualFileSystem::Get()->Mount("textures", "assets/textures/");
	VirtualFileSystem::Get()->Mount("models", "assets/models/");

	m_Scene = Ref<Scene>::Create();
	//m_Scene->m_Camera = Ref<FreeFlyCamera>::Create();

	auto house_tex = Texture2D::LoadFromFile("assets/textures/WoodenCabin.jpg");
	/*
	Ref<Model> house = AssetLoader::LoadStaticModel("assets/models/WoodenCabin.obj");
	house->GetMesh(0)->SetTexture(HL_MATERIAL_TEXTURE_ALBEDO, house_tex);
	house->GetMesh(1)->SetTexture(HL_MATERIAL_TEXTURE_ALBEDO, house_tex);

	Ref<Model> houseLod1 = AssetLoader::LoadStaticModel("assets/models/WoodenCabinLOD1.obj");
	houseLod1->GetMesh(0)->SetTexture(HL_MATERIAL_TEXTURE_ALBEDO, house_tex);
	houseLod1->GetMesh(1)->SetTexture(HL_MATERIAL_TEXTURE_ALBEDO, house_tex);

	Ref<Model> houseLod2 = AssetLoader::LoadStaticModel("assets/models/WoodenCabinLOD2.obj");
	houseLod2->GetMesh(0)->SetTexture(HL_MATERIAL_TEXTURE_ALBEDO, house_tex);
	houseLod2->GetMesh(1)->SetTexture(HL_MATERIAL_TEXTURE_ALBEDO, house_tex);
	
	for (int32 i = -20; i < 20; i++)
	{
		for (int32 j = -20; j < 20; j++)
		{
			Entity entity = Entity();
			auto rc = entity.AddComponent<RenderComponent>();
			rc->MainModel = house;
			rc->ModelLOD1 = houseLod1;
			rc->ModelLOD2 = houseLod2;
			rc->UseLOD = true;

			entity.SetTransform(Transform::FromPosition({ i * 5, -2, j * 5 }).Scale(0.03f));

		//	m_Scene->AddEntity(entity);
		}
	}
	*/

	m_RenderSystemRef = GetECSSystemManager().GetSystem("RenderSystem").As<RenderSystem>();
	//UI::ShouldDisplayDebugInformation(true);
	HL_TRACE("Sandbox Initialized");
}

void Sandbox::OnUpdate(Timestep timestep)
{
	//m_Scene->Update(timestep);
}

void Sandbox::OnShutdown()
{
	HL_TRACE("Sandbox Shutdown");
}

void Sandbox::OnEvent(Event &e)
{
	if (e.IsInCategory(EventCategory::EventCategoryApplication))
		HL_TRACE(e.ToString());
}

void Sandbox::OnUIRender(Timestep timestep)
{
	// UI::BeginWindow("Scene Debug Information");
	// 
	// HLString entities_total = "         Total Entities   : " + HLString::ToString(m_Scene->GetEntityCount());
	// HLString entities_processed = "Processed Entities  : " + HLString::ToString(m_Scene->GetFrameEntitiesProcessed());
	// HLString entities_rendered = "Rendered Entities    : " + HLString::ToString(m_RenderSystemRef->FrameEntitiesRendered);
	// HLString meshes_rendered = "Rendered Meshes    : " + HLString::ToString(m_RenderSystemRef->FrameMeshesRendered);
	// HLString triangles_rendered = "Rendered Triangles : " + HLString::ToString(m_RenderSystemRef->FrameTrianglesRendered);
	// 
	// UI::DrawText(entities_total);
	// UI::DrawText(entities_processed);
	// UI::DrawText(entities_rendered);
	// UI::DrawText(meshes_rendered);
	// UI::DrawText(triangles_rendered);
	// UI::Separator();
	// 
	// UI::EndWindow();
}

void Sandbox::OnResize(uint32 width, uint32 height)
{
	//m_Scene->m_Camera->OnWindowResize(width, height);
}

