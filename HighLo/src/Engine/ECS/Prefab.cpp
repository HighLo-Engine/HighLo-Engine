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

	void Prefab::Create(Entity e, bool serialize)
	{
		m_Scene = Scene::CreateEmpty();
		m_Entity = CreatePrefabFromEntity(e);

		if (serialize)
		{
// TODO: Serialize data
		}
	}
	
	Entity Prefab::CreatePrefabFromEntity(Entity entity)
	{
		Entity newEntity = m_Scene->CreateEntity();

		newEntity.AddComponent<PrefabComponent>();

// TODO: copy all other components

		return newEntity;
	}
}

