// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.4 (2022-01-21) Added missing TODOs
//     - 1.3 (2021-10-04) Refactored initialization to use the FileSystemWatcher instead of FileSystem class
//     - 1.2 (2021-09-22) Changed AssetManager to be a Singleton class
//     - 1.1 (2021-09-15) Changed FileExtension implementation
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Asset.h"
#include "AssetRegistry.h"

#include "Engine/Core/FileSystemPath.h"
#include "Engine/Events/Events.h"
#include "Engine/Loaders/MeshLoader.h"
#include "Engine/Loaders/AssetImporter.h"

namespace highlo
{
	struct AssetManagerConfig
	{
		HLString AssetDirectory = "assets/";
		HLString AssetRegistryPath = "assets/AssetRegistry.hl";

		HLString MeshPath = "assets/models/";
		HLString TexturePath = "assets/textures/";
		HLString ShaderPath = "assets/shaders/";
		HLString ScriptPath = "assets/scripts/";
	};

	class AssetManager : public Singleton<AssetManager>
	{
	public:
		
		// Should only be called by the engine
		HLAPI void Init();
		HLAPI void Shutdown();

		HLAPI void OnEvent(Event &e);

		HLAPI AssetMetaData &GetMetaData(AssetHandle handle);
		HLAPI AssetMetaData &GetMetaData(const FileSystemPath &path);
		HLAPI AssetMetaData &GetMetaData(const Ref<Asset> &asset) { return GetMetaData(asset->Handle); }

		HLAPI FileSystemPath GetFileSystemPath(const AssetMetaData &metaData);
		HLAPI FileSystemPath GetRelativePath(const FileSystemPath &path);
			 
		HLAPI AssetHandle GetAssetHandleFromFilePath(const FileSystemPath &path);
		HLAPI bool IsAssetHandleValid(AssetHandle handle) { return GetMetaData(handle).IsValid(); }

		HLAPI AssetType GetAssetTypeFromExtension(const HLString &extension);
		HLAPI AssetType GetAssetTypeFromPath(const FileSystemPath &path);

		HLAPI AssetHandle ImportAsset(const FileSystemPath &path);
		HLAPI bool ReloadAsset(AssetHandle handle);

		template<typename T, typename... Args>
		HLAPI AssetHandle CreateMemoryOnlyAsset(Args&& ...args)
		{
			static_assert(std::is_base_of<Asset, T>::value, "CreateMemoryOnlyAsset only works for types derived from Asset");

			Ref<T> asset = Ref<T>::Create(std::forward<Args>(args)...);
			asset->Handle = AssetHandle(); // New UUID

			s_MemoryAssets[asset->Handle] = asset;
			return asset->Handle;
		}

		HLAPI bool IsMemoryAsset(AssetHandle handle)
		{
			return s_MemoryAssets.find(handle) != s_MemoryAssets.end();
		}

		template<typename T, typename... Args>
		HLAPI Ref<T> CreateAsset(const HLString &fileName, const HLString &dirPath, Args&&... args)
		{
			static_assert(std::is_base_of<Asset, T>::value, "Error: Did you forget to derive your class from Asset?");

			AssetMetaData assetInfo;
			assetInfo.Handle = AssetHandle(); // New UUID
			if (dirPath.IsEmpty() || dirPath == "." || dirPath == "..")
			{
				assetInfo.FilePath = fileName;
			}
			else
			{
				assetInfo.FilePath = AssetManager::GetRelativePath(dirPath + "/" + fileName);
			}

			assetInfo.IsDataLoaded = true;
			assetInfo.Type = T::GetStaticType();

			if (AssetExists(assetInfo))
			{
				bool foundAvailableFileName = false;
				int32 current = 1;

				while (!foundAvailableFileName)
				{
					HLString nextFilePath = dirPath + "/" + assetInfo.FilePath.Filename();
					if (current < 10)
						nextFilePath += " (0" + HLString::ToString(current) + ")";
					else
						nextFilePath += " (" + HLString::ToString(current) + ")";

					nextFilePath += assetInfo.FilePath.Extension();

					if (!FileSystem::Get()->FileExists(nextFilePath))
					{
						foundAvailableFileName = true;
						assetInfo.FilePath = nextFilePath;
						break;
					}

					++current;
				}
			}

			s_AssetRegistry[assetInfo.FilePath.Absolute()] = assetInfo;
			WriteRegistryToFile();

			Ref<Asset> asset = Ref<T>::Create(std::forward<Args>(args)...);
			asset->Handle = assetInfo.Handle;
			s_LoadedAssets[asset->Handle] = asset;
			AssetImporter::Serialize(assetInfo, asset);

			return asset;
		}

		template<typename T>
		HLAPI Ref<T> GetAsset(AssetHandle handle)
		{
			if (IsMemoryAsset(handle))
				return s_MemoryAssets[handle].As<T>();

			auto &assetInfo = GetMetaData(handle);
			if (!assetInfo.IsValid())
				return nullptr;

			Ref<Asset> asset = nullptr;
			if (!assetInfo.IsDataLoaded)
			{
				assetInfo.IsDataLoaded = AssetImporter::TryLoadData(assetInfo, asset);
				if (!assetInfo.IsDataLoaded)
					return nullptr;

				s_LoadedAssets[handle] = asset;
			}
			else
			{
				asset = s_LoadedAssets[handle];
			}

			return asset.As<T>();
		}

		template<typename T>
		HLAPI Ref<T> GetAsset(const FileSystemPath &path)
		{
			return GetAsset<T>(GetAssetHandleFromFilePath(path));
		}

		HLAPI bool AssetExists(AssetMetaData &metaData);
		HLAPI void OnUIRender(bool &openui);

	private:

		void LoadAssetRegistry();
		void WriteRegistryToFile();

		void ProcessDirectory(const FileSystemPath &dirPath);
		void ReloadAllAssets();
		
		bool OnFileSystemChangedEvent(FileSystemChangedEvent &e);
		static void OnAssetRenamed(AssetHandle handle, const FileSystemPath &newFilePath);
		static void OnAssetMoved(AssetHandle handle, FileSystemPath &destinationFilePath);
		static void OnAssetDeleted(AssetHandle handle);

	private:

		static std::unordered_map<AssetHandle, Ref<Asset>> s_LoadedAssets;
		static std::unordered_map<AssetHandle, Ref<Asset>> s_MemoryAssets;
		inline static AssetRegistry s_AssetRegistry;

	private:

		friend class AssetBrowserBaseItem;
		friend class AssetBrowserItem;
		friend class AssetBrowserDirectory;
	};
}

