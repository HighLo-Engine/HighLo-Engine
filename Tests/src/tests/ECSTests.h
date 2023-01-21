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
	Entity e(UUID(), "TestEntity");
	EXPECT_EQ(e.HasComponent<SpriteComponent>(), false);

	e.AddComponent<SpriteComponent>();
	EXPECT_EQ(e.HasComponent<SpriteComponent>(), true);
}

TEST_F(ECSTests, RemoveComponent)
{
	Entity e(UUID(), "TestEntity");
	e.AddComponent<SpriteComponent>();
	EXPECT_EQ(e.HasComponent<SpriteComponent>(), true);

	e.RemoveComponent<SpriteComponent>();
	EXPECT_EQ(e.HasComponent<SpriteComponent>(), false);
}

TEST_F(ECSTests, GetComponent)
{
	Entity e(UUID(), "TestEntity");
	SpriteComponent *comp = e.AddComponent<SpriteComponent>();
	comp->TilingFactor = 42.0f;
	EXPECT_EQ(e.HasComponent<SpriteComponent>(), true);

	SpriteComponent *otherComp = e.GetComponent<SpriteComponent>();
	EXPECT_EQ(otherComp->TilingFactor, 42.0f);
}

TEST_F(ECSTests, ViewTests)
{
	Ref<Scene> demoScene = Scene::CreateEmpty();
	Entity firstEntity = demoScene->CreateEntity("DemoName");
	Entity secondEntity = demoScene->CreateEntity("AnotherEntity");
	Entity thirdEntity = demoScene->CreateEntity("LOL");

	SpriteComponent *component = thirdEntity.AddComponent<SpriteComponent>();

	auto view = demoScene->GetRegistry().View<SpriteComponent>();
	EXPECT_EQ(thirdEntity.GetUUID(), view[0]);
}

