// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-11-03) initial release
//

#pragma once

#include "AssetBrowser/AssetBrowserItem.h"
#include "AssetBrowser/AssetBrowserItemList.h"
#include "AssetBrowser/SelectionStack.h"

#include "Engine/Scene/Project.h"
#include "Engine/Events/Events.h"

// From Windows Header
#undef RemoveDirectory

namespace highlo
{
	struct AssetBrowserSettings
	{
		float Padding = 2.0f;
		float ThumbnailSize = 128.0f;
		bool ShowAssetType = false;
	};

	class AssetBrowserPanel
	{
	public:

		HLAPI AssetBrowserPanel(Ref<Project> project);
		HLAPI virtual ~AssetBrowserPanel();

		HLAPI static AssetBrowserPanel &Get() { return *s_Instance; }

		HLAPI void OnUIRender();
		HLAPI void OnEvent(Event &e);

		HLAPI const SelectionStack &GetSelectionStack() const { return m_SelectionStack; }
		HLAPI AssetBrowserItemList &GetCurrentItems() { return m_CurrentItems; }

		HLAPI Ref<DirectoryInfo> GetDirectory(const FileSystemPath &filePath) const;
		HLAPI static AssetBrowserSettings &GetSettings();
		HLAPI static void RestoreDefaultSettings();

	private:

		AssetHandle ProcessDirectory(const FileSystemPath &dirPath, const Ref<DirectoryInfo> &parentDirInfo);
		void ChangeDirectory(Ref<DirectoryInfo> &dirInfo);
		void RemoveDirectory(Ref<DirectoryInfo> &dirInfo, bool removeFromParent = true);

		void RenderDirectoryHierarchy(Ref<DirectoryInfo> &dirInfo);
		void RenderTopBar(float height, float edgeOffset = 4.0f);
		void RenderItems();
		void RenderDeleteDialogue();

		void Refresh();
		void UpdateInput();
		
		void PasteCopiedAssets();
		void ClearSelections();

		void UpdateDropArea(const Ref<DirectoryInfo> &target);
		void SortItemList();
		void SortSubDirectories();
		AssetBrowserItemList Search(const HLString &query, const Ref<DirectoryInfo> &dirInfo);

		bool OnFileSystemChangedEvent(FileSystemChangedEvent &e);
		bool OnKeyPressedEvent(KeyPressedEvent &e);

	private:

		template<typename AssetType, typename... Args>
		Ref<AssetType> CreateAssetRepresentation(const HLString &filePath, Args&& ...args)
		{
			FileSystemWatcher::Get()->DisableWatchUntilNextAction();

			Ref<AssetType> asset = AssetManager::Get()->CreateAsset<AssetType>(filePath, m_CurrentDirectory->FilePath.String(), std::forward<Args>(args)...);
			if (!asset)
				return nullptr;

			m_CurrentDirectory->Assets.push_back(asset->Handle);
			ChangeDirectory(m_CurrentDirectory);

			auto &item = m_CurrentItems[m_CurrentItems.Find(asset->Handle)];
			m_SelectionStack.Select(asset->Handle);
			item->SetSelected();
			item->StartRenaming();

			return asset;
		}

	private:

		static AssetBrowserPanel *s_Instance;

		Ref<Project> m_Project;
		HLString m_BasePath;

		Ref<Texture2D> m_FileIcon;
		Ref<Texture2D> m_FolderIcon;
		Ref<Texture2D> m_Shadow;

		std::map<HLString, Ref<Texture2D>> m_AssetIconMap;
		std::unordered_map<AssetHandle, Ref<DirectoryInfo>> m_Directories;
		std::vector<Ref<DirectoryInfo>> m_BreadCrumbData;

		AssetBrowserItemList m_CurrentItems;
		SelectionStack m_SelectionStack, m_CopiedAssets;

		Ref<DirectoryInfo> m_CurrentDirectory;
		Ref<DirectoryInfo> m_BaseDirectory;
		Ref<DirectoryInfo> m_NextDirectory, m_PrevDirectory;

		bool m_IsAnyItemHovered = false;
		bool m_UpdateNavigationPath = false;
		bool m_IsAssetBrowserHovered = false;
		bool m_IsAssetBrowserFocused = false;
		char m_SearchBuffer[HL_MAX_INPUT_BUFFER_LENGTH];
	};
}

