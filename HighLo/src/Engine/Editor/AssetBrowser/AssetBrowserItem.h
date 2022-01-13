// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-11-03) initial release
//

#pragma once

#include "Engine/Renderer/Texture.h"

namespace highlo
{
#define HL_MAX_INPUT_BUFFER_LENGTH 512

	enum class AssetBrowserAction
	{
		None = 0,
		Refresh				= HL_BIT(0),
		ClearSelections		= HL_BIT(1),
		Selected			= HL_BIT(2),
		Hovered				= HL_BIT(3),
		Renamed				= HL_BIT(4),
		NavigateToThis		= HL_BIT(5),
		OpenDeleteDialogue	= HL_BIT(6),
		SelectThis			= HL_BIT(7),
		Moved				= HL_BIT(8),
		ShowInExplorer		= HL_BIT(9),
		OpenExternal		= HL_BIT(10),
		Reload				= HL_BIT(11),
		Copy				= HL_BIT(12),
		Duplicate			= HL_BIT(13),
		StartRenaming		= HL_BIT(14)

	};

	struct AssetBrowserActionResult
	{
		uint16 Field = 0;

		HLAPI void Set(AssetBrowserAction flag, bool value = true)
		{
			if (value)
				Field |= (uint16)flag;
			else
				Field &= ~(uint16)flag;
		}

		HLAPI bool IsSet(AssetBrowserAction flag) const
		{
			return (uint16)flag & Field;
		}
	};

	struct DirectoryInfo : public IsSharedReference
	{
		AssetHandle Handle;
		Ref<DirectoryInfo> Parent;

		FileSystemPath FilePath;

		std::vector<AssetHandle> Assets;
		std::unordered_map<AssetHandle, Ref<DirectoryInfo>> SubDirectories;
	};

	class AssetBrowserBaseItem : public IsSharedReference
	{
	public:

		enum class ItemType : uint16
		{
			Directory,	// represents a folder inside the asset folder
			Asset		// Can be any file type supported by the engine
		};

		HLAPI AssetBrowserBaseItem(ItemType type, AssetHandle id, const HLString &name, const Ref<Texture2D> &icon);
		HLAPI virtual ~AssetBrowserBaseItem() {}

		HLAPI void OnRenderBegin();
		HLAPI AssetBrowserActionResult OnRender(float thumbnailSize, bool shouldDisplayAssetType, float edgeOffset = 4.0f);
		HLAPI void OnRenderEnd();

		// Should be overwritten by other browser items (deleting and moving works differently for folders and files)
		HLAPI virtual void Delete() {}
		HLAPI virtual bool Move(const FileSystemPath &dest) { return false; }
		HLAPI virtual void Activate(AssetBrowserActionResult &actionResult) {}

		HLAPI AssetHandle GetID() const { return m_ID; }
		HLAPI bool IsSelected() const { return m_IsSelected; }
		HLAPI bool IsRenaming() const { return m_IsRenaming; }
		HLAPI ItemType GetType() const { return m_Type; }
		HLAPI const HLString &GetName() const { return m_Name; }
		HLAPI const Ref<Texture2D> &GetIcon() const { return m_Icon; }

		HLAPI void StartRenaming();
		HLAPI void Rename(const HLString &newName);
		HLAPI void StopRenaming();

		HLAPI void SetSelected(bool value = true);

	private:

		virtual void OnRenamed(const HLString &newName) { m_Name = newName; }
		virtual void RenderCustomContextItems() {}
		virtual void UpdateDrop(AssetBrowserActionResult &actionResult) {}

		void OnContextMenuOpen(AssetBrowserActionResult &actionResult);

	protected:

		ItemType m_Type;
		AssetHandle m_ID;
		HLString m_Name;
		Ref<Texture2D> m_Icon;

		// possible states
		bool m_IsSelected = false;
		bool m_IsRenaming = false;
		bool m_IsDragging = false;

	private:

		friend class AssetBrowserPanel;
		friend class AssetManager;
	};

	class AssetBrowserDirectory : public AssetBrowserBaseItem
	{
	public:

		HLAPI AssetBrowserDirectory(const Ref<DirectoryInfo> &dirInfo, const Ref<Texture2D> &icon);
		HLAPI virtual ~AssetBrowserDirectory();

		HLAPI Ref<DirectoryInfo> &GetDirectoryInfo() { return m_DirectoryInfo; }

		// Inherited from AssetBrowserBaseItem
		HLAPI virtual void Delete() override;
		HLAPI virtual bool Move(const FileSystemPath &dest) override;

	private:

		virtual void Activate(AssetBrowserActionResult &actionResult) override;
		virtual void OnRenamed(const HLString &newName) override;
		virtual void UpdateDrop(AssetBrowserActionResult &actionResult) override;

		Ref<DirectoryInfo> m_DirectoryInfo;

	private:

		friend class AssetManager;
	};

	class AssetBrowserItem : public AssetBrowserBaseItem
	{
	public:

		HLAPI AssetBrowserItem(const AssetMetaData &metaData, const Ref<Texture2D> &icon);
		HLAPI virtual ~AssetBrowserItem();

		HLAPI const AssetMetaData &GetAssetInfo() const { return m_AssetInfo; }

		HLAPI virtual void Delete() override;
		HLAPI virtual bool Move(const FileSystemPath &dest) override;

	private:

		virtual void Activate(AssetBrowserActionResult &actionResult) override;
		virtual void OnRenamed(const HLString &newName) override;

		AssetMetaData m_AssetInfo;

	private:

		friend class AssetManager;
	};

	namespace utils
	{
		static HLString AssetBrowserItemTypeToString(AssetBrowserBaseItem::ItemType type)
		{
			switch (type)
			{
				case AssetBrowserBaseItem::ItemType::Asset:
					return "Asset";

				case AssetBrowserBaseItem::ItemType::Directory:
					return "Directory";
			}

			return "Unknown";
		}
	}
}

