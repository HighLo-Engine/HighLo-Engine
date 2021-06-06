#pragma once

#include "Engine/Core/HLCore.h"
#include "Engine/Core/DataTypes/HLString.h"
#include "ECS_Registry.h"

namespace highlo
{
	class Entity
	{
	public:

		HLAPI Entity(const HLString &tag = "Entity");
		HLAPI Entity(EntityID id);
		HLAPI Entity(const Entity &entity);
		HLAPI Entity operator=(const Entity &entity);

		HLString Tag;
		const EntityID ID;
		TransformComponent *_TransformComponent;

		template <typename T>
		HLAPI T *AddComponent()
		{
			return ECS_Registry::Get().AddComponent<T>(ID);
		}

		template <typename T>
		HLAPI T *GetComponent()
		{
			return ECS_Registry::Get().GetComponent<T>(ID);
		}

		template <typename T>
		HLAPI bool HasComponent()
		{
			return ECS_Registry::Get().HasComponent<T>(ID);
		}

		template <typename T>
		HLAPI T *FindComponentByTag(const HLString &tag)
		{
			std::vector<T*> &components = ECS_Registry::Get().GetComponents();
			for (T *component : components)
			{
				if (component->Tag == tag)
				{
					return component;
				}
			}

			return nullptr;
		}

		template <typename T>
		HLAPI T *FindComponentByID(EntityID id)
		{
			std::vector<T*> &components = ECS_Registry::Get().GetComponents();
			for (T *component : components)
			{
				if (component->ID == id)
				{
					return component;
				}
			}

			return nullptr;
		}

		template <typename T>
		HLAPI void RemoveComponent()
		{
			ECS_Registry::Get().RemoveComponent<T>(ID);
		}

		HLAPI bool operator==(const Entity &other) const
		{
			return ID == other.ID;
		}

		HLAPI bool operator!=(const Entity &other) const
		{
			return !(*this == other);
		}
	};
}

