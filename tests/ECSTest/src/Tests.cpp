#include "Tests.h"
using namespace highlo;

static ECS_Registry s_ECS_Registry;

bool test_add_component()
{
	highloUnit::Timer timer("test_add_component");

	Entity entity;
	entity.AddComponent<IDComponent>();

	highloUnit::Test test;
	return test.AssertEqual(timer, entity.HasComponent<IDComponent>(), true);
}

bool test_add_component_with_value_change()
{
	highloUnit::Timer timer("test_add_component_with_value_change");

	Entity entity;
	IDComponent *id = entity.AddComponent<IDComponent>();
	id->ID = 42;

	bool hasValue = false;
	s_ECS_Registry.ForEach<IDComponent>([&hasValue](UUID entityID, TransformComponent &transform, IDComponent &idComp)
	{
		if (idComp.ID == 42)
			hasValue = true;
	});

	highloUnit::Test test;
	return test.AssertEqual(timer, hasValue, true);
}

bool test_for_each_multiple_component()
{
	highloUnit::Timer timer("for_each_component");

	for (uint64 i = 1; i < 10; i++)
	{
		Entity entity;
		SceneComponent *scene = entity.AddComponent<SceneComponent>();
		scene->SceneID = 42;
		
		if (i % 2 == 0)
		{
			IDComponent *id = entity.AddComponent<IDComponent>();
			id->ID = 2000 + i;
		}
	}

	uint32 entityCount = 0;
	
	s_ECS_Registry.ForEachMultiple<SceneComponent, IDComponent>([&entityCount](UUID entityID, TransformComponent &transform, std::vector<void*> components) {
		SceneComponent *sceneComponent = reinterpret_cast<SceneComponent*>(components[0]);
		IDComponent *idComponent = reinterpret_cast<IDComponent*>(components[1]);
		++entityCount;
	});

	highloUnit::Test test;
	return test.AssertEqual<uint32>(timer, entityCount, 4);
}
