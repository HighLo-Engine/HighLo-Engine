#include "HighLoPch.h"
#include "Scene.h"
#include "Engine/Renderer/CoreRenderer.h"
#include "Engine/Window/Window.h"

namespace highlo
{
	Scene::Scene(const HLString& name, float worldSize)
		: Name(name), m_CameraFrustum(HLFrustum{}), m_WorldSize(worldSize)
	{
		m_FirstChunkSize = m_WorldSize / m_ChunkRowCount;

		GenerateFirstLayerChunks();
		
		SetRenderDistance(100.0f);
		SetLOD1Distance(50.0f);
		SetLOD2Distance(80.0f);
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

		auto& CameraPosition = Camera->GetPosition();
		auto& window_size = glm::vec2((float)Window::Get().GetWidth(), (float)Window::Get().GetHeight());

		m_FrameEntitiesProcessed = 0;

		for (auto& chunk : m_Chunks)
		{
			if (!chunk->GetEntityCount())
				continue;

			auto& chunk_pos = chunk->GetPosition();
			auto chunk_size = chunk->GetSize();

			const auto& p1 = glm::vec3(chunk_pos.x, CameraPosition.y, chunk_pos.z);
			const auto& p2 = glm::vec3(chunk_pos.x + chunk_size, CameraPosition.y, chunk_pos.z);
			const auto& p3 = glm::vec3(chunk_pos.x, CameraPosition.y, chunk_pos.z + chunk_size);
			const auto& p4 = glm::vec3(chunk_pos.x + chunk_size, CameraPosition.y, chunk_pos.z + chunk_size);

			float d1 = DistanceSquaredSSE(CameraPosition, p1);
			float d2 = DistanceSquaredSSE(CameraPosition, p2);
			float d3 = DistanceSquaredSSE(CameraPosition, p3);
			float d4 = DistanceSquaredSSE(CameraPosition, p4);

			if (!(	d1 <= m_RenderDistanceSquared ||
					d2 <= m_RenderDistanceSquared ||
					d3 <= m_RenderDistanceSquared ||
					d4 <= m_RenderDistanceSquared ))
			{
				continue;
			}

			auto wts1 = WorldToScreen(p1, Camera->GetViewMatrix(), Camera->GetProjection(), window_size);
			auto wts2 = WorldToScreen(p2, Camera->GetViewMatrix(), Camera->GetProjection(), window_size);
			auto wts3 = WorldToScreen(p3, Camera->GetViewMatrix(), Camera->GetProjection(), window_size);
			auto wts4 = WorldToScreen(p4, Camera->GetViewMatrix(), Camera->GetProjection(), window_size);

			if (wts1.z < 0 && wts2.z < 0 && wts3.z < 0 && wts4.z < 0)
				continue;

			chunk->Update(ts, *Camera, m_CameraFrustum, m_FrameEntitiesProcessed);
			//chunk->RenderDebugMesh();
		}
	}

	void Scene::AddEntity(Entity entity)
	{
		m_Entities.push_back(entity);

		for (auto& chunk : m_Chunks)
		{
			const auto is_entity_in_chunk = [&](const glm::vec3& entity_pos, SceneChunk& chunk) -> bool {
				auto& chunk_pos = chunk.GetPosition();
				auto chunk_size = chunk.GetSize();

				if (entity_pos.x >= chunk_pos.x && entity_pos.x <= chunk_pos.x + chunk_size &&
					entity_pos.z >= chunk_pos.z && entity_pos.z <= chunk_pos.z + chunk_size)
					return true;

				return false;
			};

			if (is_entity_in_chunk(entity._TransformComponent->Transform.GetPosition(), *chunk))
				chunk->AddEntity(entity);
		}
	}
	
	void Scene::SetRenderDistance(float distance)
	{
		m_RenderDistanceSquared = distance * distance;

		for (auto& chunk : m_Chunks)
			chunk->SetRenderDistance(distance);
	}
	
	void Scene::SetLOD1Distance(float distance)
	{
		m_LOD1DistanceSquared = distance * distance;

		for (auto& chunk : m_Chunks)
			chunk->SetLOD1Distance(distance);
	}
	
	void Scene::SetLOD2Distance(float distance)
	{
		m_LOD2DistanceSquared = distance * distance;

		for (auto& chunk : m_Chunks)
			chunk->SetLOD2Distance(distance);
	}

	void Scene::GenerateFirstLayerChunks()
	{
		float start_pos = -m_WorldSize / 2.0f;

		for (int i = 0; i < m_ChunkRowCount; i++)
		{
			for (int j = 0; j < m_ChunkRowCount; j++)
			{
				glm::vec3 chunk_pos = {
					start_pos + (float)i * m_FirstChunkSize,
					0,
					start_pos + (float)j * m_FirstChunkSize
				};

				auto chunk = Ref<SceneChunk>::Create(0, m_ChunkLayers - 1, m_FirstChunkSize, chunk_pos);
				m_Chunks.push_back(chunk);
			}
		}
	}
}
