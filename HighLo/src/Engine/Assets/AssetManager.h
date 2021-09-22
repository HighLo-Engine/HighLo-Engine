// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.2 (2021-09-22) Changed AssetManager to be a Singleton class
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

	class AssetManager : public Singleton<AssetManager>
	{
	public:
		using AssetsChangeEventFn = std::function<void(FileSystemChangedEvent)>;

		// Should only be called by the engine
		HLAPI void Init();
		HLAPI void Shutdown();

		HLAPI void SetAssetChangeCallback(const AssetsChangeEventFn &callback);

		HLAPI AssetMetaData &GetMetaData(AssetHandle handle);
		HLAPI AssetMetaData &GetMetaData(const HLString &path);
		HLAPI AssetMetaData &GetMetaData(const Ref<Asset> &asset) { return GetMetaData(asset->Handle); }

		HLAPI HLString GetFileSystemPath(const AssetMetaData &metaData);
		HLAPI HLString GetRelativePath(const HLString &path);
			 
		HLAPI AssetHandle GetAssetHandleFromFilePath(const HLString &path);
		HLAPI bool IsAssetHandleValid(AssetHandle handle) { return GetMetaData(handle).IsValid(); }

		HLAPI AssetType GetAssetTypeFromExtension(const HLString &extension);
		HLAPI AssetType GetAssetTypeFromPath(const HLString &path);

		HLAPI AssetHandle ImportAsset(const HLString &path);
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
		HLAPI Ref<T> GetAsset(AssetHandle handle)
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

