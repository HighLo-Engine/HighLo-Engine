#include "HighLoPch.h"
#include "Prefab.h"

namespace highlo
{
	Prefab::Prefab()
	{
		m_Scene = Scene::CreateEmpty();
	}
	
	Prefab::Prefab(Entity e)
	{
		m_Scene = Scene::CreateEmpty();
		m_Entity = CreatePrefabFromEntity(e);
	}
	
	Prefab::~Prefab()
	{

	}
	
	Entity Prefab::CreatePrefabFromEntity(Entity entity)
	{
		Entity newEntity = m_Scene->CreateEntity();

		newEntity.AddComponent<PrefabComponent>();

		// TODO: copy all other components

		return newEntity;
	}
}

