// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "AssetManager.h"

#include <Engine/ImGui/imgui.h>

#include "Engine/Core/FileSystem.h"
#include "Engine/Core/FileSystemWatcher.h"
#include "AssetExtensions.h"

#include "Engine/Scene/Project.h"
#include "Engine/Loaders/DocumentWriter.h"
#include "Engine/Loaders/DocumentReader.h"

#define ASSET_MANAGER_LOG_PREFIX "AManager>     "

namespace highlo
{
	static AssetMetaData s_NullMetaData;
	std::unordered_map<AssetHandle, Ref<Asset>> AssetManager::s_LoadedAssets;
	std::unordered_map<AssetHandle, Ref<Asset>> AssetManager::s_MemoryAssets;

	struct AssetRegistryEntry
	{
		FileSystemPath FilePath;
		AssetType Type;
	};

	void AssetManager::Init()
	{
		LoadAssetRegistry();
		ReloadAllAssets();
	}

	void AssetManager::Shutdown()
	{
		WriteRegistryToFile();

		s_AssetRegistry.Clear();
		s_LoadedAssets.clear();
		s_MemoryAssets.clear();
	}

	void AssetManager::OnEvent(Event &e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<FileSystemChangedEvent>(HL_BIND_EVENT_FUNCTION(AssetManager::OnFileSystemChangedEvent));
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
		if (s_AssetRegistry.Contains(path))
			return s_AssetRegistry[path];

		return s_NullMetaData;
	}

	FileSystemPath AssetManager::GetFileSystemPath(const AssetMetaData &metaData)
	{
		return metaData.FilePath;
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

	AssetType AssetManager::GetAssetTypeFromExtension(HLString &extension)
	{
		if (s_AssetExtesions.find(extension.ToLowerCase()) == s_AssetExtesions.end())
			return AssetType::None;

		return s_AssetExtesions.at(extension.ToLowerCase());
	}

	AssetType AssetManager::GetAssetTypeFromPath(const FileSystemPath &path)
	{
		HLString extension = path.Extension();
		return GetAssetTypeFromExtension(extension);
	}

	AssetHandle AssetManager::ImportAsset(const FileSystemPath &path)
	{
		FileSystemPath relativePath = path.RelativePath();

		if (s_AssetRegistry.Contains(relativePath))
			return s_AssetRegistry[relativePath].Handle;

		AssetType type = GetAssetTypeFromPath(relativePath);
		if (type == AssetType::None)
			return 0;

		AssetMetaData assetInfo;
		assetInfo.Handle = AssetHandle(); // New UUID
		assetInfo.FilePath = relativePath;
		assetInfo.Type = type;
		s_AssetRegistry[assetInfo.FilePath] = assetInfo;

		return assetInfo.Handle;
	}

	bool AssetManager::ReloadAsset(AssetHandle handle)
	{
		AssetMetaData &assetInfo = GetMetaData(handle);
		if (!assetInfo.IsDataLoaded)
		{
			HL_CORE_WARN(ASSET_MANAGER_LOG_PREFIX "[-] Trying to load asset '{0}', but it was never loaded before! [-]", *assetInfo.FilePath);

			Ref<Asset> asset;
			assetInfo.IsDataLoaded = AssetImporter::TryLoadData(assetInfo, asset);
			return assetInfo.IsDataLoaded;
		}

		HL_ASSERT(s_LoadedAssets.find(handle) != s_LoadedAssets.end());
		Ref<Asset> &asset = s_LoadedAssets.at(handle);
		assetInfo.IsDataLoaded = AssetImporter::TryLoadData(assetInfo, asset);
		return assetInfo.IsDataLoaded;
	}

	bool AssetManager::AssetExists(AssetMetaData &metaData)
	{
		return FileSystem::Get()->FileExists(Project::GetActive()->GetAssetDirectory() / metaData.FilePath);
	}

	void AssetManager::LoadAssetRegistry()
	{
		FileSystemPath assetRegistryPath = Project::GetAssetRegistryPath();
		if (!FileSystem::Get()->FileExists(assetRegistryPath))
			return;

		// TODO
	}

	void AssetManager::WriteRegistryToFile()
	{
		FileSystemPath assetRegistryPath = Project::GetAssetRegistryPath();
		Ref<DocumentWriter> writer = DocumentWriter::Create(assetRegistryPath, DocumentType::Json);

		std::map<UUID, AssetRegistryEntry> sortedAssets;
		for (auto &[filePath, assetInfo] : s_AssetRegistry)
		{
			if (!FileSystem::Get()->FileExists(GetFileSystemPath(assetInfo)))
				continue;

			FileSystemPath pathToSerialize = assetInfo.FilePath;
			HLString pathStr = pathToSerialize.String();

			if (pathStr.Contains("\\"))
			{
				pathStr = pathStr.Replace("\\", "/");
			}

			sortedAssets[assetInfo.Handle] = { FileSystemPath(pathStr), assetInfo.Type };
		}

		HL_CORE_INFO(ASSET_MANAGER_LOG_PREFIX "[+] Serializing asset registry with {0} entries [+]", sortedAssets.size());

		writer->BeginArray();
		for (auto &[handle, entry] : sortedAssets)
		{
			writer->BeginObject();
			writer->WriteUInt64("Handle", (uint64)handle);
			writer->WriteString("FilePath", entry.FilePath.String());
			writer->WriteString("AssetType", utils::AssetTypeToString(entry.Type));
			writer->EndObject();
		}
		writer->EndArray();

		writer->WriteOut();
	}

	void AssetManager::ProcessDirectory(const FileSystemPath &dirPath)
	{
		std::vector<File> fileList = dirPath.GetFileList();
		for (uint32 i = 0; i < fileList.size(); ++i)
		{
			File entry = fileList[i];
			if (!entry.IsFile)
			{
			//	HL_CORE_TRACE(ASSET_MANAGER_LOG_PREFIX "[=] Discovering path {0} [=]", *entry.Name);
				ProcessDirectory(entry.FullPath);
			}
			else
			{
			//	HL_CORE_TRACE(ASSET_MANAGER_LOG_PREFIX "[=] Importing asset {0} [=]", *entry.Name);
				ImportAsset(entry.FullPath);
			}
		}
	}

	void AssetManager::ReloadAllAssets()
	{
		ProcessDirectory(Project::GetAssetDirectory());
		WriteRegistryToFile();
	}

	bool AssetManager::OnFileSystemChangedEvent(FileSystemChangedEvent &e)
	{
		HL_CORE_TRACE(ASSET_MANAGER_LOG_PREFIX "[+] OnFileSystemChangedEvent: {0} [+]", e.GetName());

		if (!e.IsDirectory)
		{
			switch (e.Action)
			{
				case FileSystemAction::Added:
					AssetManager::Get()->ImportAsset(e.FilePath);
					break;

				case FileSystemAction::Deleted:
					AssetManager::Get()->OnAssetDeleted(AssetManager::Get()->GetAssetHandleFromFilePath(e.FilePath));
					break;

				case FileSystemAction::Modified:
				{
					AssetHandle handle = GetAssetHandleFromFilePath(e.FilePath);
					const auto &assetInfo = GetMetaData(handle);

					if (assetInfo.IsValid() && assetInfo.IsDataLoaded)
						ReloadAsset(handle);

					break;
				}

				case FileSystemAction::Renamed:
				{
					AssetType prevType = AssetManager::Get()->GetAssetTypeFromPath(e.OldName);
					AssetType type = AssetManager::Get()->GetAssetTypeFromPath(e.FilePath);

					if (prevType == AssetType::None && type != AssetType::None)
					{
						AssetManager::Get()->ImportAsset(e.FilePath);
					}
					else
					{
						OnAssetRenamed(AssetManager::Get()->GetAssetHandleFromFilePath(e.FilePath.ParentPath() / e.OldName), e.FilePath);
						e.Tracking = true;
					}
					break;
				}
			}
		}

		return true;
	}

	void AssetManager::OnAssetRenamed(AssetHandle handle, const FileSystemPath &newFilePath)
	{
		AssetMetaData assetInfo = AssetManager::Get()->GetMetaData(handle);
		if (!assetInfo.IsValid())
			return;

		s_AssetRegistry.Remove(assetInfo.FilePath);
		assetInfo.FilePath = s_AssetRegistry.GetKey(newFilePath);
		s_AssetRegistry[assetInfo.FilePath] = assetInfo;
		
		AssetManager::Get()->WriteRegistryToFile();
	}

	void AssetManager::OnAssetMoved(AssetHandle handle, FileSystemPath &destinationFilePath)
	{
		AssetMetaData assetInfo = AssetManager::Get()->GetMetaData(handle);
		if (!assetInfo.IsValid())
			return;

		s_AssetRegistry.Remove(assetInfo.FilePath);
		assetInfo.FilePath = destinationFilePath / assetInfo.FilePath;
		s_AssetRegistry[assetInfo.FilePath] = assetInfo;

		AssetManager::Get()->WriteRegistryToFile();
	}

	void AssetManager::OnAssetDeleted(AssetHandle handle)
	{
		AssetMetaData assetInfo = AssetManager::Get()->GetMetaData(handle);
		if (!assetInfo.IsValid())
			return;

		s_AssetRegistry.Remove(assetInfo.FilePath);
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
			
			ImGui::Text("Search for any asset: ");
			ImGui::SameLine();
			ImGui::InputText("##assetSearch", searchBuffer, 256);

			UI::BeginPropertyGrid(2);

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

