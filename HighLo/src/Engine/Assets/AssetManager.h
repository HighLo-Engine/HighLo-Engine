// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.3 (2021-10-04) Refactored initialization to use the FileSystemWatcher instead of FileSystem class
//     - 1.2 (2021-09-22) Changed AssetManager to be a Singleton class
//     - 1.1 (2021-09-15) Changed FileExtension implementation
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Asset.h"
#include "Engine/Core/FileSystemPath.h"
#include "Engine/Events/Events.h"
#include "Engine/Loaders/AssetLoader.h"

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

	class AssetRegistry
	{
	public:

		HLAPI AssetMetaData &operator[](const FileSystemPath &path);
		HLAPI const AssetMetaData &Get(const FileSystemPath &path) const;

		HLAPI size_t Count() const { return m_AssetRegistry.size(); }
		HLAPI bool Contains(const FileSystemPath &path) const; 
		HLAPI size_t Remove(const FileSystemPath &path);
		HLAPI void Clear();

		HLAPI std::unordered_map<FileSystemPath, AssetMetaData>::iterator begin() { return m_AssetRegistry.begin(); }
		HLAPI std::unordered_map<FileSystemPath, AssetMetaData>::iterator end() { return m_AssetRegistry.end(); }
		HLAPI std::unordered_map<FileSystemPath, AssetMetaData>::const_iterator cbegin() { return m_AssetRegistry.begin(); }
		HLAPI std::unordered_map<FileSystemPath, AssetMetaData>::const_iterator cend() { return m_AssetRegistry.end(); }

	private:
		std::unordered_map<FileSystemPath, AssetMetaData> m_AssetRegistry;
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
		HLAPI Ref<T> CreateAsset(const HLString &fileName, const HLString &dirPath, Args&&... args)
		{
			static_assert(std::is_base_of<Asset, T>::value, "Error: Did you forget to derive your class from Asset?");

			AssetMetaData assetInfo;
			assetInfo.Handle = AssetHandle(); // Create new random assetID
			if (dirPath.IsEmpty() || dirPath == ".")
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
			// AssetImporter::Serialize(assetInfo, asset);
			return asset;
		}

		template<typename T>
		HLAPI Ref<T> GetAsset(AssetHandle handle)
		{
			auto &assetInfo = GetMetaData(handle);

			Ref<Asset> asset = nullptr;
			if (!assetInfo.IsDataLoaded)
			{
				// TODO
				/*
				assetInfo.IsDataLoaded = AssetImporter::TryLoadData(assetInfo, asset);
				if (!assetInfo.IsDataLoaded)
					return nullptr;

				s_LoadedAssets[handle] = asset;
				*/
			}
			else
			{
				asset = s_LoadedAssets[handle];
			}

			return asset.As<T>();
		}

		template<typename T>
		HLAPI Ref<T> GetAsset(const HLString &path)
		{
			return GetAsset<T>(GetAssetHandleFromFilePath(path));
		}

		HLAPI bool AssetExists(AssetMetaData &metaData);
		HLAPI void OnUIRender(bool &openui);

	private:

		void LoadAssetRegistry();
		void WriteRegistryToFile();

		void ProcessDirectory(const HLString &dirPath);
		void ReloadAllAssets();
		
		bool OnFileSystemChangedEvent(FileSystemChangedEvent &e);
		static void OnAssetRenamed(AssetHandle handle, const FileSystemPath &newFilePath);
		static void OnAssetMoved(AssetHandle handle, FileSystemPath &destinationFilePath);
		static void OnAssetDeleted(AssetHandle handle);

	private:

		static std::unordered_map<AssetHandle, Ref<Asset>> s_LoadedAssets;
		inline static AssetRegistry s_AssetRegistry;

	private:

		friend class AssetBrowserBaseItem;
		friend class AssetBrowserItem;
		friend class AssetBrowserDirectory;
	};
}

