// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-11-18) initial release
//

#pragma once

#include <HighLo.h>
#include <gtest/gtest.h>

using namespace highlo;

struct ECSTests : public testing::Test
{
	ECS_Registry Registry;

	ECSTests()
	{
	}

	virtual ~ECSTests()
	{
	}
};

TEST_F(ECSTests, AddComponent)
{
	Entity entity;
	entity.AddComponent<IDComponent>();
	EXPECT_EQ(entity.HasComponent<IDComponent>(), true);
}

TEST_F(ECSTests, AddComponentWithValueChange)
{
	Entity entity;
	IDComponent *id = entity.AddComponent<IDComponent>();
	id->ID = 42;

	bool hasValue = false;
	Registry.ForEach<IDComponent>([&hasValue](UUID uuid, TransformComponent &transform, IDComponent &idComp)
	{
		if (idComp.ID == 42)
			hasValue = true;
	});

	EXPECT_EQ(hasValue, true);
}

TEST_F(ECSTests, ForEachMultipleComponent)
{
	for (uint64 i = 1; i < 10; i++)
	{
		Entity entity;
		entity.AddComponent<SceneComponent>();

		if (i % 2 == 0)
			entity.AddComponent<IDComponent>();
	}

	uint32 entityCount = 0;
	Registry.ForEachMultiple<SceneComponent, IDComponent>([&entityCount](UUID entityID, TransformComponent &transform, std::vector<void *> &components)
	{
		++entityCount;
	});

	EXPECT_EQ(entityCount, 4);
}

TEST_F(ECSTests, ViewTests)
{
	// APPROVED WORKING

	Ref<Scene> demoScene = Scene::CreateEmpty();
	Entity oneEntity = demoScene->CreateEntity("DemoName");
	Entity secondEntity = demoScene->CreateEntity("AnotherEntity");
	Entity thirdEntity = demoScene->CreateEntity("LOL");
	thirdEntity.AddComponent<SpriteComponent>();

	std::cout << "First Entity ID: " << oneEntity.GetUUID() << std::endl;
	std::cout << "Second Entity ID: " << secondEntity.GetUUID() << std::endl;
	std::cout << "Third Entity ID: " << thirdEntity.GetUUID() << std::endl;

	auto view = demoScene->GetRegistry().View<SpriteComponent>();
	for (auto entityId : view)
	{
		std::cout << entityId << std::endl;
	}
}

// TODO
TEST_F(ECSTests, ForEachMultipleComponentWithValueChange)
{
	/*
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

	bool hasValues = false;
	Registry.ForEachMultiple<SceneComponent, IDComponent>([&hasValues](UUID entityID, TransformComponent &transform, std::vector<void *> &components)
	{
		SceneComponent *sceneComponent = reinterpret_cast<SceneComponent *>(components[0]);
		IDComponent *idComponent = reinterpret_cast<IDComponent *>(components[1]);
	
		std::cout << "SceneID: " << sceneComponent->SceneID << std::endl;
		std::cout << "ID     : " << idComponent->ID << std::endl;
		std::cout << std::endl;
	
		if (sceneComponent->SceneID == 42 && idComponent->ID != 0)
		{
			hasValues = true;
		}
	});

	EXPECT_EQ(hasValues, true);
	*/
}

