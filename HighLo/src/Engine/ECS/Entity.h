// Copyright (c) 2021 Albert Slepak and Can Karka. All rights reserved.

//
// version history:
//     - 1.1 (2021-11-10) big refactoring to support child entities and parents
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/DataTypes/String.h"
#include "ECS_Registry.h"
#include "Engine/Math/Transform.h"

namespace highlo
{
	/// <summary>
	/// Forward declarations
	/// </summary>
	class Scene;

	class Entity
	{
	public:

		HLAPI Entity() = default;
		HLAPI Entity(UUID sceneID, const HLString &tag = "Entity");
		HLAPI Entity(UUID sceneID, UUID entityID);
		HLAPI Entity(UUID sceneID, UUID entityID, const HLString &tag = "Entity", const Transform &transform = Transform::Identity());
		HLAPI Entity(const Entity &other);
		HLAPI Entity &operator=(const Entity &other);

		template<typename T>
		HLAPI T *AddComponent()
		{
			return ECS_Registry::Get().AddComponent<T>(m_ID);
		}

		template<typename T>
		HLAPI T *AddOrReplace(Entity srcEntity)
		{
			if (!srcEntity.HasComponent<T>())
				return nullptr;

			T *componentToCopy = srcEntity.GetComponent<T>();
			return ECS_Registry::Get().AddOrReplace<T>(m_ID, componentToCopy);
		}

		template<typename T>
		HLAPI T *GetComponent() const
		{
			return ECS_Registry::Get().GetComponent<T>(m_ID);
		}

		template<typename T>
		HLAPI bool HasComponent() const
		{
			return ECS_Registry::Get().HasComponent<T>(m_ID);
		}

		template<typename T>
		HLAPI void RemoveComponent()
		{
			ECS_Registry::Get().RemoveComponent<T>(m_ID);
		}

		HLAPI UUID GetUUID() const { return m_ID; }
		
		HLAPI void SetTransform(const Transform &transform) { m_Transform = transform; }
		HLAPI Transform &Transform() { return m_Transform; }
		HLAPI const highlo::Transform &Transform() const { return m_Transform; }
		
		HLAPI void SetTag(const HLString &tag) { m_Tag = tag; }
		HLAPI HLString &Tag() { return m_Tag; }
		HLAPI const HLString &Tag() const { return m_Tag; }

		HLAPI void SetParent(Entity other);
		HLAPI std::vector<UUID> &Children();
		HLAPI const std::vector<UUID> &Children() const;

		HLAPI void SetParentUUID(UUID uuid);
		HLAPI UUID GetParentUUID() const;
		HLAPI bool HasParent() const;
		HLAPI Entity GetParent();
		HLAPI bool RemoveChild(Entity child);

		HLAPI bool IsAncesterOf(Entity other);
		HLAPI bool IsDescendantOf(Entity other);

		HLAPI bool IsHidden() const { return m_Hidden; }
		HLAPI void Show() { m_Hidden = false; }
		HLAPI void Hide() { m_Hidden = true; }

		HLAPI bool operator==(const Entity &other) const { return m_ID == other.m_ID; }
		HLAPI bool operator!=(const Entity &other) const { return !(*this == other); }
		HLAPI operator bool() const { return m_Initialized; }
		HLAPI operator uint64() const { return (uint64)m_ID; }

	private:

		bool m_Initialized = false;
		bool m_Hidden = false;

		HLString m_Tag;
		UUID m_ID;
		UUID m_SceneID;
		
		highlo::Transform m_Transform;
	};
}

