#pragma once
#include "SceneChunk.h"
#include "Engine/Renderer/Light.h"

namespace highlo
{
	class Scene : public IsSharedReference
	{
	public:
		HLAPI Scene(const HLString& name = "Scene1", float worldSize = 1000);

		HLString Name;
		Ref<Camera> Camera;
		Light SceneLight;

		HLAPI void Update(Timestep ts);
		HLAPI void AddEntity(Entity entity);

		HLAPI inline const float GetRenderDistanceSquared() const { return m_RenderDistanceSquared; }
		HLAPI inline const float GetRenderDistance() const { return glm::sqrt(m_RenderDistanceSquared); }
		HLAPI void SetRenderDistance(float distance);

		HLAPI inline const float GetLOD1DistanceSquared() const { return m_LOD1DistanceSquared; }
		HLAPI inline const float GetLOD1Distance() const { return glm::sqrt(m_LOD1DistanceSquared); }
		HLAPI void SetLOD1Distance(float distance);

		HLAPI inline const float GetLOD2DistanceSquared() const { return m_LOD2DistanceSquared; }
		HLAPI inline const float GetLOD2Distance() const { return glm::sqrt(m_LOD2DistanceSquared); }
		HLAPI void SetLOD2Distance(float distance);

		HLAPI inline const uint64 GetEntityCount() const { return m_Entities.size(); }

		HLAPI inline const uint64 GetFrameEntitiesProcessed() const { return m_FrameEntitiesProcessed; }

	private:
		float		m_WorldSize;
		float		m_FirstChunkSize;
		const int	m_ChunkRowCount = 10;
		uint64		m_ChunkLayers = 2;

		glm::mat4 m_FrustumInitMatrix;
		HLFrustum m_CameraFrustum;

		float m_RenderDistanceSquared;
		float m_LOD1DistanceSquared;
		float m_LOD2DistanceSquared;

		std::vector<Entity> m_Entities;

		uint64 m_FrameEntitiesProcessed = 0;

	private:
		std::vector<Ref<SceneChunk>> m_Chunks;

		void GenerateFirstLayerChunks();
	};
}
