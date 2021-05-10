#include "HighLoPch.h"
#include "RenderSystem.h"
#include "Engine/Renderer/CoreRenderer.h"
#include "Engine/Core/HLTimer.h"

namespace highlo
{
	void RenderSystem::OnUpdate(Timestep ts, ECS_Registry& registry)
	{
		registry.ForEach<RenderComponent>([&, ts](EntityID entityID, RenderComponent& component, TransformComponent& transform) {
			for (uint64 i = 0; i < component.Model.GetMeshCount(); i++)
				CoreRenderer::DrawMesh(component.Model.GetMesh(i), transform.Transform);
		});
	}
}
