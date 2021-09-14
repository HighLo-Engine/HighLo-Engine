// Copyright (c) 2021 Albert Slepak and Can Karka. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/ECS/Entity.h"
#include "Engine/Camera/Camera.h"
#include "Engine/Math/Frustum.h"

namespace highlo
{
	class SceneChunk : public IsSharedReference
	{
	public:
		SceneChunk(uint64 layer, uint64 deepestLayer, float size, const glm::vec3& position);

		void Update(Timestep ts, Camera& Camera, HLFrustum& CameraFrustum, uint64& processedEntityCount);
		void AddEntity(Entity entity);

		void SetRenderDistance(float distance);
		void SetLOD1Distance(float distance);
		void SetLOD2Distance(float distance);

		inline void SetDeepestLayer(uint64 layer) { m_DeepestLayer; }

		inline const uint64 GetEntityCount() const { return m_Entities.size(); }

		void RenderDebugMesh();

		inline const float GetSize() const { return m_Size; }
		inline const glm::vec3& GetPosition() const { return m_Position; }

	private:
		void		Init();

		uint64		m_Layer;
		uint64		m_DeepestLayer;
		uint64		m_ChunkRowCount = 5;
		float		m_Size;
		glm::vec3	m_Position;
		Ref<Mesh>	m_Mesh;

		float m_RenderDistanceSquared;
		float m_LOD1DistanceSquared;
		float m_LOD2DistanceSquared;

		std::vector<Entity> m_Entities;
		std::map<EntityID, RenderComponent*> m_RenderComponentMap;

		void ProcessEntities(Timestep ts, Camera& camera, HLFrustum& cameraFrustum, uint64& processedEntityCount);
		void ProcessSubChunks(Timestep ts, Camera& camera, HLFrustum& cameraFrustum, uint64& processedEntityCount);

	private:
		std::vector<Ref<SceneChunk>> m_SubChunks;

		void GenerateSubChunks();
	};
}
