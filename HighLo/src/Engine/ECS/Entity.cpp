#include "HighLoPch.h"
#include "Entity.h"
#include "Engine/ECS/Components.h"

namespace highlo
{
	Entity::Entity(const HLString& tag) 
		: ID(ECS_Registry::Get().GenerateEntityID()), Tag(tag)
	{
		ECS_Registry::Get().AddComponent<TransformComponent>(ID);
	}

	Entity::Entity(EntityID id) 
		: ID(id), Tag("Entity")
	{
	}
	
	Entity::Entity(const Entity& entity) 
		: ID(entity.ID), Tag(entity.Tag)
	{
	}
	
	Entity Entity::operator=(const Entity& entity)
	{
		return Entity(entity.ID);
	}
}
