#include "Tests.h"
using namespace highlo;

static ECS_Registry m_ECS_Registry;

bool test_add_component()
{
	highloUnit::Timer timer("test_add_component");

	Entity entity;
	entity.AddComponent<TagComponent>();

	highloUnit::Test test;
	return test.AssertEqual(timer, entity.HasComponent<TagComponent>(), true);
}

