

#pragma once

#include "Engine/Assets/Asset.h"
#include "Engine/ECS/Entity.h"
#include "Engine/Scene/Scene.h"

namespace highlo
{
	class Prefab : public Asset
	{
	public:

		HLAPI Prefab();
		HLAPI Prefab(Entity e);
		HLAPI ~Prefab();

		HLAPI void Create(Entity e, bool serialize = true);

		HLAPI static AssetType GetStaticType() { return AssetType::Prefab; }
		HLAPI virtual AssetType GetAssetType() const override { return GetStaticType(); }

	private:

		Entity CreatePrefabFromEntity(Entity entity);

		Ref<Scene> m_Scene;
		Entity m_Entity;
	};
}
