#include "HighLoPch.h"
#include "RenderSystem.h"
#include "Engine/Renderer/CoreRenderer.h"
#include "Engine/Renderer/Renderer.h"

namespace highlo
{
	void RenderSystem::OnUpdate(Timestep ts, ECS_Registry& registry)
	{
		FrameEntitiesRendered = 0;
		FrameMeshesRendered = 0;
		FrameTrianglesRendered = 0;

		registry.ForEach<RenderComponent>([&, ts](EntityID entityID, RenderComponent& component, TransformComponent& transform) {
			if (!component.Enabled)
				return;

			for (uint64 i = 0; i < component.Model.GetMeshCount(); i++)
			{
				if (component.Wireframe)
					Renderer::SetWireframe(true);

				CoreRenderer::DrawMesh(component.Model.GetMesh(i), transform.Transform);

				if (component.Wireframe)
					Renderer::SetWireframe(false);
				
				FrameMeshesRendered++;
				FrameTrianglesRendered += component.Model.GetMesh(i)->GetMeshData().m_Indices.size() / 3;
			}

			Renderer::SetWireframe(true);
			//CoreRenderer::DrawMesh(transform.Transform.GetOBB(), transform.Transform);
			Renderer::SetWireframe(false);

			FrameEntitiesRendered++;
		});
	}
}
