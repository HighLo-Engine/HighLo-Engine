#include "Tests.h"
using namespace highlo;

#include <glm/glm.hpp>

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

	for (uint64_t i = 1; i < 10; i++)
	{
		Entity entity;
		auto transform = entity.AddComponent<TransformComponent>();
		transform->Transform.SetTransform(glm::mat4(1.0f));

		if (i % 2 == 0)
		{
			auto id = entity.AddComponent<IDComponent>();
			id->ID = 2000 + i;
		}
	}

	uint32_t entityCount = 0;

	s_ECS_Registry.ForEachMultiple<TransformComponent, IDComponent>([&entityCount](EntityID entityID, TransformComponent& transform, std::vector<void*> components) {
		auto transformComponent = reinterpret_cast<TransformComponent*>(components[0]);
		auto idComponent = reinterpret_cast<IDComponent*>(components[1]);

		entityCount++;
	});

	constexpr auto CORRECT_SELECTED_ENTITY_COUNT = 4;

	highloUnit::Test test;
	return test.AssertEqual(timer, entityCount == CORRECT_SELECTED_ENTITY_COUNT, true);
}
