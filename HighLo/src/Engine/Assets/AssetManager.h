// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.1 (2021-09-15) Changed FileExtension implementation
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Asset.h"
#include "Engine/Events/Events.h"

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

		HLAPI AssetMetaData &operator[](const HLString &path);
		HLAPI const AssetMetaData &Get(const HLString &path) const;

		HLAPI size_t Count() const { return m_AssetRegistry.size(); }
		HLAPI bool Contains(const HLString &path) const; 
		HLAPI size_t Remove(const HLString &path);
		HLAPI void Clear();

		HLAPI std::unordered_map<HLString, AssetMetaData>::iterator begin() { return m_AssetRegistry.begin(); }
		HLAPI std::unordered_map<HLString, AssetMetaData>::iterator end() { return m_AssetRegistry.end(); }
		HLAPI std::unordered_map<HLString, AssetMetaData>::const_iterator cbegin() { return m_AssetRegistry.begin(); }
		HLAPI std::unordered_map<HLString, AssetMetaData>::const_iterator cend() { return m_AssetRegistry.end(); }

	private:
		std::unordered_map<HLString, AssetMetaData> m_AssetRegistry;
	};

	class AssetManager
	{
	public:
		using AssetsChangeEventFn = std::function<void(FileSystemChangedEvent)>;

		// Should only be called by the engine
		HLAPI static void Init();
		HLAPI static void Shutdown();

		HLAPI static void SetAssetChangeCallback(const AssetsChangeEventFn &callback);

		HLAPI static AssetMetaData &GetMetaData(AssetHandle handle);
		HLAPI static AssetMetaData &GetMetaData(const HLString &path);
		HLAPI static AssetMetaData &GetMetaData(const Ref<Asset> &asset) { return GetMetaData(asset->Handle); }

		HLAPI static HLString GetFileSystemPath(const AssetMetaData &metaData);
		HLAPI static HLString GetRelativePath(const HLString &path);
			 
		HLAPI static AssetHandle GetAssetHandleFromFilePath(const HLString &path);
		HLAPI static bool IsAssetHandleValid(AssetHandle handle) { return GetMetaData(handle).IsValid(); }

		HLAPI static AssetType GetAssetTypeFromExtension(const HLString &extension);
		HLAPI static AssetType GetAssetTypeFromPath(const HLString &path);

		HLAPI static AssetHandle ImportAsset(const HLString &path);
		HLAPI static bool ReloadAsset(AssetHandle handle);

		template<typename T, typename... Args>
		HLAPI static Ref<T> CreateAsset(const HLString &fileName, const HLString &dirPath, Args&&... args)
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
					HLString nextFilePath = dirPath + "/" + assetInfo.FilePath.Stem().String();
					if (current < 10)
						nextFilePath += " (0" + HLString::ToString(current) + ")";
					else
						nextFilePath += " (" + HLString::ToString(current) + ")";

					nextFilePath += assetInfo.FilePath.Extension().ToString();

					if (!FileSystem::FileExists(nextFilePath))
					{
						foundAvailableFileName = true;
						assetInfo.FilePath = nextFilePath;
						break;
					}

					++current;
				}
			}

			s_AssetRegistry[assetInfo.FilePath] = assetInfo;
			WriteRegistryToFile();

			Ref<Asset> asset = Ref<T>::Create(std::forward<Args>(args)...);
			asset->Handle = assetInfo.Handle;
			s_LoadedAssets[asset->Handle] = asset;
			// AssetImporter::Serialize(assetInfo, asset);
			return asset;
		}

		template<typename T>
		HLAPI static Ref<T> GetAsset(AssetHandle handle)
		{
			auto &assetInfo = GetMetaData(handle);

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
		HLAPI static Ref<T> GetAsset(const HLString &path)
		{
			return GetAsset<T>(GetAssetHandleFromFilePath(path));
		}

		HLAPI static bool AssetExists(AssetMetaData &metaData);
		HLAPI static void OnUIRender(bool &openui);

	private:

		static void LoadAssetRegistry();
		static void WriteRegistryToFile();

		static void ProcessDirectory(const HLString &dirPath);
		static void ReloadAllAssets();
		
		static void OnFileSystemChangedEvent(FileSystemChangedEvent &e);
		static void OnAssetRenamed(AssetHandle handle, const HLString &newFilePath);
		static void OnAssetMoved(AssetHandle handle, const HLString &destinationFilePath);
		static void OnAssetDeleted(AssetHandle handle);

	private:

		static std::unordered_map<AssetHandle, Ref<Asset>> s_LoadedAssets;
		static AssetsChangeEventFn s_AssetsChangeCallback;
		inline static AssetRegistry s_AssetRegistry;
	};
}

