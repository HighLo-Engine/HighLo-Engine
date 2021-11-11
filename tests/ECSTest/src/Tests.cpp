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

bool for_each_component()
{
	highloUnit::Timer timer("for_each_component");

	for (uint64 i = 1; i < 10; i++)
	{
		Entity entity;
		auto transform = entity.AddComponent<SceneComponent>();
		transform->SceneID = 0;
		
		if (i % 2 == 0)
		{
			auto id = entity.AddComponent<IDComponent>();
			id->ID = 2000 + i;
		}
	}

	uint32 entityCount = 0;
	
	s_ECS_Registry.ForEachMultiple<SceneComponent, IDComponent>([&entityCount](UUID entityID, TransformComponent &transform, std::vector<void*> components) {
		auto sceneComponent = reinterpret_cast<SceneComponent*>(components[0]);
		auto idComponent = reinterpret_cast<IDComponent*>(components[1]);
		++entityCount;
	});

	highloUnit::Test test;
	return test.AssertEqual<uint32>(timer, entityCount, 4);
}
