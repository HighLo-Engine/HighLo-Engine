// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

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
		HLAPI SceneHierarchyPanel(const Ref<Scene> &scene, bool isWindow, SelectionContext context);

		HLAPI void SetContext(const Ref<Scene> &scene);
		HLAPI void SetEntityDeletedCallback(const std::function<void(Entity)> &callback) { m_EntityDeletedCallback = callback; }
		HLAPI void SetInvalidAssetMetaDataCallback(const std::function<void(Entity, AssetHandle)> &callback) { m_InvalidAssetMetaDataCallback = callback; }

		HLAPI void OnUIRender(bool *pOpen = nullptr);

	private:

		void OnExternalEntityDestroyed(Entity entity);
		void DrawEntityCreateMenu(Entity parent);
		void DrawEntityNode(Entity entity, const HLString &searchFilter);

	private:

		Ref<Scene> m_Scene;
		bool m_IsWindow = false;
		bool m_IsWindowFocused = false;

		std::function<void(Entity)> m_EntityDeletedCallback;
		std::function<void(Entity, AssetHandle)> m_InvalidAssetMetaDataCallback;

		Ref<Texture2D> m_TreeNodeRoot = nullptr;
		Ref<Texture2D> m_TreeNodeChildren = nullptr;
		Ref<Texture2D> m_TreeNodeChildrenOpened = nullptr;

		SelectionContext m_SelectionContext;
		UUID m_SelectedEntityID;
		static SelectionContext s_ActiveSelectionContext;

		int32 m_FirstSelectedRow = -1;
		int32 m_LastSelectedRow = -1;
	};
}
