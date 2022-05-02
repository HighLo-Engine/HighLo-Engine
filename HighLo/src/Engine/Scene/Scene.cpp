// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Scene.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/Renderer2D.h"
#include "Engine/Core/Profiler/ProfilerTimer.h"

#include "Engine/ECS/Prefab.h"

namespace highlo
{
	static const HLString s_DefaultEntityName = "Entity";
	static std::unordered_map<UUID, Scene*> s_ActiveScenes;

	Scene::Scene(const HLString &name, bool isEditorScene, bool constructScene)
		: m_DebugName(name), m_IsEditorScene(isEditorScene)
	{
		if (constructScene)
		{
			s_ActiveScenes[m_SceneID] = this;
			Init();
		}
	}
	
	Scene::~Scene()
	{
		s_ActiveScenes.erase(m_SceneID);
	}

	void Scene::Init()
	{
		m_SkyboxShader = Renderer::GetShaderLibrary()->Get("Skybox");
		m_SkyboxMaterial = Material::Create(m_SkyboxShader);
		m_SkyboxMaterial->SetFlag(MaterialFlag::DepthTest, false);
	}
	
	void Scene::UpdateScene(Timestep ts)
	{
		HL_PROFILE_FUNCTION();
	}

	void Scene::OnUpdateOverlay(Ref<SceneRenderer> renderer, Timestep ts, const Camera &overlayCamera)
	{
		HL_PROFILE_FUNCTION();

		renderer->BeginScene(overlayCamera);

		/*
		Renderer::Submit([&]() {
			Renderer2D::BeginScene(overlayCamera);
			Renderer2D::DrawQuad(Transform::FromPosition({ -0.25f, 0.0f, 0.0f }), glm::vec4(0.941f, 0.502f, 0.502f, 1.0f));
			Renderer2D::DrawQuad(Transform::FromPosition({ 0.0f, -0.25f, -0.9f }), glm::vec4(0.641f, 0.502f, 0.902f, 1.0f));
			Renderer2D::FillCircle(Transform::FromPosition({ 6.0f, 0.25f, 0.0f }), 1.0f, 1.0f, glm::vec4(0.8f, 0.2f, 0.3f, 1.0f));
			Renderer2D::DrawCircle(Transform::FromPosition({ -6.0f, 0.25f, 0.0f }), 1.0f, glm::vec4(0.8f, 0.2f, 0.3f, 1.0f));
		//	Renderer2D::DrawLine({ 0.0f, 0.0f }, { 6.0f, 6.0f }, glm::vec4(0.2f, 0.3f, 9.0f, 1.0f));
			Renderer2D::DrawText("Hello World!", { 0.0f, 2.0f, 0.0f }, 100.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
			Renderer2D::EndScene();
		});
		*/

		Renderer2D::BeginScene(overlayCamera);
		Renderer2D::DrawQuad(Transform::FromPosition({ -0.25f, 0.0f, 0.0f }), glm::vec4(0.941f, 0.502f, 0.502f, 1.0f));
		Renderer2D::DrawQuad(Transform::FromPosition({ 0.0f, -0.25f, -0.9f }), glm::vec4(0.641f, 0.502f, 0.902f, 1.0f));
		Renderer2D::FillCircle(Transform::FromPosition({ 6.0f, 0.25f, 0.0f }), 1.0f, 1.0f, glm::vec4(0.8f, 0.2f, 0.3f, 1.0f));
		Renderer2D::DrawCircle(Transform::FromPosition({ -6.0f, 0.25f, 0.0f }), 1.0f, glm::vec4(0.8f, 0.2f, 0.3f, 1.0f));
		//	Renderer2D::DrawLine({ 0.0f, 0.0f }, { 6.0f, 6.0f }, glm::vec4(0.2f, 0.3f, 9.0f, 1.0f));
		Renderer2D::DrawText("Hello World!", { 0.0f, 2.0f, 0.0f }, 100.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
		Renderer2D::EndScene();


		renderer->EndScene();
	}

	void Scene::OnUpdateRuntime(Ref<SceneRenderer> renderer, Timestep ts)
	{
		HL_PROFILE_FUNCTION();

		Entity mainCameraEntity = GetMainCameraEntity();
		if (!mainCameraEntity)
			return;

		CameraComponent *cameraComp = mainCameraEntity.GetComponent<CameraComponent>();
		if (!cameraComp->Primary)
			return;

		renderer->BeginScene(cameraComp->Camera);
		renderer->EndScene();
	}
	
	void Scene::OnUpdateEditor(Ref<SceneRenderer> renderer, Timestep ts, const EditorCamera &editorCamera)
	{
		HL_PROFILE_FUNCTION();

		renderer->BeginScene(editorCamera);
		renderer->EndScene();
	}
	
	void Scene::OnSimulate(Ref<SceneRenderer> renderer, Timestep ts, const EditorCamera &editorCamera)
	{
		HL_PROFILE_FUNCTION();

		renderer->BeginScene(editorCamera);
		renderer->EndScene();
	}
	
	void Scene::OnEvent(Event &e)
	{
	}
	
	void Scene::OnRuntimeStart()
	{

	}
	
	void Scene::OnRuntimeStop()
	{
	}
	
	void Scene::OnSimulationStart()
	{
	}
	
	void Scene::OnSimulationStop()
	{
	}
	
	void Scene::SetViewportSize(uint32 width, uint32 height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;
	}
	
	void Scene::SetSkybox(const Ref<Texture3D> &skybox)
	{
	}
	
	Entity Scene::FindEntityByUUID(UUID id)
	{
		auto &view = m_Registry.View<RelationshipComponent>();
		for (UUID entityID : view)
		{
			if (entityID == id && m_EntityIDMap.find(id) != m_EntityIDMap.end())
				return m_EntityIDMap.at(id);
		}

		return Entity{};
	}
	
	Entity Scene::FindEntityByTag(const HLString &tag)
	{
		auto &view = m_Registry.View<RelationshipComponent>();
		for (UUID entityID : view)
		{
			HL_ASSERT(m_EntityIDMap.find(entityID) != m_EntityIDMap.end());
			Entity e = m_EntityIDMap.at(entityID);
			if (e.Tag() == tag)
			{
				return e;
			}
		}

		return Entity{};
	}

	Entity Scene::GetMainCameraEntity()
	{
		HL_PROFILE_FUNCTION();

		auto &view = m_Registry.View<CameraComponent>();
		for (UUID entityID : view)
		{
			HL_ASSERT(m_EntityIDMap.find(entityID) != m_EntityIDMap.end());
			Entity e = m_EntityIDMap.at(entityID);
			CameraComponent *c = e.GetComponent<CameraComponent>();
			if (c->Primary)
			{
				return e;
			}
		}

		HL_CORE_ERROR("Could not find any matching entity!");
		return Entity{};
	}
	
	void Scene::ConvertToLocalSpace(Entity &entity)
	{
		HL_PROFILE_FUNCTION();

		Entity parent = entity.GetParent();
		if (!parent)
			return;

		auto &transform = entity.Transform();
		glm::mat4 parentTransform = GetWorldSpaceTransformMatrix(parent);

		glm::mat4 localTransform = glm::inverse(parentTransform) * transform.GetTransform();
		
		// TODO: test if this works
		glm::vec3 translation, rotation, scale;
		Math::Decompose(localTransform, translation, scale, rotation);
		entity.SetTransform(Transform::FromPosition(translation).Scale(scale).Rotate(rotation));
	}
	
	void Scene::ConvertToWorldSpace(Entity &entity)
	{
		HL_PROFILE_FUNCTION();

		Entity parent = entity.GetParent();
		if (!parent)
			return;

		// TODO: test if this works
		glm::mat4 transform = GetTransformRelativeToParent(entity);
		glm::vec3 translation, rotation, scale;
		Math::Decompose(transform, translation, scale, rotation);
		entity.SetTransform(Transform::FromPosition(translation).Scale(scale).Rotate(rotation));
	}
	
	glm::mat4 Scene::GetTransformRelativeToParent(Entity &entity)
	{
		HL_PROFILE_FUNCTION();

		glm::mat4 transform(1.0f);

		Entity parent = entity.GetParent();
		if (parent)
			transform = GetTransformRelativeToParent(parent);

		return transform * entity.Transform().GetTransform();
	}
	
	glm::mat4 Scene::GetWorldSpaceTransformMatrix(Entity &entity)
	{
		HL_PROFILE_FUNCTION();

		glm::mat4 transform = entity.Transform().GetTransform();
		while (Entity parent = FindEntityByUUID(entity.GetParentUUID()))
		{
			transform = parent.Transform().GetTransform();
			entity = parent;
		}

		return transform;
	}
	
	Transform Scene::GetWorldSpaceTransform(Entity &entity)
	{
		HL_PROFILE_FUNCTION();

		glm::vec3 translation, scale, rotation;
		glm::mat4 transform = GetWorldSpaceTransformMatrix(entity);

		Transform result;
		Math::Decompose(transform, translation, scale, rotation);
		result.SetPosition(translation);
		result.SetScale(scale);
		result.SetRotation(rotation);
		return result;
	}
	
	void Scene::ParentEntity(Entity &entity, Entity &parent)
	{
		HL_PROFILE_FUNCTION();

		if (parent.IsDescendantOf(entity))
		{
			UnparentEntity(parent);

			Entity newParent = FindEntityByUUID(entity.GetUUID());
			if (newParent)
			{
				UnparentEntity(entity);
				ParentEntity(parent, newParent);
			}
		}
		else
		{
			Entity prevParent = FindEntityByUUID(entity.GetParentUUID());
			if (prevParent)
				UnparentEntity(entity);
		}

		entity.SetParentUUID(parent.GetUUID());
		parent.Children().push_back(entity.GetUUID());
		ConvertToLocalSpace(entity);
	}
	
	void Scene::UnparentEntity(Entity &entity, bool convertToWorldSpace)
	{
		HL_PROFILE_FUNCTION();

		Entity parent = entity.GetParent();
		if (!parent)
			return;

		auto &parentChildren = parent.Children();
		parentChildren.erase(std::remove(parentChildren.begin(), parentChildren.end(), entity.GetUUID()), parentChildren.end());

		if (convertToWorldSpace)
			ConvertToWorldSpace(entity);

		entity.SetParentUUID(0);
	}
	
	void Scene::CopyTo(Ref<Scene> &target)
	{
		HL_PROFILE_FUNCTION();

		// Lights
		target->m_Light = m_Light;
		target->m_LightMultiplier = m_LightMultiplier;

		// Skybox
		target->m_Environment = m_Environment;
		target->m_SkyboxTexture = m_SkyboxTexture;
		target->m_SkyboxMaterial = m_SkyboxMaterial;
		target->m_SkyboxShader = m_SkyboxShader;
		target->m_SkyboxLod = m_SkyboxLod;

		// 

	}
	
	Ref<Scene> Scene::GetScene(UUID uuid)
	{
		if (s_ActiveScenes.find(uuid) != s_ActiveScenes.end())
			return s_ActiveScenes.at(uuid);

		return {};
	}
	
	Ref<Scene> Scene::CreateEmpty()
	{
		return Scene::Create("Empty", false, false);
	}

	Ref<Scene> Scene::Create(const HLString &name, bool isEditorScene, bool constructScene)
	{
		return Ref<Scene>::Create(name, isEditorScene, constructScene);
	}

	Scene *Scene::GetActiveScene()
	{
		Scene *selected = nullptr;
		for (auto &[id, scene] : s_ActiveScenes)
			selected = scene;

		return selected;
	}

	void Scene::CopyAllComponents(Entity &dest, const Entity &src)
	{
		PrefabComponent *pc = dest.AddOrReplace<PrefabComponent>(src);						// HL_ASSERT(pc, "Prefab Component could not be copied!");
		SceneComponent *sc = dest.AddOrReplace<SceneComponent>(src);						// HL_ASSERT(sc);
		CameraComponent *cc = dest.AddOrReplace<CameraComponent>(src);						// HL_ASSERT(cc);
		SpriteComponent *spriteComponent = dest.AddOrReplace<SpriteComponent>(src);			// HL_ASSERT(spriteComponent);
		StaticModelComponent *smc = dest.AddOrReplace<StaticModelComponent>(src);			// HL_ASSERT(smc);
		DynamicModelComponent *dmc = dest.AddOrReplace<DynamicModelComponent>(src);			// HL_ASSERT(dmc);
		DirectionalLightComponent *dlc = dest.AddOrReplace<DirectionalLightComponent>(src); // HL_ASSERT(dlc);
		PointLightComponent *plc = dest.AddOrReplace<PointLightComponent>(src);				// HL_ASSERT(plc);
		SkyLightComponent *slc = dest.AddOrReplace<SkyLightComponent>(src);					// HL_ASSERT(slc);
		TextComponent *tc = dest.AddOrReplace<TextComponent>(src);							// HL_ASSERT(tc);

	}
	
	void Scene::AddEntity(Entity &entity)
	{
		m_EntityIDMap[entity.GetUUID()] = entity;
	}

	void Scene::UpdateEntity(Entity &entity)
	{
		HL_ASSERT(m_EntityIDMap.find(entity.GetUUID()) != m_EntityIDMap.end());
		m_EntityIDMap[entity.GetUUID()] = entity;
	}

	Entity Scene::CreateEntity(const HLString &name)
	{
		HL_PROFILE_FUNCTION();

		auto entity = Entity(m_SceneID, name);

		entity.AddComponent<RelationshipComponent>();
		entity.SetTransform(Transform::FromPosition({ 0.0f, 0.0f, 0.0f }));

		m_EntityIDMap[entity.GetUUID()] = entity;
		return entity;
	}
	
	Entity Scene::CreateEntityWithUUID(UUID uuid, const HLString &name)
	{
		HL_PROFILE_FUNCTION();

		auto entity = Entity(m_SceneID, name);

		entity.AddComponent<RelationshipComponent>();
		entity.SetTransform(Transform::FromPosition({ 0.0f, 0.0f, 0.0f }));

		HL_ASSERT(m_EntityIDMap.find(uuid) == m_EntityIDMap.end());
		m_EntityIDMap[uuid] = entity;
		return entity;
	}
	
	void Scene::DestroyEntity(Entity &entity, bool excludeChildren, bool first)
	{
		HL_PROFILE_FUNCTION();

		if (!m_IsEditorScene)
		{

		}

		if (!excludeChildren)
		{
			for (uint64 i = 0; i < entity.Children().size(); ++i)
			{
				auto childId = entity.Children()[i];
				Entity child = FindEntityByUUID(childId);
				if (child)
					DestroyEntity(child, excludeChildren, false);
			}
		}

		if (first)
		{
			if (entity.HasParent())
				entity.GetParent().RemoveChild(entity);
		}

		m_Registry.DestroyAllByEntityId(entity.GetUUID());
	}
	
	Entity Scene::DuplicateEntity(Entity &entity)
	{
		HL_PROFILE_FUNCTION();

		Entity newEntity = CreateEntity(entity.Tag());
		newEntity.SetTransform(entity.Transform());
		CopyAllComponents(newEntity, entity);

		auto childIds = entity.Children();
		for (auto childId : childIds)
		{
			Entity childDuplicate = DuplicateEntity(FindEntityByUUID(childId));
			UnparentEntity(childDuplicate, false);

			childDuplicate.SetParentUUID(newEntity.GetUUID());
			newEntity.Children().push_back(childDuplicate.GetUUID());
		}

		if (entity.HasParent())
		{
			Entity parent = FindEntityByUUID(entity.GetParentUUID());
			HL_ASSERT(parent, "Failed to find parent entity");

			newEntity.SetParentUUID(entity.GetParentUUID());
			parent.Children().push_back(newEntity.GetUUID());
		}

		return newEntity;
	}
}
