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
		HLAPI Entity(const Entity &other);
		HLAPI Entity operator=(const Entity &other);

		template <typename T>
		HLAPI T *AddComponent()
		{
			return ECS_Registry::Get().AddComponent<T>(m_ID);
		}

		template <typename T>
		HLAPI T *GetComponent() const
		{
			return ECS_Registry::Get().GetComponent<T>(m_ID);
		}

		template <typename T>
		HLAPI bool HasComponent() const
		{
			return ECS_Registry::Get().HasComponent<T>(m_ID);
		}

		template <typename T>
		HLAPI void RemoveComponent()
		{
			ECS_Registry::Get().RemoveComponent<T>(m_ID);
		}

		HLAPI UUID GetUUID() const { return m_ID; }
		HLAPI void SetTransform(const Transform &transform) { m_TransformComponent->Transform = transform; }
		HLAPI Transform &Transform() { return m_TransformComponent->Transform; }
		HLAPI const highlo::Transform &Transform() const { return m_TransformComponent->Transform; }
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

		HLAPI bool operator==(const Entity &other) const;
		HLAPI bool operator!=(const Entity &other) const;
		HLAPI operator bool() const;
		HLAPI operator uint64() const;

	private:

		bool m_Initialized = false;
		HLString m_Tag;
		const UUID m_ID = 0;
		UUID m_SceneID = 0;
		TransformComponent *m_TransformComponent;
	};
}

