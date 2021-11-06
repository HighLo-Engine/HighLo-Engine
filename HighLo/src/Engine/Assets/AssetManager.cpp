// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "AssetManager.h"

#include <Engine/ImGui/imgui.h>

#include "Engine/Core/FileSystem.h"
#include "Engine/Core/FileSystemWatcher.h"
#include "Engine/Core/File.h"
#include "AssetExtensions.h"

#include "Engine/Scene/Project.h"

namespace highlo
{
	// ====================================================================================
	//                                   AssetRegistry
	// ====================================================================================

	static FileSystemPath GetKey(const FileSystemPath &path)
	{
		auto key = path.RelativePath(Project::GetAssetDirectory());
		if (key.String().IsEmpty())
			key = path.String();

		return key;
	}

	AssetMetaData &AssetRegistry::operator[](const FileSystemPath &path)
	{
		auto key = GetKey(path);
		HL_CORE_INFO("[AssetManager] Retrieving Asset '{0}' with key '{1}'", **path, **key);
		HL_ASSERT(m_AssetRegistry.find(key) != m_AssetRegistry.end());
		HL_ASSERT(!key.String().IsEmpty());
		return m_AssetRegistry[key];
	}
	
	const AssetMetaData &AssetRegistry::Get(const FileSystemPath &path) const
	{
		const auto key = GetKey(path);
		HL_CORE_INFO("[AssetManager] Retrieving Asset '{0}' with key '{1}'", **path, **key);
		HL_ASSERT(m_AssetRegistry.find(key) != m_AssetRegistry.end());
		HL_ASSERT(!key.String().IsEmpty());
		return m_AssetRegistry.at(key);
	}
	
	bool AssetRegistry::Contains(const FileSystemPath &path) const
	{
		auto key = GetKey(path);
		return m_AssetRegistry.find(key) != m_AssetRegistry.end();
	}
	
	size_t AssetRegistry::Remove(const FileSystemPath &path)
	{
		auto key = GetKey(path);
		HL_CORE_INFO("[AssetManager] Removing Asset '{0}'", **path);
		return m_AssetRegistry.erase(key);
	}
	
	void AssetRegistry::Clear()
	{
		HL_CORE_INFO("[AssetManager] Removing all assets");
		m_AssetRegistry.clear();
	}

	// ====================================================================================
	//                                   AssetManager
	// ====================================================================================

	static AssetMetaData s_NullMetaData;
	std::unordered_map<AssetHandle, Ref<Asset>> AssetManager::s_LoadedAssets;
	AssetManager::AssetsChangeEventFn AssetManager::s_AssetsChangeCallback;

	void AssetManager::Init()
	{
		LoadAssetRegistry();
		FileSystemWatcher::Get()->SetChangeCallback(AssetManager::OnFileSystemChangedEvent);
		ReloadAllAssets();
	}

	void AssetManager::Shutdown()
	{
		WriteRegistryToFile();

		s_AssetRegistry.Clear();
		s_LoadedAssets.clear();
	}

	void AssetManager::SetAssetChangeCallback(const AssetsChangeEventFn &callback)
	{
		s_AssetsChangeCallback = callback;
	}

	AssetMetaData &AssetManager::GetMetaData(AssetHandle handle)
	{
		for (auto &[path, metaData] : s_AssetRegistry)
		{
			if (metaData.Handle == handle)
				return metaData;
		}

		return s_NullMetaData;
	}

	AssetMetaData &AssetManager::GetMetaData(const FileSystemPath &path)
	{
		if (s_AssetRegistry.Contains(path.String()))
			return s_AssetRegistry[*path];

		return s_NullMetaData;
	}

	FileSystemPath AssetManager::GetFileSystemPath(const AssetMetaData &metaData)
	{
		return Project::GetAssetDirectory() / metaData.FilePath;
	}

	FileSystemPath AssetManager::GetRelativePath(const FileSystemPath &path)
	{
		HLString temp = path.String();
		if (temp.IndexOf(Project::GetActive()->GetAssetDirectory().String()) != HLString::NPOS)
			return path.RelativePath(Project::GetActive()->GetAssetDirectory());

		return path;
	}

	AssetHandle AssetManager::GetAssetHandleFromFilePath(const FileSystemPath &path)
	{
		return s_AssetRegistry.Contains(*path) ? s_AssetRegistry[*path].Handle : 0;
	}

	AssetType AssetManager::GetAssetTypeFromExtension(const HLString &extension)
	{
		if (s_AssetExtesions.find(extension.ToLowerCase()) == s_AssetExtesions.end())
			return AssetType::None;

		return s_AssetExtesions.at(extension.ToLowerCase());
	}

	AssetType AssetManager::GetAssetTypeFromPath(const FileSystemPath &path)
	{
		return GetAssetTypeFromExtension(path.GetFile()->GetExtension());
	}

	AssetHandle AssetManager::ImportAsset(const FileSystemPath &path)
	{
		// TODO
		return 0;
	}

	bool AssetManager::ReloadAsset(AssetHandle handle)
	{
		AssetMetaData &assetInfo = GetMetaData(handle);
		if (!assetInfo.IsDataLoaded)
		{
			HL_CORE_WARN("[AssetManager] Trying to load asset '{0}', but it was never loaded before!", *assetInfo.FilePath);

			Ref<Asset> asset;
			//assetInfo.IsDataLoaded = AssetImporter::TryLoadData(assetInfo, asset);
			return assetInfo.IsDataLoaded;
		}

		HL_ASSERT(s_LoadedAssets.find(handle) != s_LoadedAssets.end());
		Ref<Asset> &asset = s_LoadedAssets.at(handle);
		// TODO
		//assetInfo.IsDataLoaded = AssetImporter::TryLoadData(assetInfo, asset);
		return assetInfo.IsDataLoaded;
	}

	bool AssetManager::AssetExists(AssetMetaData &metaData)
	{
		// TODO: add root dir to the beginning of the path
		return FileSystem::Get()->FileExists(metaData.FilePath);
	}

	void AssetManager::LoadAssetRegistry()
	{
	}

	void AssetManager::WriteRegistryToFile()
	{
	}

	void AssetManager::ProcessDirectory(const HLString &dirPath)
	{
	}

	void AssetManager::ReloadAllAssets()
	{
		// TODO
	//	ProcessDirectory();
		WriteRegistryToFile();
	}

	void AssetManager::OnFileSystemChangedEvent(FileSystemChangedEvent &e)
	{
		HL_CORE_TRACE("OnFileSystemChangedEvent: {0}", e.GetName());
		// TODO: insert asset directory before this
		e.SetFilePath("" + e.GetPath());
		
	#ifdef HL_PLATFORM_WINDOWS
		HLString temp = e.GetPath();
		temp.Replace("\\", "/");
		e.SetFilePath(temp);
	#endif // HL_PLATFORM_WINDOWS

		// TOOD: Remove extension in NewName
		// e.SetNewName();

		if (!e.IsDirectory())
		{
			switch (e.GetAction())
			{
				case FileSystemAction::Added:
					AssetManager::Get()->ImportAsset(e.GetPath());
					break;

				case FileSystemAction::Deleted:
					AssetManager::Get()->OnAssetDeleted(AssetManager::Get()->GetAssetHandleFromFilePath(e.GetPath()));
					break;

				case FileSystemAction::Modified:
					// TODO: Reload Data if loaded
					break;

				case FileSystemAction::Renamed:
				{
					AssetType prevType = AssetManager::Get()->GetAssetTypeFromPath(e.GetOldName());
					AssetType type = AssetManager::Get()->GetAssetTypeFromPath(e.GetPath());

					if (prevType == AssetType::None && type != AssetType::None)
					{
						AssetManager::Get()->ImportAsset(e.GetPath());
					}
					else
					{
						// TODO: Get relative path
						OnAssetRenamed(AssetManager::Get()->GetAssetHandleFromFilePath(/* insert relative path here */ "/" + e.GetOldName()), e.GetPath());
						e.SetTracking(true);
					}
					break;
				}
			}
		}

		s_AssetsChangeCallback(e);
	}

	void AssetManager::OnAssetRenamed(AssetHandle handle, const FileSystemPath &newFilePath)
	{
		AssetMetaData assetInfo = AssetManager::Get()->GetMetaData(handle);
		
		s_AssetRegistry.Remove(*assetInfo.FilePath);
		assetInfo.FilePath = newFilePath;
		s_AssetRegistry[*assetInfo.FilePath] = assetInfo;
		
		AssetManager::Get()->WriteRegistryToFile();
	}

	void AssetManager::OnAssetMoved(AssetHandle handle, FileSystemPath &destinationFilePath)
	{
		AssetMetaData assetInfo = AssetManager::Get()->GetMetaData(handle);

		s_AssetRegistry.Remove(*assetInfo.FilePath);
		assetInfo.FilePath = destinationFilePath / assetInfo.FilePath;
		s_AssetRegistry[*assetInfo.FilePath] = assetInfo;

		AssetManager::Get()->WriteRegistryToFile();
	}

	void AssetManager::OnAssetDeleted(AssetHandle handle)
	{
		AssetMetaData assetInfo = AssetManager::Get()->GetMetaData(handle);

		s_AssetRegistry.Remove(*assetInfo.FilePath);
		s_LoadedAssets.erase(handle);

		AssetManager::Get()->WriteRegistryToFile();
	}

	void AssetManager::OnUIRender(bool &openui)
	{
		if (!openui)
			return;

		ImGui::Begin("Asset Manager", &openui);
		if (UI::BeginTreeNode("Registry"))
		{
			static char searchBuffer[256];
			static float assetManagerColumnWidth = 0.0f;
			
			ImGui::InputText("##assetSearch", searchBuffer, 256);

			UI::BeginPropertyGrid();

			if (assetManagerColumnWidth == 0.0f)
			{
				ImVec2 textSize = ImGui::CalcTextSize("File Path");
				assetManagerColumnWidth = textSize.x * 2.0f;
				ImGui::SetColumnWidth(0, assetManagerColumnWidth);
			}

			for (const auto &[path, assetInfo] : s_AssetRegistry)
			{
				HLString handleStr = fmt::format("{0}", assetInfo.Handle);
				HLString filePath = assetInfo.FilePath.String();
				HLString typeStr = utils::AssetTypeToString(assetInfo.Type);

				if (searchBuffer[0] != 0)
				{
					// User searched something in the search bar
					HLString searchString = searchBuffer;
					searchString = searchString.ToLowerCase();
					
					HLString handleStrLower = handleStr.ToLowerCase();
					HLString filePathLower = filePath.ToLowerCase();
					HLString typeStrLower = typeStr.ToLowerCase();

					if (handleStrLower.IndexOf(searchString) != HLString::NPOS
				 	  || filePathLower.IndexOf(searchString) != HLString::NPOS
				  	  || typeStrLower.IndexOf(searchString)  != HLString::NPOS)
					{
						UI::DrawInputText("Handle", handleStr);
						UI::DrawInputText("FilePath", filePath);
						UI::DrawInputText("Type", typeStr);
						UI::Separator();
					}
				}
				else
				{
					UI::DrawInputText("Handle", handleStr);
					UI::DrawInputText("File Path", filePath);
					UI::DrawInputText("Type", typeStr);
					UI::Separator();
				}
			}

			UI::EndPropertyGrid();
			UI::EndTreeNode();
		}

		ImGui::End();
	}
}

