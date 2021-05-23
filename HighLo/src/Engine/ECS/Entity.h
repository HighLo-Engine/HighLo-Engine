#pragma once

#include "Engine/Core/HLCore.h"
#include "Engine/Core/DataTypes/HLString.h"
#include "ECS_Registry.h"

namespace highlo
{
	class Entity
	{
	public:
		HLAPI Entity(const HLString& tag = "Entity");
		HLAPI Entity(EntityID id);
		HLAPI Entity(const Entity& entity);
		HLAPI Entity operator=(const Entity& entity);

		HLString Tag;
		const EntityID ID;
		TransformComponent* _TransformComponent;

		template <typename T>
		HLAPI T* AddComponent()
		{
			return ECS_Registry::Get().AddComponent<T>(ID);
		}

		template <typename T>
		HLAPI T* GetComponent()
		{
			return ECS_Registry::Get().GetComponent<T>(ID);
		}
	};
}

