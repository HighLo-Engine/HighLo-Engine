// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

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

		Renderer::Submit([&]() {
			Renderer2D::BeginScene(overlayCamera.GetProjection());
			Renderer2D::DrawQuad(Transform::FromPosition({ -0.25f, 0.0f, 0.0f }), glm::vec4(0.941f, 0.502f, 0.502f, 1.0f));
			Renderer2D::DrawQuad(Transform::FromPosition({ 0.0f, -0.25f, -0.9f }), glm::vec4(0.641f, 0.502f, 0.902f, 1.0f));
			Renderer2D::EndScene();
		});

		renderer->EndScene();
	}

	void Scene::OnUpdateRuntime(Ref<SceneRenderer> renderer, Timestep ts)
	{
		HL_PROFILE_FUNCTION();
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
		HL_PROFILE_FUNCTION();


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
		HL_PROFILE_FUNCTION();

		// TODO
		return Entity{};
	}
	
	Entity Scene::FindEntityByTag(const HLString &tag)
	{
		HL_PROFILE_FUNCTION();

		// TODO
		return Entity{};
	}

	Entity Scene::GetMainCameraEntity()
	{
		HL_PROFILE_FUNCTION();

		// TODO


		return {};
	}
	
	void Scene::ConvertToLocalSpace(Entity entity)
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
		Decompose(localTransform, translation, scale, rotation);
		entity.SetTransform(Transform::FromPosition(translation).Scale(scale).Rotate(rotation));
	}
	
	void Scene::ConvertToWorldSpace(Entity entity)
	{
		HL_PROFILE_FUNCTION();

		Entity parent = entity.GetParent();
		if (!parent)
			return;

		// TODO: test if this works
		glm::mat4 transform = GetTransformRelativeToParent(entity);
		glm::vec3 translation, rotation, scale;
		Decompose(transform, translation, scale, rotation);
		entity.SetTransform(Transform::FromPosition(translation).Scale(scale).Rotate(rotation));
	}
	
	glm::mat4 Scene::GetTransformRelativeToParent(Entity entity)
	{
		HL_PROFILE_FUNCTION();

		glm::mat4 transform(1.0f);

		Entity parent = entity.GetParent();
		if (parent)
			transform = GetTransformRelativeToParent(parent);

		return transform * entity.Transform().GetTransform();
	}
	
	glm::mat4 Scene::GetWorldSpaceTransformMatrix(Entity entity)
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
	
	TransformComponent Scene::GetWorldSpaceTransform(Entity entity)
	{
		HL_PROFILE_FUNCTION();

		glm::vec3 translation;
		glm::vec3 scale;
		glm::vec3 rotation;
		glm::mat4 transform = GetWorldSpaceTransformMatrix(entity);

		TransformComponent component;
		Transform trans;

		Decompose(transform, translation, scale, rotation);
		trans.SetPosition(translation);
		trans.SetScale(scale);
		trans.SetRotation(rotation);

		component.Transform = trans;
		return component;
	}
	
	void Scene::ParentEntity(Entity entity, Entity parent)
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
	
	void Scene::UnparentEntity(Entity entity, bool convertToWorldSpace)
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
	
	Entity Scene::CreateEntity(const HLString &name)
	{
		HL_PROFILE_FUNCTION();

		auto entity = Entity(m_SceneID, name);
		auto idComponent = entity.AddComponent<IDComponent>();
		idComponent->ID = {};

		entity.AddComponent<TransformComponent>();
		entity.AddComponent<RelationshipComponent>();

		m_EntityIDMap[idComponent->ID] = entity;
		return entity;
	}
	
	Entity Scene::CreateEntityWithUUID(UUID uuid, const HLString &name)
	{
		HL_PROFILE_FUNCTION();

		auto entity = Entity(m_SceneID, name);
		auto idComponent = entity.AddComponent<IDComponent>();
		idComponent->ID = uuid;

		entity.AddComponent<TransformComponent>();
		entity.AddComponent<RelationshipComponent>();

		HL_ASSERT(m_EntityIDMap.find(uuid) == m_EntityIDMap.end());
		m_EntityIDMap[uuid] = entity;
		return entity;
	}
	
	void Scene::DestroyEntity(Entity entity)
	{
		HL_PROFILE_FUNCTION();


	}
	
	Entity Scene::DuplicateEntity(Entity entity)
	{
		HL_PROFILE_FUNCTION();

		Entity newEntity = CreateEntity(entity.Tag());
		
		// TODO: copy all components

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
	
	Entity Scene::CreatePrefabEntity(Entity entity, const glm::vec3 *translation)
	{
		// TODO
		return entity;
	}
	
	Entity Scene::CreatePrefabEntity(Entity entity, Entity parent, const glm::vec3 *translation)
	{
		// TODO
		return entity;
	}
}