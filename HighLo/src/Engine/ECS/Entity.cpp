// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Entity.h"

#include "Engine/ECS/Components.h"
#include "Engine/Scene/Scene.h"

namespace highlo
{
	Entity::Entity(UUID sceneID, const HLString &tag)
		: m_ID(UUID()), m_Tag(tag), m_SceneID(sceneID), m_Initialized(true), m_Hidden(false)
	{
		m_Transform = Transform::Identity();
	}

	Entity::Entity(UUID sceneID, UUID entityID)
		: m_ID(entityID), m_SceneID(sceneID), m_Tag("Entity"), m_Initialized(true), m_Hidden(false)
	{
		m_Transform = Transform::Identity();
	}

	Entity::Entity(UUID sceneID, UUID entityID, const HLString &tag, const highlo::Transform &transform)
		: m_ID(entityID), m_SceneID(sceneID), m_Tag(tag), m_Initialized(true), m_Hidden(false), m_Transform(transform)
	{
	}
	
	Entity::Entity(const Entity &other)
		: m_ID(other.m_ID),
		m_Tag(other.m_Tag), 
		m_Transform(other.m_Transform), 
		m_SceneID(other.m_SceneID), 
		m_Initialized(other.m_Initialized), 
		m_Hidden(other.m_Hidden)
	{
	}
	
	Entity &Entity::operator=(const Entity &other)
	{
		m_ID					= other.m_ID;
		m_SceneID				= other.m_SceneID;
		m_Tag					= other.m_Tag;
		m_Initialized			= other.m_Initialized;
		m_Transform				= other.m_Transform;
		m_Hidden				= other.m_Hidden;

		return *this;
	}

	void Entity::SetParent(Entity other)
	{
		SetParentUUID(other.GetUUID());
		other.Children().emplace_back(GetUUID());
	}

	std::vector<UUID> &Entity::Children()
	{
		return GetComponent<RelationshipComponent>()->Children;
	}

	const std::vector<UUID> &Entity::Children() const
	{
		return GetComponent<RelationshipComponent>()->Children;
	}

	void Entity::SetParentUUID(UUID uuid)
	{
		GetComponent<RelationshipComponent>()->ParentHandle = uuid;
	}

	UUID Entity::GetParentUUID() const
	{
		return GetComponent<RelationshipComponent>()->ParentHandle;
	}

	bool Entity::HasParent() const
	{
		return Scene::GetScene(m_SceneID)->FindEntityByUUID(GetParentUUID());
	}

	Entity Entity::GetParent()
	{
		if (!HasParent())
			return *this;

		return Scene::GetScene(m_SceneID)->FindEntityByUUID(GetParentUUID());
	}

	bool Entity::RemoveChild(Entity child)
	{
		UUID childId = child.GetUUID();
		auto &children = Children();

		auto it = std::find(children.begin(), children.end(), childId);
		if (it != children.end())
		{
			children.erase(it);
			return true;
		}

		return false;
	}

	bool Entity::IsAncesterOf(Entity other)
	{
		const auto &children = Children();

		if (children.size() == 0)
			return false;

		for (UUID child : children)
		{
			if (child == other.GetUUID())
				return true;
		}

		for (UUID child : children)
		{
			if (Scene::GetScene(m_SceneID)->FindEntityByUUID(child))
				return true;
		}

		return false;
	}

	bool Entity::IsDescendantOf(Entity other)
	{
		return other.IsAncesterOf(*this);
	}
}

