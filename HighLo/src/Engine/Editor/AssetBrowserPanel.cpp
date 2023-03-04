// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "AssetBrowserPanel.h"

#include "Engine/Core/Input.h"
#include "Engine/Core/FileSystem.h"
#include "Engine/Core/FileSystemWatcher.h"
#include "Engine/ImGui/imgui.h"
#include "Engine/ImGui/ImGuiScopeHelpers.h"
#include "Engine/ImGui/ImGuiTreeNode.h"
#include "Engine/ImGui/ImGuiWidgets.h"
#include "Engine/Assets/AssetManager.h"
#include "Engine/Scene/Scene.h"
#include "Engine/ECS/Prefab.h"
#include "Engine/Math/Color.h"
#include "Engine/Application/Application.h"

namespace highlo
{
	namespace UI
	{
		static bool TreeNode(const HLString &id, const HLString &label, ImGuiTreeNodeFlags flags = 0, const Ref<Texture2D> &icon = nullptr)
		{
			ImGuiWindow *window = ImGui::GetCurrentWindow();
			if (window->SkipItems)
				return false;

			return UI::TreeNodeWithIcon(icon, window->GetID(*id), flags, *label, NULL);
		}
	}

	AssetBrowserPanel *AssetBrowserPanel::s_Instance = nullptr;
	static AssetBrowserSettings *s_AssetBrowserSettings = nullptr;
	static std::mutex s_LockMutex;

	AssetBrowserPanel::AssetBrowserPanel(Ref<Project> project)
		: m_Project(project)
	{
		s_Instance = this;
		s_AssetBrowserSettings = new AssetBrowserSettings();

		// Load Textures
		m_Shadow = Texture2D::LoadFromFile("assets/Resources/icons/shadow.png");
		m_FileIcon = Texture2D::LoadFromFile("assets/Resources/icons/file.png");
		m_FolderIcon = Texture2D::LoadFromFile("assets/Resources/icons/folder.png");

		Ref<Texture2D> imageTex = Texture2D::LoadFromFile("assets/Resources/icons/file-image-solid.png");

		// Load Asset Icons
		m_AssetIconMap["fbx"] = Texture2D::LoadFromFile("assets/Resources/icons/fbx-file-format.png");
		m_AssetIconMap["obj"] = Texture2D::LoadFromFile("assets/Resources/icons/object-file-format.png");
		m_AssetIconMap["wav"] = Texture2D::LoadFromFile("assets/Resources/icons/file-audio-solid.png");
		m_AssetIconMap["otf"] = Texture2D::LoadFromFile("assets/Resources/icons/font-solid.png");
		m_AssetIconMap["png"] = imageTex;
		m_AssetIconMap["jpg"] = imageTex;
		m_AssetIconMap["jpeg"] = imageTex;
		m_AssetIconMap["tiff"] = imageTex;
		m_AssetIconMap["tga"] = imageTex;
		m_AssetIconMap["hdr"] = Texture2D::LoadFromFile("assets/Resources/icons/hdr-file-format.png");
		m_AssetIconMap["ttf"] = Texture2D::LoadFromFile("assets/Resources/icons/font-solid.png");
		m_AssetIconMap["cs"] = Texture2D::LoadFromFile("assets/Resources/icons/font-solid.png");
	//	m_AssetIconMap["hlscene"] = Texture2D::LoadFromFile("assets/Resources/icons/");

		HLString basePath = project->GetAssetDirectory().String();
		if (basePath.EndsWith('/'))
			basePath = basePath.Substr(0, basePath.LastIndexOf('/'));

		m_BasePath = basePath;

		AssetHandle baseDirHandle = ProcessDirectory(basePath, nullptr);
		m_BaseDirectory = m_Directories[baseDirHandle];
		ChangeDirectory(m_BaseDirectory);

		memset(m_SearchBuffer, 0, HL_MAX_INPUT_BUFFER_LENGTH);
	}

	AssetBrowserPanel::~AssetBrowserPanel()
	{
		delete s_AssetBrowserSettings;
	}
	
	void AssetBrowserPanel::OnUIRender()
	{
		Translation *translation = HLApplication::Get().GetActiveTranslation();

		ImGui::Begin("Asset Browser", NULL, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);
		m_IsAssetBrowserHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows);
		m_IsAssetBrowserFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);
		const float topBarHeight = 26.0f;
		const float bottomBarHeight = 52.0f;

		{
			UI::ScopedStyle spacing(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
			UI::ScopedStyle padding(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 4.0f));

			UI::ScopedStyle cellPadding(ImGuiStyleVar_CellPadding, ImVec2(10.0f, 2.0f));

			ImGuiTableFlags tableFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_BordersInnerV;
			UI::PushID();

			if (ImGui::BeginTable(UI::GenerateID(), 2, tableFlags, ImVec2(0.0f, 0.0f)))
			{
				ImGui::TableSetupColumn("Outliner", 0, 300.0f);
				ImGui::TableSetupColumn("Directory Structure", ImGuiTableColumnFlags_WidthStretch);

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);

				// Content Outliner

				ImGui::BeginChild("##folders_common");
				{
					if (ImGui::CollapsingHeader(translation->GetText("asset-browser-content-headline"), nullptr, ImGuiTreeNodeFlags_DefaultOpen))
					{
						UI::ScopedStyle spacing(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
						UI::ScopedColorStack itemBg(ImGuiCol_Header, IM_COL32_DISABLE, ImGuiCol_HeaderActive, IM_COL32_DISABLE);

						SortSubDirectories();

						for (auto &[handle, directory] : m_BaseDirectory->SubDirectories)
							RenderDirectoryHierarchy(directory);
					}

					// Draw side shadow
					ImRect windowRect = UI::RectExpanded(ImGui::GetCurrentWindow()->Rect(), 0.0f, 10.0f);
					ImGui::PushClipRect(windowRect.Min, windowRect.Max, false);
					UI::DrawShadowInner(m_Shadow, 20, windowRect, 1.0f, windowRect.GetHeight() / 4.0f, false, true, false, false);
					ImGui::PopClipRect();
				}
				ImGui::EndChild();

				ImGui::TableSetColumnIndex(1);

				// Directory Content

				ImGui::BeginChild("##directory_structure", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetWindowHeight() - topBarHeight - bottomBarHeight));
				{
					ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
					RenderTopBar(topBarHeight);
					ImGui::PopStyleVar();

					ImGui::Separator();

					ImGui::BeginChild("Scrolling");
					{
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 0.35f));

						ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4.0f, 4.0f));
						if (ImGui::BeginPopupContextWindow(0, 1, false))
						{
							if (ImGui::BeginMenu((HLString(ICON_FA_PLUS) + " " + translation->GetText("asset-browser-right-click-menu-new")).C_Str()))
							{
								if (ImGui::MenuItem((HLString(ICON_FA_FOLDER) + " " + translation->GetText("asset-browser-right-click-menu-new-folder")).C_Str()))
								{
									FileSystemPath newFilePath = Project::GetAssetDirectory() / m_CurrentDirectory->FilePath / "New Folder";
									bool created = FileSystem::Get()->CreateFolder(newFilePath);
									if (created)
									{
										Refresh();
										const auto &directoryInfo = GetDirectory(newFilePath);
										if (directoryInfo)
										{
											uint32 index = m_CurrentItems.Find(directoryInfo->Handle);
											if (index != AssetBrowserItemList::InvalidItem)
												m_CurrentItems[index]->StartRenaming();
										}
										else
										{
											HL_CORE_ERROR("Could not retrieve directory info for {0}", **newFilePath);
										}
									}
									else
									{
										HL_CORE_ERROR("Could not create Folder {0}", **newFilePath);
									}
								}

								if (ImGui::MenuItem((HLString(ICON_FA_FILM) + " " + translation->GetText("asset-browser-right-click-menu-new-scene")).C_Str()))
								{
									CreateAssetRepresentation<Scene>("New Scene.hlscene");
								}

								if (ImGui::MenuItem(HLString(ICON_FA_FILE) + " " + "Script"))
								{
									// TODO: Show create dialogue for script creation
									HL_CORE_TRACE("Trying to create script file in {}", **m_CurrentDirectory->FilePath);
								}

								ImGui::EndMenu();
							}

							if (ImGui::MenuItem((HLString(ICON_FA_TRUCK_LOADING) + " " + translation->GetText("asset-browser-right-click-menu-import").C_Str())))
							{
								// TODO
							}

							if (ImGui::MenuItem((HLString(ICON_FA_SYNC_ALT) + " " + translation->GetText("asset-browser-right-click-menu-refresh")).C_Str()))
								Refresh();

							if (ImGui::MenuItem((HLString(ICON_FA_COPY) + " " + translation->GetText("asset-browser-right-click-menu-copy")).C_Str(), "Ctrl+C", nullptr, m_SelectionStack.Count() > 0))
								m_CopiedAssets.CopyFrom(m_SelectionStack);

							if (ImGui::MenuItem((HLString(ICON_FA_PASTE) + " " + translation->GetText("asset-browser-right-click-menu-paste")).C_Str(), "Ctrl+V", nullptr, m_CopiedAssets.Count() > 0))
								PasteCopiedAssets();

							if (ImGui::MenuItem((HLString(ICON_FA_CLONE) + " " + translation->GetText("asset-browser-right-click-menu-duplicate")).C_Str(), "Ctrl+D", nullptr, m_SelectionStack.Count() > 0))
							{
								m_CopiedAssets.CopyFrom(m_SelectionStack);
								PasteCopiedAssets();
							}

							ImGui::Separator();

							if (ImGui::MenuItem((HLString(ICON_FA_EXTERNAL_LINK_ALT) + " " + translation->GetText("asset-browser-right-click-menu-explorer")).C_Str()))
							{
								FileSystemPath p = m_CurrentDirectory->FilePath.Absolute();
								FileSystem::Get()->OpenInExplorer(p);
							}

							ImGui::EndPopup();
						}
						ImGui::PopStyleVar(); // ItemSpacing

						const float paddingForOutline = 2.0f;
						const float scrollBarrOffset = 20.0f + ImGui::GetStyle().ScrollbarSize;
						float panelWidth = ImGui::GetContentRegionAvail().x - scrollBarrOffset;
						float cellSize = s_AssetBrowserSettings->ThumbnailSize + s_AssetBrowserSettings->Padding + paddingForOutline;
						int32 columnCount = (int32)(panelWidth / cellSize);
						if (columnCount < 1) columnCount = 1;

						{
							const float rowSpacing = 12.0f;
							UI::ScopedStyle spacing(ImGuiStyleVar_ItemSpacing, ImVec2(paddingForOutline, rowSpacing));
							ImGui::Columns(columnCount, 0, false);

							UI::ScopedStyle border(ImGuiStyleVar_FrameBorderSize, 0.0f);
							UI::ScopedStyle padding(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
							RenderItems();
						}

						if (ImGui::IsWindowFocused() && !ImGui::IsMouseDragging(ImGuiMouseButton_Left))
							UpdateInput();

						ImGui::PopStyleColor(2);

						RenderDeleteDialogue();
					}
					ImGui::EndChild();
				}
				ImGui::EndChild();

				if (ImGui::BeginDragDropTarget())
				{
					const ImGuiPayload *data = ImGui::AcceptDragDropPayload("scene_entity_hierarchy");
					if (data)
					{
						UUID &entityId = *(UUID*)data->Data;
						Scene *currentScene = Scene::GetActiveScene();
						HL_ASSERT(currentScene, "No scene has been created yet");

						Entity &e = currentScene->FindEntityByUUID(entityId);
						Ref<Prefab> prefab = CreateAssetRepresentation<Prefab>("New Prefab.hprefab");
						prefab->Create(e);

					}
					ImGui::EndDragDropTarget();
				}

				ImGui::EndTable();
			}
			UI::PopID();
		}
		ImGui::End();
	}

	void AssetBrowserPanel::OnEvent(Event &e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(HL_BIND_EVENT_FUNCTION(AssetBrowserPanel::OnKeyPressedEvent));
		dispatcher.Dispatch<FileSystemChangedEvent>(HL_BIND_EVENT_FUNCTION(AssetBrowserPanel::OnFileSystemChangedEvent));
	}
	
	Ref<DirectoryInfo> AssetBrowserPanel::GetDirectory(const FileSystemPath &filePath) const
	{
		if (filePath.String().IsEmpty() || filePath.String() == "." || filePath.String() == "..")
		{
			return m_BaseDirectory;
		}

		for (const auto &[handle, dir] : m_Directories)
		{
			if (dir->FilePath == filePath)
				return dir;
		}

		return nullptr;
	}

	AssetBrowserSettings &AssetBrowserPanel::GetSettings()
	{
		return *s_AssetBrowserSettings;
	}

	void AssetBrowserPanel::RestoreDefaultSettings()
	{
		s_AssetBrowserSettings->Padding = 2.0f;
		s_AssetBrowserSettings->ThumbnailSize = 128.0f;
		s_AssetBrowserSettings->ShowAssetType = false;
	}

	AssetHandle AssetBrowserPanel::ProcessDirectory(const FileSystemPath &dirPath, const Ref<DirectoryInfo> &parentDirInfo)
	{
		const auto &dir = GetDirectory(dirPath);
		if (dir)
			return dir->Handle;

		Ref<DirectoryInfo> dirInfo = Ref<DirectoryInfo>::Create();
		dirInfo->Handle = AssetHandle();
		dirInfo->Parent = parentDirInfo;

		if (dirPath == m_Project->GetAssetDirectory())
			dirInfo->FilePath = FileSystemPath(); // Empty FileSystemPath is not a valid path
		else
			dirInfo->FilePath = dirPath.RelativePath();

		std::vector<File> dirList = dirPath.GetFileList();
	//	HL_CORE_TRACE("Logged {0} files in directory {1}", dirList.size(), *dirPath.Filename());

		for (uint32 i = 0; i < dirPath.GetFileCount(); ++i)
		{
			File &entry = dirList[i];
			FileSystemPath entryPath = entry;

			if (!entry.IsFile)
			{
				AssetHandle subDirHandle = ProcessDirectory(entryPath, dirInfo);
				dirInfo->SubDirectories[subDirHandle] = m_Directories[subDirHandle];
				continue;
			}

			AssetMetaData &metaData = AssetManager::Get()->GetMetaData(entry.FullPath);
			if (!metaData.IsValid())
			{
				AssetType type = AssetManager::Get()->GetAssetTypeFromPath(entryPath);
				if (type == AssetType::None)
					continue;

				metaData.Handle = AssetManager::Get()->ImportAsset(entryPath);
			}

			// Failed to import asset
			if (!metaData.IsValid())
			{
				HL_CORE_WARN("Failed to import {0}", *entry.FullPath);
				continue;
			}

			dirInfo->Assets.push_back(metaData.Handle);
		}

		m_Directories[dirInfo->Handle] = dirInfo;
		return dirInfo->Handle;
	}
	
	void AssetBrowserPanel::ChangeDirectory(Ref<DirectoryInfo> &dirInfo)
	{
		if (!dirInfo)
			return;

		m_UpdateNavigationPath = true;
		m_CurrentItems.Clear();

		for (auto &[subDirHandle, subDir] : dirInfo->SubDirectories)
			m_CurrentItems.Items.push_back(Ref<AssetBrowserDirectory>::Create(subDir, m_FolderIcon));

		std::vector<AssetHandle> invalidAssets;
		for (auto assetHandle : dirInfo->Assets)
		{
			AssetMetaData metaData = AssetManager::Get()->GetMetaData(assetHandle);
			if (metaData.IsValid())
				m_CurrentItems.Items.push_back(Ref<AssetBrowserItem>::Create(metaData, m_AssetIconMap.find(metaData.FilePath.Extension()) != m_AssetIconMap.end() ? m_AssetIconMap[metaData.FilePath.Extension()] : m_FileIcon));
		}

		SortItemList();

		m_PrevDirectory = dirInfo;
		m_CurrentDirectory = dirInfo;

		ClearSelections();
	}
	
	void AssetBrowserPanel::RemoveDirectory(Ref<DirectoryInfo> &dirInfo, bool removeFromParent)
	{
		if (dirInfo->Parent && removeFromParent)
		{
			auto &childList = dirInfo->Parent->SubDirectories;
			childList.erase(childList.find(dirInfo->Handle));
		}

		for (auto &[handle, subDir] : dirInfo->SubDirectories)
			RemoveDirectory(subDir, false);

		dirInfo->SubDirectories.clear();
		dirInfo->Assets.clear();

		bool result = FileSystem::Get()->RemoveFolder(dirInfo->FilePath);
		if (!result)
		{
			HL_CORE_ERROR("Could not delete {0}", **dirInfo->FilePath);
			return;
		}

		// This has to be the last thing to do, otherwise dirInfo will not be usable
		m_Directories.erase(m_Directories.find(dirInfo->Handle));
	}
	
	void AssetBrowserPanel::RenderDirectoryHierarchy(Ref<DirectoryInfo> &dirInfo)
	{
		const HLString name = dirInfo->FilePath.Filename();
		HLString id = HLString::ToString(dirInfo->Handle) + "_TreeNode";
		
		bool prevState = ImGui::TreeNodeBehaviorIsOpen(ImGui::GetID(*id));

		auto *window = ImGui::GetCurrentWindow();
		window->DC.CurrLineSize.y = 20.0f;
		window->DC.CurrLineTextBaseOffset = 3.0f;

		const ImRect itemRect = { window->WorkRect.Min.x, window->DC.CursorPos.y, window->WorkRect.Max.x, window->DC.CursorPos.y + window->DC.CurrLineSize.y };
		const bool isItemClicked = [&itemRect, &id]
		{
			if (ImGui::ItemHoverable(itemRect, ImGui::GetID(*id)))
			{
				return ImGui::IsMouseDown(ImGuiMouseButton_Left) || ImGui::IsMouseReleased(ImGuiMouseButton_Left);
			}

			return false;
		}();
		const bool isWindowFocused = ImGui::IsWindowFocused();

		auto FillWithColor = [&](const ImColor &color)
		{
			const ImU32 bgColor = ImGui::ColorConvertFloat4ToU32(color);
			ImGui::GetWindowDrawList()->AddRectFilled(itemRect.Min, itemRect.Max, bgColor);
		};

		auto CheckifAnyDescendantSelected = [&](Ref<DirectoryInfo> &dirInfo, auto isAnyDescendantSelected) -> bool
		{
			if (dirInfo->Handle == m_CurrentDirectory->Handle)
				return true;

			if (!dirInfo->SubDirectories.empty())
			{
				for (auto &[childHandle, childDir] : dirInfo->SubDirectories)
				{
					if (isAnyDescendantSelected(childDir, isAnyDescendantSelected))
						return true;
				}
			}

			return false;
		};

		const bool isAnyDescendantSelected = CheckifAnyDescendantSelected(dirInfo, CheckifAnyDescendantSelected);
		const bool isActiveDir = dirInfo->Handle == m_CurrentDirectory->Handle;

		ImGuiTreeNodeFlags flags = (isActiveDir ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_SpanFullWidth;
		if (isActiveDir || isItemClicked)
		{
			if (isWindowFocused)
			{
				FillWithColor(Colors::Theme::Selection);
			}
			else
			{
				const ImColor color = UI::ColorWithMultipliedValue(Colors::Theme::Selection, 0.8f);
				FillWithColor(UI::ColorWithMultipliedSaturation(color, 0.7f));
			}

			ImGui::PushStyleColor(ImGuiCol_Text, Colors::Theme::BackgroundDark);
		}
		else if (isAnyDescendantSelected)
		{
			FillWithColor(Colors::Theme::SelectionMuted);
		}

		// ====================================================================================================================================
		// ====================================================  Draw TreeNode  ===============================================================
		// ====================================================================================================================================

		bool open = UI::TreeNode(id, name, flags, m_FolderIcon);
		if (isActiveDir || isItemClicked)
			ImGui::PopStyleColor();

		UI::ShiftCursorY(3.0f); // Fix Overlap

		// ====================================================================================================================================
		// ====================================================  Draw Children  ===============================================================
		// ====================================================================================================================================

		if (open)
		{
			for (auto &[handle, child] : dirInfo->SubDirectories)
				RenderDirectoryHierarchy(child);
		}

		UpdateDropArea(dirInfo);

		if (open != prevState && !isActiveDir)
		{
			if (!ImGui::IsMouseDragging(ImGuiMouseButton_Left, 0.01f))
				ChangeDirectory(dirInfo);
		}

		if (open)
			ImGui::TreePop();
	}
	
	void AssetBrowserPanel::RenderTopBar(float height, float edgeOffset)
	{
		ImGui::BeginChild("##top_bar", ImVec2(0, height));
		ImGui::BeginHorizontal("##top_bar", ImGui::GetWindowSize());
		Translation *translation = HLApplication::Get().GetActiveTranslation();

		// ====================================================================================================================================
		// ===========================================================  Navigation buttons  ===================================================
		// ====================================================================================================================================
		{
			UI::ScopedStyle spacing(ImGuiStyleVar_ItemSpacing, ImVec2(2.0f, 0.0f));
			auto ContentBrowserButton = [height](const HLString &icon)
			{
				const ImU32 buttonColor = Colors::Theme::BackgroundDark;
				const ImU32 buttonColorP = UI::ColorWithMultipliedValue(Colors::Theme::BackgroundDark, 0.8f);
				UI::ScopedColorStack buttonColors(ImGuiCol_Button, buttonColor, ImGuiCol_ButtonHovered, buttonColor, ImGuiCol_ButtonActive, buttonColorP);

				const float iconSize = std::min(26.0f, height);
				const float iconPadding = 3.0f;
				return ImGui::Button(icon, ImVec2(iconSize, iconSize));
			};

			if (ContentBrowserButton(ICON_FA_ARROW_LEFT))
			{
				m_NextDirectory = m_CurrentDirectory;
				m_PrevDirectory = m_CurrentDirectory->Parent;
				ChangeDirectory(m_PrevDirectory);
			}

			UI::DrawHelpMarker(translation->GetText("asset-browser-prev-dir-btn-tooltip"), false);
			ImGui::Spring(-1.0f, edgeOffset);

			if (ContentBrowserButton(ICON_FA_ARROW_RIGHT))
			{
				ChangeDirectory(m_NextDirectory);
			}

			UI::DrawHelpMarker(translation->GetText("asset-browser-next-dir-btn-tooltip"), false);
			ImGui::Spring(-1.0f, edgeOffset * 2.0f);

			if (ContentBrowserButton(ICON_FA_SYNC_ALT))
				Refresh();

			UI::DrawHelpMarker(translation->GetText("asset-browser-refresh-dir-btn-tooltip"), false);
			ImGui::Spring(-1.0f, edgeOffset * 2.0f);
		}

		// ====================================================================================================================================
		// =======================================================  Search  ===================================================================
		// ====================================================================================================================================

		{
			UI::ShiftCursorY(2.0f);
			ImGui::SetNextItemWidth(200);
			if (UI::Widgets::SearchWidget<HL_MAX_INPUT_BUFFER_LENGTH>(m_SearchBuffer))
			{
				if (strlen(m_SearchBuffer) == 0)
				{
					ChangeDirectory(m_CurrentDirectory);
				}
				else
				{
					m_CurrentItems = Search(m_SearchBuffer, m_CurrentDirectory);
					SortItemList();
				}
			}
			UI::ShiftCursorY(-2.0f);
		}

		if (m_UpdateNavigationPath)
		{
			m_BreadCrumbData.clear();

			Ref<DirectoryInfo> current = m_CurrentDirectory;
			while (current && current->Parent != nullptr)
			{
				m_BreadCrumbData.push_back(current);
				current = current->Parent;
			}

			std::reverse(m_BreadCrumbData.begin(), m_BreadCrumbData.end());
			m_UpdateNavigationPath = false;
		}

		// ====================================================================================================================================
		// =======================================================  Breadcrumbs  ==============================================================
		// ====================================================================================================================================

		{
			UI::ScopedFont boldFont(ImGui::GetIO().Fonts->Fonts[0]);
			UI::ScopedColor textColor(ImGuiCol_Text, Colors::Theme::TextDarker);

		//	const HLString &assetDirName = m_Project->GetConfig().AssetDirectory;
			const HLString &assetDirName = m_BasePath;
			ImVec2 textSize = ImGui::CalcTextSize(*assetDirName);
			const float textPadding = ImGui::GetStyle().FramePadding.y;

			if (ImGui::Selectable(assetDirName, false, 0, ImVec2(textSize.x, textSize.y + textPadding)))
				ChangeDirectory(m_BaseDirectory);

			UpdateDropArea(m_BaseDirectory);

			for (auto &dir : m_BreadCrumbData)
			{
				ImGui::Text("/");

				HLString dirName = dir->FilePath.Filename();
				ImVec2 textSize = ImGui::CalcTextSize(*dirName);
				if (ImGui::Selectable(*dirName, false, 0, ImVec2(textSize.x, textSize.y + textPadding)))
				{
					ChangeDirectory(dir);
				}

				UpdateDropArea(dir);
			}
		}

		ImGui::EndHorizontal();
		ImGui::EndChild();
	}
	
	void AssetBrowserPanel::RenderItems()
	{
		m_IsAnyItemHovered = false;
		bool openDeleteDialogue = false;

		std::lock_guard<std::mutex> lock(s_LockMutex);
		for (auto &item : m_CurrentItems)
		{
			item->OnRenderBegin();
			AssetBrowserActionResult result = item->OnRender(s_AssetBrowserSettings->ThumbnailSize, s_AssetBrowserSettings->ShowAssetType);

			if (result.IsSet(AssetBrowserAction::ClearSelections))
				ClearSelections();

			if (result.IsSet(AssetBrowserAction::Selected) && !m_SelectionStack.IsSelected(item->GetID()))
			{
				m_SelectionStack.Select(item->GetID());
				item->SetSelected();
			}

			if (result.IsSet(AssetBrowserAction::SelectThis) && m_SelectionStack.Count() == 2)
			{
				uint32 firstIndex = m_CurrentItems.Find(m_SelectionStack[0]);
				uint32 lastIndex = m_CurrentItems.Find(item->GetID());

				if (firstIndex > lastIndex)
				{
					uint32 temp = firstIndex;
					firstIndex = lastIndex;
					lastIndex = temp;
				}

				for (uint32 i = firstIndex; i <= lastIndex; ++i)
				{
					auto toSelect = m_CurrentItems[i];
					toSelect->SetSelected();
					m_SelectionStack.Select(toSelect->GetID());
				}
			}

			if (result.IsSet(AssetBrowserAction::StartRenaming))
				item->StartRenaming();

			if (result.IsSet(AssetBrowserAction::Copy))
				m_CopiedAssets.Select(item->GetID());

			if (result.IsSet(AssetBrowserAction::Reload))
				AssetManager::Get()->ReloadAsset(item->GetID());

			if (result.IsSet(AssetBrowserAction::OpenDeleteDialogue))
				openDeleteDialogue = true;

			if (result.IsSet(AssetBrowserAction::ShowInExplorer))
			{
				if (item->GetType() == AssetBrowserBaseItem::ItemType::Directory)
				{
					FileSystemPath p = m_CurrentDirectory->FilePath / item->GetName();
					HL_CORE_TRACE("Opening directory path {0} in explorer", **p);
					FileSystem::Get()->OpenInExplorer(p);
				}
				else
				{
					FileSystemPath p = AssetManager::Get()->GetFileSystemPath(AssetManager::Get()->GetMetaData(item->GetID()));
					HL_CORE_TRACE("Opening asset path {0} in explorer", **p);
					FileSystem::Get()->OpenInExplorer(p);
				}
			}

			if (result.IsSet(AssetBrowserAction::OpenExternal))
			{
				// TODO
			}

			if (result.IsSet(AssetBrowserAction::Hovered))
				m_IsAnyItemHovered = true;

			item->OnRenderEnd();

			if (result.IsSet(AssetBrowserAction::Renamed))
			{
				SortItemList();
				break;
			}

			if (result.IsSet(AssetBrowserAction::NavigateToThis))
			{
				ChangeDirectory(item.As<AssetBrowserDirectory>()->GetDirectoryInfo());
				break;
			}

			if (result.IsSet(AssetBrowserAction::Refresh))
			{
				ChangeDirectory(m_CurrentDirectory);
				break;
			}
		}

		if (openDeleteDialogue)
		{
			ImGui::OpenPopup("Delete");
			openDeleteDialogue = false;
		}
	}
	
	void AssetBrowserPanel::RenderDeleteDialogue()
	{
		if (ImGui::BeginPopupModal("Delete", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			if (m_SelectionStack.Count() == 0)
				ImGui::CloseCurrentPopup();

			ImGui::Text("Are you sure you want to delete %d items?", m_SelectionStack.Count());
			float columnWidth = ImGui::GetContentRegionAvailWidth() / 4;

			ImGui::Columns(4, 0, false);
			ImGui::SetColumnWidth(0, columnWidth);
			ImGui::SetColumnWidth(1, columnWidth);
			ImGui::SetColumnWidth(2, columnWidth);
			ImGui::SetColumnWidth(3, columnWidth);
			ImGui::NextColumn();

			if (ImGui::Button("Yes", ImVec2(columnWidth, 0)))
			{
				for (AssetHandle handle : m_SelectionStack)
				{
					uint32 index = m_CurrentItems.Find(handle);
					if (index == AssetBrowserItemList::InvalidItem)
						continue;

					m_CurrentItems[index]->Delete();
					m_CurrentItems.Erase(handle);
				}

				for (AssetHandle handle : m_SelectionStack)
				{
					if (m_Directories.find(handle) != m_Directories.end())
						RemoveDirectory(m_Directories[handle]);
				}

				m_SelectionStack.Clear();
				ChangeDirectory(m_CurrentDirectory);
				ImGui::CloseCurrentPopup();
			}

			ImGui::NextColumn();
			ImGui::SetItemDefaultFocus();

			if (ImGui::Button("No", ImVec2(columnWidth, 0)))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::NextColumn();
			ImGui::EndPopup();
		}
	}
	
	void AssetBrowserPanel::Refresh()
	{
		std::lock_guard<std::mutex> lock(s_LockMutex);

		m_CurrentItems.Clear();
		m_Directories.clear();

		Ref<DirectoryInfo> currentDir = m_CurrentDirectory;
		AssetHandle baseDirHandle = ProcessDirectory(m_Project->GetAssetDirectory().String(), nullptr);
		m_BaseDirectory = m_Directories[baseDirHandle];
		m_CurrentDirectory = GetDirectory(currentDir->FilePath);

		if (!m_CurrentDirectory)
			m_CurrentDirectory = m_BaseDirectory;

		ChangeDirectory(m_CurrentDirectory);
	}
	
	void AssetBrowserPanel::UpdateInput()
	{
		if (!m_IsAssetBrowserHovered)
			return;

		if ((!m_IsAnyItemHovered && ImGui::IsMouseDown(ImGuiMouseButton_Left)) || Input::IsKeyPressed(HL_KEY_ESCAPE))
			ClearSelections();

		if (Input::IsKeyPressed(HL_KEY_DELETE) && m_SelectionStack.Count() > 0)
		{
			HL_CORE_TRACE("Showing delete popup");
			ImGui::OpenPopup("Delete");
		}

		if (Input::IsKeyPressed(HL_KEY_F5))
		{
			HL_CORE_TRACE("Refreshing from disk");
			Refresh();
		}
	}
	
	void AssetBrowserPanel::PasteCopiedAssets()
	{
		if (m_CopiedAssets.Count() == 0)
			return;

		// TODO:
		// If a file gets copied more times, it looks like this:
		// xxx_01_02.xxx
		// xxx_01_02_03.xxx
		// so the numbers before still have to be cut off!

		auto GetUniquePath = [](const FileSystemPath &p)
		{
			static int32 counter = 0;

			auto CheckFileName = [&p](auto checkFileName) -> FileSystemPath
			{
				++counter;

				const HLString CounterAsStr = []
				{
					if (counter < 10)
						return "0" + HLString::ToString(counter);
					else
						return HLString::ToString(counter);
				}();

				HLString basePath = p.Filename() + "_" + CounterAsStr + "." + p.Extension();
				if (FileSystem::Get()->FileExists(basePath))
					return checkFileName(checkFileName);
				else
					return FileSystemPath(basePath);
			};

			return CheckFileName(CheckFileName);
		};

		FileSystemWatcher::Get()->DisableWatchUntilNextAction();
		for (AssetHandle copiedHandle : m_CopiedAssets)
		{
			const auto &item = m_CurrentItems[m_CurrentItems.Find(copiedHandle)];
			auto originalFilePath = Project::GetAssetDirectory();

			if (item->GetType() == AssetBrowserBaseItem::ItemType::Asset)
			{
				originalFilePath = item.As<AssetBrowserItem>()->GetAssetInfo().FilePath;
				uint32 lastSlash = originalFilePath.String().LastIndexOf('/') + 1;
				FileSystemPath parentPath = FileSystemPath(originalFilePath.String().Substr(0, lastSlash));
				auto newFileName = GetUniquePath(originalFilePath);
				auto filePath = parentPath / newFileName;
				HL_ASSERT(!FileSystem::Get()->FileExists(filePath));

				HL_CORE_TRACE("Copying asset from {0} to {1}", **originalFilePath, **filePath);
				FileSystem::Get()->Copy(filePath, originalFilePath);
			}
			else
			{
				originalFilePath = item.As<AssetBrowserDirectory>()->GetDirectoryInfo()->FilePath;
				auto filePath = GetUniquePath(originalFilePath);
				HL_ASSERT(!FileSystem::Get()->FolderExists(filePath));

				HL_CORE_TRACE("Copying folder from {0} to {1}", **originalFilePath, **filePath);
				FileSystem::Get()->CopyRecursive(filePath, originalFilePath);
			}
		}

		Refresh();
		m_SelectionStack.Clear();
		m_CopiedAssets.Clear();
	}
	
	void AssetBrowserPanel::ClearSelections()
	{
		for (auto &item : m_CurrentItems)
		{
			item->SetSelected(false);

			if (item->IsRenaming())
				item->StopRenaming();
		}

		m_SelectionStack.Clear();
	}
	
	void AssetBrowserPanel::UpdateDropArea(const Ref<DirectoryInfo> &target)
	{
		if (target->Handle != m_CurrentDirectory->Handle && ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("asset_payload");
			if (payload)
			{
				uint32 count = payload->DataSize / sizeof(AssetHandle);
				for (uint32 i = 0; i < count; ++i)
				{
					AssetHandle handle = *(((AssetHandle*)payload->Data) + i);
					uint32 index = m_CurrentItems.Find(handle);
					if (index != AssetBrowserItemList::InvalidItem)
					{
						HL_CORE_TRACE("Moving to {0}", **target->FilePath);
						m_CurrentItems[index]->Move(target->FilePath);
						m_CurrentItems.Erase(handle);
					}
				}
			}

			ImGui::EndDragDropTarget();
		}
	}
	
	void AssetBrowserPanel::SortItemList()
	{
		std::sort(m_CurrentItems.begin(), m_CurrentItems.end(), [](const Ref<AssetBrowserItem> &item1, const Ref<AssetBrowserItem> &item2)
		{
			HLString item1Name = item1->GetName();
			HLString item2Name = item2->GetName();

			if (item1->GetType() == item2->GetType())
				return item1Name.ToLowerCase() < item2Name.ToLowerCase();

			return (uint16)item1->GetType() < (uint16)item2->GetType();
		});
	}

	void AssetBrowserPanel::SortSubDirectories()
	{
		// TODO
	}
	
	AssetBrowserItemList AssetBrowserPanel::Search(const HLString &query, const Ref<DirectoryInfo> &dirInfo)
	{
		AssetBrowserItemList results;
		HLString queryLower = query;
		queryLower = queryLower.ToLowerCase();
		
		for (auto &[handle, subDir] : dirInfo->SubDirectories)
		{
			HLString subDirName = subDir->FilePath.Filename();
			subDirName = subDirName.ToLowerCase();

			if (subDirName.IndexOf(queryLower) != HLString::NPOS)
				results.Items.push_back(Ref<AssetBrowserDirectory>::Create(subDir, m_FolderIcon));

			AssetBrowserItemList list = Search(query, subDir);
			results.Items.insert(results.Items.end(), list.Items.begin(), list.Items.end());
		}

		for (auto &assetHandle : dirInfo->Assets)
		{
			auto &asset = AssetManager::Get()->GetMetaData(assetHandle);
			HLString fileName = asset.FilePath.Filename();
			fileName = fileName.ToLowerCase();

			if (fileName.IndexOf(queryLower) != HLString::NPOS)
				results.Items.push_back(Ref<AssetBrowserItem>::Create(asset, m_AssetIconMap.find(asset.FilePath.Extension()) != m_AssetIconMap.end() ? m_AssetIconMap[asset.FilePath.Extension()] : m_FileIcon));

			if (queryLower[0] != '.')
				continue;

			if (asset.FilePath.Extension().IndexOf(&queryLower[1]) != HLString::NPOS)
				results.Items.push_back(Ref<AssetBrowserItem>::Create(asset, m_AssetIconMap.find(asset.FilePath.Extension()) != m_AssetIconMap.end() ? m_AssetIconMap[asset.FilePath.Extension()] : m_FileIcon));
		}

		return results;
	}
	
	bool AssetBrowserPanel::OnFileSystemChangedEvent(FileSystemChangedEvent &e)
	{
		Refresh();
		return true;
	}
	
	bool AssetBrowserPanel::OnKeyPressedEvent(KeyPressedEvent &e)
	{
		if (m_IsAssetBrowserFocused)
			return false;

		bool handled = false;
		if (Input::IsKeyPressed(HL_KEY_LEFT_CONTROL))
		{
			switch (e.GetKeyCode())
			{
				case HL_KEY_C:
					m_CopiedAssets.CopyFrom(m_SelectionStack);
					handled = true;
					break;

				case HL_KEY_V:
					PasteCopiedAssets();
					handled = true;
					break;

				case HL_KEY_D:
					m_CopiedAssets.CopyFrom(m_SelectionStack);
					PasteCopiedAssets();
					handled = true;
					break;
			}
		}

		return handled;
	}
}

