// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "RenderSystem.h"

#include "Engine/Renderer/CoreRenderer.h"
#include "Engine/Renderer/Renderer.h"

namespace highlo
{
	void RenderSystem::OnUpdate(Timestep ts, ECS_Registry &registry)
	{
		// TODO: Needs refactor
	#if 0
		FrameEntitiesRendered = 0;
		FrameMeshesRendered = 0;
		FrameTrianglesRendered = 0;

		registry.ForEach<RenderComponent>([&, ts](UUID entityID, TransformComponent &transform, RenderComponent &component) {
			if (!component.Enabled)
				return;

			Ref<Model> &model = component.MainModel;
			if (component.UseLOD)
			{
				switch (component.LodLevel)
				{
					case 1: model = component.ModelLOD1; break;
					case 2: model = component.ModelLOD2; break;
					default: break;
				}
			}

			for (uint64 i = 0; i < model->GetMeshCount(); i++)
			{
				if (component.Wireframe)
					Renderer::SetWireframe(true);

				CoreRenderer::DrawMesh(model->GetMesh(i), transform.Transform);

				if (component.Wireframe)
					Renderer::SetWireframe(false);
				
				FrameMeshesRendered++;
				FrameTrianglesRendered += model->GetMesh(i)->GetMeshData().Indices.size() / 3;
			}

			if (component.ResetForNextFrame)
				component.Enabled = false;

			FrameEntitiesRendered++;
		});
	#endif
	}
}

