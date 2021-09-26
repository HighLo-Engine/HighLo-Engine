// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-26) initial release
//

#pragma once

#include "AssetEditor.h"

namespace highlo
{
	class AssetEditorPanel
	{
	public:

		HLAPI static void Init();
		HLAPI static void Shutdown();

		HLAPI static void OnUpdate(Timestep ts);
		HLAPI static void OnUIRender(Timestep ts);
		HLAPI static void OnEvent(Event &e);

		HLAPI static void OpenEditor(const Ref<Asset> &asset);

		template<typename T>
		HLAPI static void RegisterEditor(AssetType type)
		{
			static_assert(std::is_base_of_v<AssetEditor, T>, "template type must be inherited from AssetEditor");
			HL_ASSERT(s_Editors.find(type) == s_Editors.end(), "EditorType already exists");

			s_Editors[type] = MakeUniqueRef<T>();
		}

	private:

		static std::unordered_map<AssetType, UniqueRef<AssetEditor>> s_Editors;
	};
}