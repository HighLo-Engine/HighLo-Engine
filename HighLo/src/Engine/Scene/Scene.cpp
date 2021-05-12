#include "HighLoPch.h"
#include "Scene.h"
#include "Engine/Renderer/CoreRenderer.h"

namespace highlo
{
	Scene::Scene(const HLString& name)
		: Name(name)
	{
	}
	
	void Scene::Update(Timestep ts)
	{
		if (!Camera)
		{
			HL_CORE_WARN("Scene>  [-] Failed to Render | Missing Camera [-]");
			return;
		}

		Camera->Update();

		Camera->GetViewProjectionMatrix(m_FrustumInitMatrix);
		m_CameraFrustum.Init(m_FrustumInitMatrix);

		CoreRenderer::BeginScene(*Camera, SceneLight);

		for (auto& entity : m_Entities)
		{
			RenderComponent* rc = m_RenderComponentMap[entity.ID];

			auto WorldAABB = rc->Model.BoundingBox.Transformed(
				entity._TransformComponent->Transform.GetPosition(),
				entity._TransformComponent->Transform.GetScale()
			);

			bool intersecting = m_CameraFrustum.SphereIntersection(entity._TransformComponent->Transform.GetPosition(), WorldAABB.GetRadius());
			//bool intersecting = m_CameraFrustum.AABBIntersection(WorldAABB);

			if (intersecting)
				rc->Enabled = true;
			else
				rc->Enabled = false;
		}
	}

	void Scene::AddEntity(Entity entity)
	{
		m_Entities.push_back(entity);
		m_RenderComponentMap[entity.ID] = entity.GetComponent<RenderComponent>();
	}
}
