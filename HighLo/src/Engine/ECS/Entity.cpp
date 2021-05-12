#include "HighLoPch.h"
#include "Entity.h"
#include "Engine/ECS/Components.h"

namespace highlo
{
	Entity::Entity(const HLString& tag) 
		: ID(ECS_Registry::Get().GenerateEntityID()), Tag(tag)
	{
		_TransformComponent = ECS_Registry::Get().AddComponent<TransformComponent>(ID);
	}

	Entity::Entity(EntityID id) 
		: ID(id), Tag("Entity")
	{
		_TransformComponent = ECS_Registry::Get().GetComponent<TransformComponent>(ID);
	}
	
	Entity::Entity(const Entity& entity) 
		: ID(entity.ID), Tag(entity.Tag), _TransformComponent(entity._TransformComponent)
	{
	}
	
	Entity Entity::operator=(const Entity& entity)
	{
		return Entity(entity.ID);
	}
}
