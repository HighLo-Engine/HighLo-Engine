#include "HighLoPch.h"
#include "SceneChunk.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/CoreRenderer.h"
#include "Engine/Renderer/MeshFactory.h"
#include "Engine/Window/Window.h"

namespace highlo
{
	SceneChunk::SceneChunk(uint64 layer, uint64 deepestLayer, float size, const glm::vec3& position)
		: m_Layer(layer), m_DeepestLayer(deepestLayer), m_Size(size), m_Position(position)
	{
		Init();
	}

	void SceneChunk::Init()
	{
		m_Mesh = MeshFactory::CreateCube(glm::vec3(m_Size));

		if (m_Layer != m_DeepestLayer)
			GenerateSubChunks();
	}

	void SceneChunk::Update(Timestep ts, Camera& camera, HLFrustum& cameraFrustum, uint64& processedEntityCount)
	{
		if (m_Layer == m_DeepestLayer)
		{
			ProcessEntities(ts, camera, cameraFrustum, processedEntityCount);
			return;
		}

		ProcessSubChunks(ts, camera, cameraFrustum, processedEntityCount);
	}

	void SceneChunk::AddEntity(Entity entity)
	{
		m_Entities.push_back(entity);

		if (m_Layer == m_DeepestLayer)
		{
			RenderComponent* rc = entity.GetComponent<RenderComponent>();
			if (!rc)
				return;

			rc->ResetForNextFrame = true;
			m_RenderComponentMap[entity.ID] = rc;
			return;
		}

		for (auto& chunk : m_SubChunks)
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

	void SceneChunk::SetRenderDistance(float distance)
	{
		m_RenderDistanceSquared = distance * distance;

		for (auto& chunk : m_SubChunks)
			chunk->SetRenderDistance(distance);
	}

	void SceneChunk::SetLOD1Distance(float distance)
	{
		m_LOD1DistanceSquared = distance * distance;

		for (auto& chunk : m_SubChunks)
			chunk->SetLOD1Distance(distance);
	}

	void SceneChunk::SetLOD2Distance(float distance)
	{
		m_LOD2DistanceSquared = distance * distance;

		for (auto& chunk : m_SubChunks)
			chunk->SetLOD2Distance(distance);
	}

	void SceneChunk::RenderDebugMesh()
	{
		Renderer::SetWireframe(true);
		CoreRenderer::DrawMesh(m_Mesh, Transform::FromPosition(glm::vec3(m_Position.x + m_Size / 2.0f, m_Position.y, m_Position.z + m_Size / 2.0f)));
		Renderer::SetWireframe(false);
	}

	void SceneChunk::ProcessEntities(Timestep ts, Camera& camera, HLFrustum& cameraFrustum, uint64& processedEntityCount)
	{
		for (auto& entity : m_Entities)
		{
			processedEntityCount++;

			float distanceToEntity = DistanceSquaredSSE(entity._TransformComponent->Transform.GetPosition(), camera.GetPosition());
			if (distanceToEntity > m_RenderDistanceSquared)
				continue;

			RenderComponent* rc = m_RenderComponentMap[entity.ID];
			if (!rc)
				continue;

			if (distanceToEntity > m_LOD2DistanceSquared)
				rc->LodLevel = 2;
			else if (distanceToEntity > m_LOD1DistanceSquared)
				rc->LodLevel = 1;
			else
				rc->LodLevel = 0;

			auto WorldAABB = rc->MainModel->BoundingBox.Transformed(
				entity._TransformComponent->Transform.GetPosition(),
				entity._TransformComponent->Transform.GetScale()
			);

			bool intersecting = cameraFrustum.SphereIntersection(entity._TransformComponent->Transform.GetPosition(), WorldAABB.GetRadius());

			if (intersecting)
				rc->Enabled = true;
			else
				rc->Enabled = false;
		}
	}

	void SceneChunk::ProcessSubChunks(Timestep ts, Camera& camera, HLFrustum& cameraFrustum, uint64& processedEntityCount)
	{
		auto& CameraPosition = camera.GetPosition();

		auto& window_size = glm::vec2((float)Window::Get().GetWidth(), (float)Window::Get().GetHeight());

		for (auto& chunk : m_SubChunks)
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

			if (!(d1 <= m_RenderDistanceSquared ||
				d2 <= m_RenderDistanceSquared ||
				d3 <= m_RenderDistanceSquared ||
				d4 <= m_RenderDistanceSquared))
			{
				continue;
			}

			auto wts1 = WorldToScreen(p1, camera.GetViewMatrix(), camera.GetProjection(), window_size);
			auto wts2 = WorldToScreen(p2, camera.GetViewMatrix(), camera.GetProjection(), window_size);
			auto wts3 = WorldToScreen(p3, camera.GetViewMatrix(), camera.GetProjection(), window_size);
			auto wts4 = WorldToScreen(p4, camera.GetViewMatrix(), camera.GetProjection(), window_size);

			if (wts1.z < 0 && wts2.z < 0 && wts3.z < 0 && wts4.z < 0)
				continue;

			chunk->Update(ts, camera, cameraFrustum, processedEntityCount);
			//chunk->RenderDebugMesh();
		}
	}

	void SceneChunk::GenerateSubChunks()
	{
		float subchunk_size = m_Size / (float)m_ChunkRowCount;

		for (int i = 0; i < m_ChunkRowCount; i++)
		{
			for (int j = 0; j < m_ChunkRowCount; j++)
			{
				glm::vec3 chunk_pos = {
					m_Position.x + (float)i * subchunk_size,
					0,
					m_Position.z + (float)j * subchunk_size
				};

				auto chunk = Ref<SceneChunk>::Create(m_Layer + 1, m_DeepestLayer, subchunk_size, chunk_pos);
				chunk->SetDeepestLayer(m_DeepestLayer);
				m_SubChunks.push_back(chunk);
			}
		}
	}
}
