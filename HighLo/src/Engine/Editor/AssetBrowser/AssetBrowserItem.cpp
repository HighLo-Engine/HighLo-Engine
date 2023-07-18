// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "AssetBrowserItem.h"

#include "Engine/Editor/AssetBrowserPanel.h"
#include "Engine/Assets/AssetManager.h"
#include "Engine/Core/Input.h"
#include "Engine/Core/FileSystem.h"
#include "Engine/Scene/Project.h"
#include "Engine/Math/Color.h"
#include "Engine/ImGui/ImGui.h"
#include "Engine/ImGui/ImGuiScopeHelpers.h"
#include "Engine/Application/Application.h"

namespace highlo
{
	static char s_RenameBuffer[HL_MAX_INPUT_BUFFER_LENGTH];

	AssetBrowserBaseItem::AssetBrowserBaseItem(ItemType type, AssetHandle id, const HLString &name, const Ref<Texture2D> &icon)
		: m_Type(type), m_ID(id), m_Name(name), m_Icon(icon)
	{
	}

	void AssetBrowserBaseItem::OnRenderBegin()
	{
		ImGui::PushID(&m_ID);
		ImGui::BeginGroup();
	}

	AssetBrowserActionResult AssetBrowserBaseItem::OnRender(float thumbnailSize, bool shouldDisplayAssetType, float edgeOffset)
	{
		AssetBrowserActionResult result;

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

		const float textLineHeight = ImGui::GetTextLineHeightWithSpacing() * 2.0f + edgeOffset * 2.0f;
		const float infoPanelHeight = HL_MAX(shouldDisplayAssetType ? thumbnailSize * 0.5f : textLineHeight, textLineHeight);

		const ImVec2 topLeftPos = ImGui::GetCursorScreenPos();
		const ImVec2 thumbnailBottomRight = { topLeftPos.x + thumbnailSize, topLeftPos.y + thumbnailSize };
		const ImVec2 infoPanelTopLeft = { topLeftPos.x, topLeftPos.y + thumbnailSize };
		const ImVec2 bottomRightPos = { topLeftPos.x + thumbnailSize, topLeftPos.y + thumbnailSize + infoPanelHeight };

		auto drawShadowCmd = [](const ImVec2 &topLeftPos, const ImVec2 &bottomRightPos, bool isDirectory)
		{
			auto *drawList = ImGui::GetWindowDrawList();
			const ImRect itemRect = UI::RectOffset(ImRect(topLeftPos, bottomRightPos), 1.0f, 1.0f);
			drawList->AddRect(itemRect.Min, itemRect.Max, Colors::Theme::PropertyField, 6.0f, isDirectory ? 0 : ImDrawFlags_RoundCornersBottom, 2.0f);
		};

		// ====================================================================================================================================
		// ===================================================  Fill backrouund  ==============================================================
		// ====================================================================================================================================
		
		if (m_Type != ItemType::Directory)
		{
			auto *drawList = ImGui::GetWindowDrawList();

			// Draw Item Shadow
			drawShadowCmd(topLeftPos, bottomRightPos, false);

			// Draw background
			drawList->AddRectFilled(topLeftPos, thumbnailBottomRight, Colors::Theme::BackgroundDark);
			drawList->AddRectFilled(infoPanelTopLeft, bottomRightPos, Colors::Theme::GroupHeader, 6.0f, ImDrawFlags_RoundCornersBottom);
		}
		else if (ImGui::ItemHoverable(ImRect(topLeftPos, bottomRightPos), ImGui::GetID(&m_ID)) || m_IsSelected)
		{
			// Draw Item shadow
			drawShadowCmd(topLeftPos, bottomRightPos, true);

			auto *drawList = ImGui::GetWindowDrawList();
			drawList->AddRectFilled(topLeftPos, bottomRightPos, Colors::Theme::GroupHeader, 6.0f);
		}

		// ====================================================================================================================================
		// =====================================================  Thumbnail  ==================================================================
		// ====================================================================================================================================

		ImGui::InvisibleButton("##thumbnailBtn", ImVec2(thumbnailSize, thumbnailSize));
		UI::DrawButtonImage(m_Icon, IM_COL32(255, 255, 255, 255), IM_COL32(255, 255, 255, 255), IM_COL32(255, 255, 255, 255), UI::RectExpanded(UI::GetItemRect(), -6.0f, -6.0f));

		// ====================================================================================================================================
		// ======================================================  Info Panel  ================================================================
		// ====================================================================================================================================

		auto renamingWidgetCmd = [&]
		{
			ImGui::SetKeyboardFocusHere();
			if (ImGui::InputText("##renameWidget", s_RenameBuffer, HL_MAX_INPUT_BUFFER_LENGTH, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				Rename(s_RenameBuffer);
				m_IsRenaming = false;
				result.Set(AssetBrowserAction::Renamed);
			}
		};

		UI::ShiftCursor(edgeOffset, edgeOffset);
		if (m_Type == ItemType::Directory)
		{
			ImGui::BeginVertical((HLString("InfoPanel") + m_Name).C_Str(), ImVec2(thumbnailSize - edgeOffset * 2.0f, infoPanelHeight - edgeOffset));
			ImGui::BeginHorizontal(*m_Name, ImVec2(thumbnailSize - 2.0f, 0.0f));

			ImGui::Spring();
			ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + (thumbnailSize - edgeOffset * 3.0f));

			const float textWidth = HL_MIN(ImGui::CalcTextSize(*m_Name).x, thumbnailSize);
			if (m_IsRenaming)
			{
				ImGui::SetNextItemWidth(thumbnailSize - edgeOffset * 3.0f);
				renamingWidgetCmd();
			}
			else
			{
				ImGui::SetNextItemWidth(textWidth);
				ImGui::Text("%s", *m_Name);
			}

			ImGui::PopTextWrapPos();
			ImGui::Spring();

			ImGui::EndHorizontal();
			ImGui::EndVertical();
		}
		else
		{
			ImGui::BeginVertical((HLString("InfoPanel") + m_Name).C_Str(), ImVec2(thumbnailSize - edgeOffset * 3.0f, infoPanelHeight - edgeOffset));
			ImGui::BeginHorizontal("label", ImVec2(0.0f, 0.0f));

			ImGui::SuspendLayout();
			ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + (thumbnailSize - edgeOffset * 2.0f));

			if (m_IsRenaming)
			{
				ImGui::SetNextItemWidth(thumbnailSize - edgeOffset * 3.0f);
				renamingWidgetCmd();
			}
			else
			{
				ImGui::Text("%s", *m_Name);
			}

			ImGui::PopTextWrapPos();
			ImGui::ResumeLayout();

			ImGui::EndHorizontal();

			ImGui::Spring();
			if (shouldDisplayAssetType)
			{
				ImGui::BeginHorizontal("assetType", ImVec2(0.0f, 0.0f));
				ImGui::Spring();

				const AssetMetaData &metaData = AssetManager::Get()->GetMetaData(m_ID);
				HLString assetType = utils::AssetTypeToString(metaData.Type);

				UI::ScopedColor textColor(ImGuiCol_Text, Colors::Theme::TextDarker);
				ImGui::TextUnformatted(*assetType.ToUpperCase());

				ImGui::EndHorizontal();
				ImGui::Spring(-1.0f, edgeOffset);
			}
			ImGui::EndVertical();
		}

		UI::ShiftCursor(-edgeOffset, -edgeOffset);
		if (!m_IsRenaming)
		{
			if (Input::IsKeyPressed(HL_KEY_F2) && m_IsSelected)
			{
				StartRenaming();
			}
		}

		ImGui::PopStyleVar(); // ItemSpacing
		ImGui::EndGroup(); // End of Item Group from OnRenderStart

		// ====================================================================================================================================
		// =====================================================  Draw Outline  ===============================================================
		// ====================================================================================================================================

		if (m_IsSelected || ImGui::IsItemHovered())
		{
			ImRect itemRect = UI::GetItemRect();
			auto *drawList = ImGui::GetWindowDrawList();

			if (m_IsSelected) // Item is selected (draw outline)
			{
				const bool mouseDown = ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsItemHovered();
				ImColor transitionColor = UI::ColorWithMultipliedValue(Colors::Theme::Selection, 0.8f);
				ImColor defaultColor = UI::ColorWithMultipliedValue(Colors::Theme::Selection, 1.0f); // NOTE: workaround for passing the Colors::Theme::Selection, as it is just a uint32 instead of ImColor
				
				drawList->AddRect(itemRect.Min, itemRect.Max, mouseDown ? transitionColor : defaultColor, 6.0f, m_Type == ItemType::Directory ? 0 : ImDrawFlags_RoundCornersBottom, 1.0f);
			}
			else // Item is hovered
			{
				if (m_Type != ItemType::Directory)
				{
					drawList->AddRect(itemRect.Min, itemRect.Max, Colors::Theme::Muted, 6.0f, ImDrawFlags_RoundCornersBottom, 1.0f);
				}
			}
		}

		// ====================================================================================================================================
		// =================================================  ImGui Mouse Event Handling  =====================================================
		// ====================================================================================================================================

		UpdateDrop(result);

		bool dragging = false;
		if (dragging = ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
		{
			const auto &selectionStack = AssetBrowserPanel::Get().GetSelectionStack();
			if (!selectionStack.IsSelected(m_ID))
				result.Set(AssetBrowserAction::ClearSelections);

			auto &currentItems = AssetBrowserPanel::Get().GetCurrentItems();
			if (selectionStack.Count() > 0)
			{
				for (const auto &selectionItemHandles : selectionStack)
				{
					uint32 index = currentItems.Find(selectionItemHandles);
					if (index == AssetBrowserItemList::InvalidItem)
						continue;

					const auto &item = currentItems[index];
					UI::Image(item->GetIcon(), ImVec2(20, 20));
					ImGui::SameLine();
					const auto &name = item->GetName();
					ImGui::TextUnformatted(*name);
				}

				ImGui::SetDragDropPayload("asset_payload", selectionStack.Data(), sizeof(AssetHandle) * selectionStack.Count());
			}

			result.Set(AssetBrowserAction::Selected);
			ImGui::EndDragDropSource();
		}

		if (ImGui::IsItemHovered())
		{
			result.Set(AssetBrowserAction::Hovered);

			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				Activate(result);
			}
			else
			{
				const auto &selectionStack = AssetBrowserPanel::Get().GetSelectionStack();

				bool action = selectionStack.Count() > 1 ? ImGui::IsMouseReleased(ImGuiMouseButton_Left) : ImGui::IsMouseClicked(ImGuiMouseButton_Left);
				bool skipWhileDragging = m_IsDragging && selectionStack.IsSelected(m_ID);
				if (action && !skipWhileDragging)
				{
					result.Set(AssetBrowserAction::Selected);

					if (!Input::IsKeyPressed(HL_KEY_LEFT_CONTROL) && !Input::IsKeyPressed(HL_KEY_LEFT_SHIFT))
						result.Set(AssetBrowserAction::ClearSelections);

					if (Input::IsKeyPressed(HL_KEY_LEFT_SHIFT))
						result.Set(AssetBrowserAction::SelectThis);
				}
			}
		}

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4.0f, 4.0f));
		if (ImGui::BeginPopupContextItem("AssetBrowserContextMenu"))
		{
			result.Set(AssetBrowserAction::Selected);

			if (!Input::IsKeyPressed(HL_KEY_LEFT_CONTROL) && !Input::IsKeyPressed(HL_KEY_LEFT_SHIFT))
				result.Set(AssetBrowserAction::ClearSelections);

			if (Input::IsKeyPressed(HL_KEY_LEFT_SHIFT))
				result.Set(AssetBrowserAction::SelectThis);

			OnContextMenuOpen(result);
			ImGui::EndPopup();
		}

		ImGui::PopStyleVar();

		m_IsDragging = dragging;
		return result;
	}

	void AssetBrowserBaseItem::OnRenderEnd()
	{
		ImGui::PopID();
		ImGui::NextColumn();
	}

	void AssetBrowserBaseItem::StartRenaming()
	{
		if (m_IsRenaming)
			return;

		m_IsRenaming = true;
		memset(s_RenameBuffer, 0, HL_MAX_INPUT_BUFFER_LENGTH);
		memcpy(s_RenameBuffer, *m_Name, m_Name.Length());
	}

	void AssetBrowserBaseItem::Rename(const HLString &newName)
	{
		OnRenamed(newName);
	}

	void AssetBrowserBaseItem::StopRenaming()
	{
		m_IsRenaming = false;
		memset(s_RenameBuffer, 0, HL_MAX_INPUT_BUFFER_LENGTH);
	}

	void AssetBrowserBaseItem::SetSelected(bool value)
	{
		m_IsSelected = value;
	}

	void AssetBrowserBaseItem::OnContextMenuOpen(AssetBrowserActionResult &actionResult)
	{
		Translation *translation = HLApplication::Get().GetActiveTranslation();

		if (ImGui::MenuItem((HLString(ICON_FA_SYNC_ALT) + " " + translation->GetText("asset-browser-item-reload")).C_Str()))
			actionResult.Set(AssetBrowserAction::Reload);

		if (ImGui::MenuItem((HLString(ICON_FA_EDIT) + " " + translation->GetText("asset-browser-item-rename")).C_Str()))
			actionResult.Set(AssetBrowserAction::StartRenaming);

		if (ImGui::MenuItem((HLString(ICON_FA_COPY) + " " + translation->GetText("asset-browser-item-copy")).C_Str()))
			actionResult.Set(AssetBrowserAction::Copy);

		if (ImGui::MenuItem((HLString(ICON_FA_TRASH_ALT) + " " + translation->GetText("asset-browser-item-delete")).C_Str()))
			actionResult.Set(AssetBrowserAction::OpenDeleteDialogue);

		if (ImGui::MenuItem((HLString(ICON_FA_EXTERNAL_LINK_ALT) + " " + translation->GetText("asset-browser-item-explorer")).C_Str()))
			actionResult.Set(AssetBrowserAction::ShowInExplorer);

		ImGui::Separator();
		RenderCustomContextItems();
	}

	// ====================================================================================================================================
	// ===================================================  AssetBrowserDirectory  ========================================================
	// ====================================================================================================================================

	AssetBrowserDirectory::AssetBrowserDirectory(const Ref<DirectoryInfo> &dirInfo, const Ref<Texture2D> &icon)
		: AssetBrowserBaseItem(AssetBrowserBaseItem::ItemType::Directory, dirInfo->Handle, dirInfo->FilePath.Filename(), icon), m_DirectoryInfo(dirInfo)
	{
	}

	AssetBrowserDirectory::~AssetBrowserDirectory()
	{
	}

	void AssetBrowserDirectory::Delete()
	{
		FileSystemPath p = m_DirectoryInfo->FilePath;
		bool deleted = FileSystem::Get()->RemoveFile(p);
		if (!deleted)
		{
			HL_CORE_WARN("Failed to delete folder {0}!", *m_DirectoryInfo->FilePath.String());
			return;
		}

		for (auto asset : m_DirectoryInfo->Assets)
			AssetManager::OnAssetDeleted(asset);
	}

	bool AssetBrowserDirectory::Move(const FileSystemPath &dest)
	{
		return FileSystem::Get()->Move(Project::GetActive()->GetAssetDirectory() / m_DirectoryInfo->FilePath, Project::GetActive()->GetAssetDirectory() / dest);
	}

	void AssetBrowserDirectory::Activate(AssetBrowserActionResult &actionResult)
	{
		actionResult.Set(AssetBrowserAction::NavigateToThis);
	}

	void AssetBrowserDirectory::OnRenamed(const HLString &newName)
	{
		if (FileSystem::Get()->FileExists(Project::GetActive()->GetAssetDirectory() / m_DirectoryInfo->FilePath.ParentPath() / newName))
		{
			HL_CORE_ERROR("A directory with that name already exists!");
			return;
		}

		FileSystem::Get()->Rename(Project::GetActive()->GetAssetDirectory() / m_DirectoryInfo->FilePath, Project::GetActive()->GetAssetDirectory() / m_DirectoryInfo->FilePath.ParentPath() / newName);
	}

	void AssetBrowserDirectory::UpdateDrop(AssetBrowserActionResult &actionResult)
	{
		if (IsSelected())
			return;

		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("asset_payload");
			if (payload)
			{
				auto &currentItems = AssetBrowserPanel::Get().GetCurrentItems();
				uint32 count = payload->DataSize / sizeof(AssetHandle);

				for (uint32 i = 0; i < count; ++i)
				{
					AssetHandle handle = *(((AssetHandle*)payload->Data) + i);
					uint32 index = currentItems.Find(handle);
					if (index != AssetBrowserItemList::InvalidItem)
					{
						if (currentItems[index]->Move(m_DirectoryInfo->FilePath))
						{
							actionResult.Set(AssetBrowserAction::Refresh);
							currentItems.Erase(handle);
						}
					}
				}
			}

			ImGui::EndDragDropTarget();
		}
	}

	// ====================================================================================================================================
	// ========================================================  AssetBrowserItem  ========================================================
	// ====================================================================================================================================

	AssetBrowserItem::AssetBrowserItem(const AssetMetaData &metaData, const Ref<Texture2D> &icon)
		: AssetBrowserBaseItem(AssetBrowserBaseItem::ItemType::Asset, metaData.Handle, metaData.FilePath.Filename(), icon), m_AssetInfo(metaData)
	{
	}

	AssetBrowserItem::~AssetBrowserItem()
	{
	}

	void AssetBrowserItem::Delete()
	{
		auto filePath = AssetManager::Get()->GetFileSystemPath(m_AssetInfo);
		bool deleted = FileSystem::Get()->RemoveFile(filePath);
		if (!deleted)
		{
			HL_CORE_ERROR("Could not delete {0}", **m_AssetInfo.FilePath);
			return;
		}

		Ref<DirectoryInfo> currentDir = AssetBrowserPanel::Get().GetDirectory(m_AssetInfo.FilePath.ParentPath());
		currentDir->Assets.erase(std::remove(currentDir->Assets.begin(), currentDir->Assets.end(), m_AssetInfo.Handle), currentDir->Assets.end());

		AssetManager::OnAssetDeleted(m_AssetInfo.Handle);
	}

	void AssetBrowserItem::OnRenamed(const HLString &newName)
	{
		FileSystemPath filepath = AssetManager::Get()->GetFileSystemPath(m_AssetInfo);
		FileSystemPath newFilepath = HLString(fmt::format("{0}\\{1}{2}", filepath.ParentPath().String(), newName, filepath.Extension()));

		if (!FileSystem::Get()->FileExists(newFilepath))
		{
			HL_CORE_ERROR("The File {0} already exists!", **newFilepath);
			return;
		}

		FileSystem::Get()->Rename(filepath, newFilepath);
	}

	bool AssetBrowserItem::Move(const FileSystemPath &dest)
	{
		auto filepath = AssetManager::Get()->GetFileSystemPath(m_AssetInfo);
		bool wasMoved = FileSystem::Get()->Move(filepath, Project::GetActive()->GetAssetDirectory() / dest);
		if (!wasMoved)
		{
			HL_CORE_ERROR("Could not move asset from {0} to {1}", **m_AssetInfo.FilePath, **dest);
			return false;
		}

		// TODO: this could be optimized, but should work for now
		FileSystemPath destReplacer = dest;
		AssetManager::OnAssetMoved(m_AssetInfo.Handle, destReplacer);
		return true;
	}

	void AssetBrowserItem::Activate(AssetBrowserActionResult &actionResult)
	{
		HL_CORE_TRACE("Opening editor window {0}", actionResult.Field);
		if (m_AssetInfo.Type == AssetType::Scene)
		{
			// TODO: Open in viewport
		}
		else
		{
		}
	}
}

