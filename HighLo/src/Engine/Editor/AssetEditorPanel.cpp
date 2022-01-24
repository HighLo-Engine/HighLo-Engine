// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "AssetEditorPanel.h"

#include "Engine/Assets/AssetManager.h"

namespace highlo
{
	std::unordered_map<AssetType, UniqueRef<AssetEditor>> AssetEditorPanel::s_Editors;

	void AssetEditorPanel::Init()
	{
		// TODO: Register default editors
	}
	
	void AssetEditorPanel::Shutdown()
	{
		s_Editors.clear();
	}
	
	void AssetEditorPanel::OnUpdate(Timestep ts)
	{
		for (auto &kv : s_Editors)
			kv.second->OnUpdate(ts);
	}
	
	void AssetEditorPanel::OnUIRender(Timestep ts)
	{
		for (auto &kv : s_Editors)
			kv.second->OnUIRender(ts);
	}
	
	void AssetEditorPanel::OnEvent(Event &e)
	{
		for (auto &kv : s_Editors)
			kv.second->OnEvent(e);
	}
	
	void AssetEditorPanel::OpenEditor(const Ref<Asset> &asset)
	{
		if (!asset)
			return;

		if (s_Editors.find(asset->GetAssetType()) == s_Editors.end())
			return;

		s_Editors[asset->GetAssetType()]->SetOpen(true);
		s_Editors[asset->GetAssetType()]->SetAsset(AssetManager::Get()->GetAsset<Asset>(asset->Handle));
	}
}

