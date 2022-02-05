// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Scene/Scene.h"
#include "Engine/Assets/Asset.h"

namespace highlo
{
	class SceneHierarchyPanel
	{
	public:

		HLAPI SceneHierarchyPanel() = default;
		HLAPI SceneHierarchyPanel(const Ref<Scene> &scene, bool isWindow);

		HLAPI void SetContext(const Ref<Scene> &scene);
		HLAPI void SetSelected(Entity entity);
		HLAPI void SetSelectionChangedCallback(const std::function<void(Entity)> &callback) { m_SelectionChangedCallback = callback; }
		HLAPI void SetEntityDeletedCallback(const std::function<void(Entity)> &callback) { m_EntityDeletedCallback = callback; }
		HLAPI void SetEntityAddedCallback(const std::function<void(Entity)> &callback) { m_EntityAddedCallback = callback; }
		HLAPI void SetInvalidAssetMetaDataCallback(const std::function<void(Entity, AssetHandle)> &callback) { m_InvalidAssetMetaDataCallback = callback; }

		HLAPI void OnUIRender(bool *pOpen = nullptr);

	private:

		void DrawEntityNode(Entity entity, const HLString &searchFilter = "");
		void DeleteEntity(Entity entity);

		Ref<Scene> m_Scene;
		Entity m_SelectedEntity;
		bool m_IsWindow = false;

		std::function<void(Entity)> m_SelectionChangedCallback, m_EntityDeletedCallback, m_EntityAddedCallback;
		std::function<void(Entity, AssetHandle)> m_InvalidAssetMetaDataCallback;

		Ref<Texture2D> m_TreeNodeRoot;
		Ref<Texture2D> m_TreeNodeChildren;
		Ref<Texture2D> m_TreeNodeChildrenOpened;
	};
}