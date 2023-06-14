// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-01-31) initial release
//

#pragma once

#include "Engine/Scene/Scene.h"

namespace highlo
{
	enum class ObjectPropertiesActionType
	{
		None = 0,
		TagChanged,
		TransformChanged,
		ComponentChanged,
	};

	class ObjectPropertiesPanel
	{
	public:

		HLAPI ObjectPropertiesPanel() = default;
		HLAPI ObjectPropertiesPanel(const Ref<Scene> &scene, bool isWindow);
		HLAPI virtual ~ObjectPropertiesPanel();

		HLAPI void SetContext(const Ref<Scene> &scene);
		HLAPI void SetSelected(Entity entity);

		HLAPI void OnUIRender(bool *pOpen = nullptr);

		HLAPI void AddActionCallback(const std::function<void(ObjectPropertiesActionType)> &callback);
		HLAPI void SetSelectionChangedCallback(const std::function<void(Entity)> &callback) { m_SelectionChangedCallback = callback; }

	private:

		void DrawComponents(Entity &entity);

		Ref<Scene> m_Scene;
		bool m_IsWindow = false;
		Entity m_SelectedEntity;

		std::function<void(ObjectPropertiesActionType)> m_ActionCallback;
		std::function<void(Entity)> m_SelectionChangedCallback;

		Ref<Texture2D> m_PlusIcon;
		Ref<Texture2D> m_SettingsIcon;
	};
}

