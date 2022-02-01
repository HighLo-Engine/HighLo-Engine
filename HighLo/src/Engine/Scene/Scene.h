// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.1 (2021-09-15) Refactored Scene class
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Assets/Asset.h"
#include "Engine/Renderer/Light.h"
#include "Engine/Renderer/Environment.h"
#include "Engine/Renderer/Font.h"
#include "Engine/Camera/Camera.h"
#include "Engine/Camera/EditorCamera.h"
#include "Engine/ECS/Entity.h"
#include "Engine/Core/UUID.h"

namespace highlo
{
	using EntityMap = std::unordered_map<UUID, Entity>;
	class SceneRenderer;

	class Scene : public Asset
	{
	public:

		HLAPI Scene(const HLString &name = "Scene1", bool isEditorScene = false, bool constructScene = true);
		HLAPI ~Scene();

		// Initializes the scene.
		HLAPI void Init();

		// Updates the entities and other properties of the scene.
		HLAPI void UpdateScene(Timestep ts);

		// Renders the static overlay that gets rendered on top of the scene (optional).
		HLAPI void OnUpdateOverlay(Ref<SceneRenderer> renderer, Timestep ts, const Camera &overlayCamera);

		// Renders the scene at actual runtime of the game.
		HLAPI void OnUpdateRuntime(Ref<SceneRenderer> renderer, Timestep ts);

		// Renders the scene in the editor viewport.
		HLAPI void OnUpdateEditor(Ref<SceneRenderer> renderer, Timestep ts, const EditorCamera &editorCamera);

		// Renders the scene in the physics simulation.
		HLAPI void OnSimulate(Ref<SceneRenderer> renderer, Timestep ts, const EditorCamera &editorCamera);

		// Handles engine and game events.
		HLAPI void OnEvent(Event &e);

		// Runtime
		HLAPI void OnRuntimeStart();
		HLAPI void OnRuntimeStop();
		HLAPI void OnSimulationStart();
		HLAPI void OnSimulationStop();

		HLAPI void SetViewportSize(uint32 width, uint32 height);

		HLAPI const Ref<Environment> &GetEnvironment() const {}
		HLAPI void SetSkybox(const Ref<Texture3D> &skybox);

		HLAPI DirectionalLight &GetLight() { return m_Light; }
		HLAPI const DirectionalLight &GetLight() const { return m_Light; }

		HLAPI float &GetSkyboxLod() { return m_SkyboxLod; }
		HLAPI float GetSkyboxLod() const { return m_SkyboxLod; }

		HLAPI const EntityMap &GetEntityMap() const { return m_EntityIDMap; }
		HLAPI UUID GetUUID() const { return m_SceneID; }

		HLAPI void AddEntity(Entity &entity);
		HLAPI void UpdateEntity(Entity &entity);
		HLAPI void SetEntityTransform(Entity &entity, Transform &transform);
		HLAPI Entity CreateEntity(const HLString &name = "");
		HLAPI Entity CreateEntityWithUUID(UUID uuid, const HLString &name = "");
		HLAPI void DestroyEntity(Entity entity, bool excludeChildren = false, bool first = true);
		HLAPI Entity DuplicateEntity(Entity entity);

		template<typename T>
		HLAPI auto GetAllEntitiesWith()
		{
			return m_Registry.View<T>();
		}

		HLAPI Entity FindEntityByUUID(UUID id);
		HLAPI Entity FindEntityByTag(const HLString &tag);
		HLAPI Entity GetMainCameraEntity();

		HLAPI void ConvertToLocalSpace(Entity entity);
		HLAPI void ConvertToWorldSpace(Entity entity);
		HLAPI glm::mat4 GetTransformRelativeToParent(Entity entity);
		HLAPI glm::mat4 GetWorldSpaceTransformMatrix(Entity entity);
	//	HLAPI Transform GetWorldSpaceTransform(Entity entity);

		HLAPI void ParentEntity(Entity entity, Entity parent);
		HLAPI void UnparentEntity(Entity entity, bool convertToWorldSpace = true);

		HLAPI void CopyTo(Ref<Scene> &target);
		HLAPI static Ref<Scene> GetScene(UUID uuid);
		HLAPI static Ref<Scene> CreateEmpty();

		HLAPI bool IsEditorScene() const { return m_IsEditorScene; }
		HLAPI bool IsPlaying() const { return m_IsPlaying; }

		HLAPI static AssetType GetStaticType() { return AssetType::Scene; }
		HLAPI virtual AssetType GetAssetType() const override { return GetStaticType(); }

		HLAPI const HLString &GetName() const { return m_Name; }
		HLAPI void SetName(const HLString &name) { m_Name = name; }

		HLAPI ECS_Registry &GetRegistry() { return m_Registry; }
		HLAPI const ECS_Registry &GetRegistry() const { return m_Registry; }

		// TODO: Only in editor
		HLAPI void SetSelectedEntity(Entity entity) { m_SelectedEntity = entity; }

		HLAPI static Ref<Scene> Create(const HLString &name = "Scene1", bool isEditorScene = false, bool constructScene = true);

		HLAPI void CopyAllComponents(Entity &dest, const Entity &src);

	private:

		UUID m_SceneID = UUID();
		HLString m_Name = "Untitled Scene";
		HLString m_DebugName;
		bool m_IsEditorScene = false;
		uint32 m_ViewportWidth = 0, m_ViewportHeight = 0;

		EntityMap m_EntityIDMap;
		ECS_Registry m_Registry;

		DirectionalLight m_Light;
		float m_LightMultiplier = 0.3f;

		// Environment
		Ref<Environment> m_Environment;
		LightEnvironment m_LightEnvironment;
		float m_EnvironmentIntensity = 1.0f;
		float m_SkyboxLod = 1.0f;
		Ref<Texture3D> m_SkyboxTexture;
		Ref<Material> m_SkyboxMaterial;
		Ref<Shader> m_SkyboxShader;

		Entity m_SelectedEntity;

		bool m_IsPlaying = false;
		bool m_ShouldSimulate = false;

		friend class Entity;
		friend class SceneRenderer;
		friend class SceneHierarchyPanel;
	};
}

