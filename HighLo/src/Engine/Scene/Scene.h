#pragma once

#include "SceneChunk.h"
#include "Engine/Renderer/Light.h"
#include "Engine/Renderer/Environment.h"
#include "Engine/Camera/Camera.h"

namespace highlo
{
	using EntityMap = std::unordered_map<uint64, Entity>;

	class Scene : public IsSharedReference
	{
	public:

		HLAPI Scene(const HLString& name = "Scene1", float worldSize = 1000, bool isEditorScene = false);

		HLString m_Name;
		Ref<Camera> m_Camera;
		Light m_SceneLight;

		HLAPI void Update(Timestep ts);
		HLAPI void UpdateRuntime(Timestep ts);
		HLAPI void UpdateEditor(Timestep ts, const Camera& camera);
		HLAPI void UpdateSimulation(Timestep ts, const Camera& camera);
		HLAPI void OnEvent(Event &e);

		// Runtime
		HLAPI void OnRuntimeStart();
		HLAPI void OnRuntimeStop();

		// Simulation
		HLAPI void OnSimulationStart();
		HLAPI void OnSimulationEnd();

		HLAPI void SetViewportSize(uint32 width, uint32 height);
		HLAPI void SetSkybox(const Ref<Texture3D> &skybox);

		HLAPI Entity GetMainCameraEntity();
		HLAPI Entity CreateEntity(const HLString &name = "");
		HLAPI Entity CreateEntityWithID(uint64 uuid, const HLString &name = "", bool runtimeMap = false);
		HLAPI Entity FindEntityByTag(const HLString &tag);
		HLAPI Entity FindEntityByUUID(uint64 id);

		HLAPI void DestroyEntity(Entity entity);
		HLAPI void DuplicateEntity(Entity entity);
		HLAPI void CopyTo(Ref<Scene> &target);

		HLAPI void ConvertToLocalSpace(Entity entity);
		HLAPI void ConvertToWorldSpace(Entity entity);
		HLAPI glm::mat4 GetTransformRelativeToParent(Entity entity);
		HLAPI glm::mat4 GetWorldSpaceTransformMatrix(Entity entity);
		HLAPI TransformComponent GetWorldSpaceTransform(Entity entity);

		HLAPI void ParentEntity(Entity entity, Entity parent);
		HLAPI void UnparentEntity(Entity entity);
		HLAPI static Ref<Scene> GetScene(uint64 uuid);

		HLAPI const EntityMap &GetEntityMap() const { return m_EntityIDMap; }
		HLAPI uint64 GetUUID() const { return m_SceneID; }
		HLAPI bool IsPlaying() const { return m_IsPlaying; }
		HLAPI float &GetSkyboxLod() { return m_SkyboxLod; }
		HLAPI float GetSkyboxLod() const { return m_SkyboxLod; }
		HLAPI Light &GetLight() { return m_SceneLight; }
		HLAPI const Light &GetLight() const { return m_SceneLight; }
		HLAPI const Ref<Environment> &GetEnvironment() const { return m_Environment; }

		template<typename T>
		HLAPI auto GetAllEntitiesWith()
		{
			return m_Registry.view<T>();
		}

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
		const int32	m_ChunkRowCount = 10;
		uint64		m_ChunkLayers = 2;

		uint64		m_SceneID = 0;
		uint32		m_ViewportWidth = 0;
		uint32		m_ViewportHeight = 0;

		glm::mat4 m_FrustumInitMatrix;
		HLFrustum m_CameraFrustum;
		EntityMap m_EntityIDMap;

		Ref<Environment> m_Environment;
		float m_EnvironmentIntensity = 1.0f;
		Ref<Texture3D> m_SkyboxTexture;
		Ref<Material> m_SkyboxMaterial;

		float m_RenderDistanceSquared;
		float m_LOD1DistanceSquared;
		float m_LOD2DistanceSquared;
		float m_SkyboxLod = 1.0f;

		bool m_IsPlaying = false;
		bool m_ShouldSimulate = false;

		std::vector<Entity> m_Entities;

		uint64 m_FrameEntitiesProcessed = 0;

	private:
		std::vector<Ref<SceneChunk>> m_Chunks;

		void GenerateFirstLayerChunks();
	};
}
