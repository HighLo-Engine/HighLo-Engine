// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "SceneHierarchyPanel.h"

#include "Engine/Assets/AssetManager.h"
#include "Engine/ImGui/imgui.h"
#include "Engine/ImGui/ImGuiWidgets.h"
#include "Engine/Application/Application.h"
#include "Engine/Core/Input.h"
#include "Engine/ECS/Prefab.h"
#include "EditorColors.h"
#include "Engine/Factories/AssetFactory.h"

namespace highlo
{
	SelectionContext SceneHierarchyPanel::s_ActiveSelectionContext = SelectionContext::Scene;
	static ImRect s_WindowBounds;
	static bool s_ActivateSearchWidget = false;

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene> &scene, bool isWindow, SelectionContext context)
		: m_Scene(scene), m_IsWindow(isWindow), m_SelectionContext(context)
	{
		if (m_Scene)
			m_Scene->SetEntityDestroyedCallback([this](Entity entity) { OnExternalEntityDestroyed(entity); });

		m_TreeNodeRoot = Texture2D::LoadFromFile("assets/Resources/Icons/treeNode_root.png");
		m_TreeNodeChildren = Texture2D::LoadFromFile("assets/Resources/Icons/treeNode_with_children.png");
		m_TreeNodeChildrenOpened = Texture2D::LoadFromFile("assets/Resources/Icons/treeNode_with_children_opened.png");
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene> &scene)
	{
		m_Scene = scene;
		if (m_Scene)
			m_Scene->SetEntityDestroyedCallback([this](Entity entity) { OnExternalEntityDestroyed(entity); });
	}

	void SceneHierarchyPanel::OnUIRender(bool *pOpen)
	{
		if (m_IsWindow)
		{
			UI::ScopedStyle padding(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::Begin("Scene Hierarchy", pOpen);
		}

		s_ActiveSelectionContext = m_SelectionContext;
		m_IsWindowFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);
		ImRect windowRect = { ImGui::GetWindowContentRegionMin(), ImGui::GetWindowContentRegionMax() };

		{
			const float edgeOffset = 4.0f;
			UI::ShiftCursorX(edgeOffset * 3.0f);
			UI::ShiftCursorY(edgeOffset * 2.0f);

			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - edgeOffset * 3.0f);

			static HLString searchedString;

			if (s_ActivateSearchWidget)
			{
				ImGui::SetKeyboardFocusHere();
				s_ActivateSearchWidget = false;
			}

			UI::Widgets::SearchWidget(searchedString);

			ImGui::Spacing();
			ImGui::Spacing();

			// Entity list
			//------------

			UI::ScopedStyle cellPadding(ImGuiStyleVar_CellPadding, ImVec2(4.0f, 0.0f));

			// Alt row colour
			const ImU32 colRowAlt = UI::ColorWithMultipliedValue(Colors::Theme::BackgroundDark, 1.3f);
			UI::ScopedColor tableBGAlt(ImGuiCol_TableRowBgAlt, colRowAlt);

			// Table
			{
				// Scrollable Table uses child window internally
				UI::ScopedColor tableBg(ImGuiCol_ChildBg, Colors::Theme::BackgroundDark);

				ImGuiTableFlags flags = ImGuiTableFlags_NoPadInnerX
					| ImGuiTableFlags_Resizable
					| ImGuiTableFlags_ScrollY
					| ImGuiTableFlags_RowBg
					| ImGuiTableFlags_NoBordersInBody;

				const int32 numColumns = 3;
				if (ImGui::BeginTable("##SceneHierarchy-Table", numColumns, flags, ImVec2(ImGui::GetContentRegionAvail())))
				{
					// If the user clicks anywhere on the scene hierarchy panel (except on a entity itself), deselect the currently selected entity
					if (m_SelectedEntityID > 0 && m_IsWindowFocused && !ImGui::IsAnyItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
					{
						SelectionManager::Deselect(s_ActiveSelectionContext, m_SelectedEntityID);
						m_SelectedEntityID = 0;
					}

					ImGuiTableFlags columnFlags = ImGuiTableColumnFlags_NoHeaderLabel | ImGuiTableColumnFlags_NoReorder | ImGuiTableColumnFlags_NoResize | ImGuiTableColumnFlags_NoHide | ImGuiTableColumnFlags_NoSort;
					ImGui::TableSetupColumn("", columnFlags); // Label
					ImGui::TableSetupColumn("", columnFlags); // Visibilty
					ImGui::TableSetupColumn("", columnFlags); // Materials and other extra settings

					// Headers
					{
						const ImU32 colActive = UI::ColorWithMultipliedValue(Colors::Theme::GroupHeader, 1.2f);
						UI::ScopedColorStack headerColours(ImGuiCol_HeaderHovered, colActive,
															ImGuiCol_HeaderActive, colActive);

						ImGui::TableSetupScrollFreeze(ImGui::TableGetColumnCount(), 1);

						ImGui::TableNextRow(ImGuiTableRowFlags_Headers, 22.0f);
						for (int32 column = 0; column < ImGui::TableGetColumnCount(); column++)
						{
							ImGui::TableSetColumnIndex(column);
							const char *column_name = ImGui::TableGetColumnName(column);
							UI::ScopedID columnID(column);

							UI::ShiftCursor(edgeOffset * 3.0f, edgeOffset * 2.0f);
							ImGui::TableHeader(column_name);
							UI::ShiftCursor(-edgeOffset * 3.0f, -edgeOffset * 2.0f);
						}

						ImGui::SetCursorPosX(ImGui::GetCurrentTable()->OuterRect.Min.x);
						UI::DrawUnderline(true, 0.0f, 5.0f);
					}

					// List
					{
						UI::ScopedColorStack entitySelection(ImGuiCol_Header, IM_COL32_DISABLE,
															 ImGuiCol_HeaderHovered, IM_COL32_DISABLE,
															 ImGuiCol_HeaderActive, IM_COL32_DISABLE);

						auto &view = m_Scene->m_Registry.View<RelationshipComponent>();
						for (UUID entityId : view)
						{
							Entity e = m_Scene->FindEntityByUUID(entityId);
							if (!e)
								continue;

							// If the parent UUID is 0, there is no parent -> this node is a root node
							if (e.GetParentUUID() == 0)
								DrawEntityNode(e, searchedString);
						}
					}

					if (ImGui::BeginPopupContextWindow(nullptr, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
					{
						DrawEntityCreateMenu({});
						ImGui::EndPopup();
					}


					ImGui::EndTable();
				}
			}

			s_WindowBounds = ImGui::GetCurrentWindow()->Rect();
		}

		if (m_IsWindow)
		{
			ImGui::End();
		}
	}

	void SceneHierarchyPanel::OnExternalEntityDestroyed(Entity entity)
	{
		if (m_EntityDeletedCallback)
			m_EntityDeletedCallback(entity);
	}

	void SceneHierarchyPanel::DrawEntityCreateMenu(Entity parent)
	{
	}
	
	void SceneHierarchyPanel::DrawEntityNode(Entity entity, const HLString &searchFilter)
	{
		HLString name = entity.Tag();

		const uint32 maxSearchDepth = 10;
		bool hasChildMatchingSearch = false;

		// TODO
	//	bool hasChildMatchingSearch = TagSearchRecursive(entity, searchFilter, maxSearchDepth);

		if (!UI::IsMatchingSearch(name, searchFilter) && !hasChildMatchingSearch)
			return;

		const float edgeOffset = 4.0f;
		const float rowHeight = 21.0f;

		// ImGui item height tweaks
		auto *window = ImGui::GetCurrentWindow();
		window->DC.CurrLineSize.y = rowHeight;
		//---------------------------------------------
		ImGui::TableNextRow(0, rowHeight);

		// Label column
		//-------------

		ImGui::TableNextColumn();

		window->DC.CurrLineTextBaseOffset = 3.0f;

		const ImVec2 rowAreaMin = ImGui::TableGetCellBgRect(ImGui::GetCurrentTable(), 0).Min;
		const ImVec2 rowAreaMax = { ImGui::TableGetCellBgRect(ImGui::GetCurrentTable(), ImGui::TableGetColumnCount() - 1).Max.x - 20,
									rowAreaMin.y + rowHeight };

		const bool isSelected = SelectionManager::IsSelected(s_ActiveSelectionContext, entity.GetUUID());

		ImGuiTreeNodeFlags flags = (isSelected ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

		if (hasChildMatchingSearch)
			flags |= ImGuiTreeNodeFlags_DefaultOpen;

		if (entity.Children().empty())
			flags |= ImGuiTreeNodeFlags_Leaf;


		const HLString strID = fmt::format("{0}{1}", name, (uint64)entity.GetUUID());

		ImGui::PushClipRect(rowAreaMin, rowAreaMax, false);
		bool isRowHovered, held;
		bool isRowClicked = ImGui::ButtonBehavior(ImRect(rowAreaMin, rowAreaMax), ImGui::GetID(*strID),
												  &isRowHovered, &held, ImGuiButtonFlags_AllowItemOverlap | ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
		bool wasRowRightClicked = ImGui::IsMouseReleased(ImGuiMouseButton_Right);

		ImGui::SetItemAllowOverlap();

		ImGui::PopClipRect();

		const bool isWindowFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

		// Row colouring
		//--------------

		// Fill with light selection colour if any of the child entities selected
		auto isAnyDescendantSelected = [&](Entity ent, auto isAnyDescendantSelected) -> bool
		{
			if (SelectionManager::IsSelected(s_ActiveSelectionContext, ent.GetUUID()))
				return true;

			if (!ent.Children().empty())
			{
				for (auto &childEntityID : ent.Children())
				{
					Entity childEntity = m_Scene->FindEntityByUUID(childEntityID);
					if (isAnyDescendantSelected(childEntity, isAnyDescendantSelected))
						return true;
				}
			}

			return false;
		};

		auto fillRowWithColour = [](const ImColor &colour)
		{
			for (int column = 0; column < ImGui::TableGetColumnCount(); column++)
				ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, colour, column);
		};

		if (isSelected)
		{
			if (isWindowFocused || UI::NavigatedTo())
				fillRowWithColour(Colors::Theme::Selection);
			else
			{
				const ImColor col = UI::ColorWithMultipliedValue(Colors::Theme::Selection, 0.9f);
				fillRowWithColour(UI::ColorWithMultipliedSaturation(col, 0.7f));
			}
		}
		else if (isRowHovered)
		{
			fillRowWithColour(Colors::Theme::GroupHeader);
		}
		else if (isAnyDescendantSelected(entity, isAnyDescendantSelected))
		{
			fillRowWithColour(Colors::Theme::SelectionMuted);
		}

		// Text colouring
		//---------------

		if (isSelected)
			ImGui::PushStyleColor(ImGuiCol_Text, Colors::Theme::BackgroundDark);

		const bool missingMesh = 
			// static model
			(entity.HasComponent<StaticModelComponent>() 
			&& (AssetManager::Get()->IsAssetHandleValid(entity.GetComponent<StaticModelComponent>()->Model)
			&& AssetManager::Get()->GetAsset<StaticModel>(entity.GetComponent<StaticModelComponent>()->Model) 
			&& AssetManager::Get()->GetAsset<StaticModel>(entity.GetComponent<StaticModelComponent>()->Model)->IsFlagSet(AssetFlag::Missing)))
			// Dynamic model
			|| (entity.HasComponent<DynamicModelComponent>()
			&& (AssetManager::Get()->IsAssetHandleValid(entity.GetComponent<DynamicModelComponent>()->Model)
				&& AssetManager::Get()->GetAsset<DynamicModel>(entity.GetComponent<DynamicModelComponent>()->Model)
				&& AssetManager::Get()->GetAsset<DynamicModel>(entity.GetComponent<DynamicModelComponent>()->Model)->IsFlagSet(AssetFlag::Missing)));

		if (missingMesh)
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.4f, 0.3f, 1.0f));

		bool isPrefab = entity.HasComponent<PrefabComponent>();
		if (isPrefab)
		{
			isPrefab = AssetManager::Get()->IsAssetHandleValid(entity.GetComponent<PrefabComponent>()->PrefabID);
			if (!isPrefab)
				entity.RemoveComponent<PrefabComponent>();
		}

		if (isPrefab && !isSelected)
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.32f, 0.7f, 0.87f, 1.0f));

		// Tree node
		//----------
		// TODO: clean up this mess
		ImGuiContext &g = *GImGui;
		auto &style = ImGui::GetStyle();
		const ImVec2 label_size = ImGui::CalcTextSize(*strID, nullptr, false);
		const ImVec2 padding = ((flags & ImGuiTreeNodeFlags_FramePadding)) ? style.FramePadding : ImVec2(style.FramePadding.x, ImMin(window->DC.CurrLineTextBaseOffset, style.FramePadding.y));
		const float text_offset_x = g.FontSize + padding.x * 2;           // Collapser arrow width + Spacing
		const float text_offset_y = ImMax(padding.y, window->DC.CurrLineTextBaseOffset);                    // Latch before ItemSize changes it
		const float text_width = g.FontSize + (label_size.x > 0.0f ? label_size.x + padding.x * 2 : 0.0f);  // Include collapser
		ImVec2 text_pos(window->DC.CursorPos.x + text_offset_x, window->DC.CursorPos.y + text_offset_y);
		const float arrow_hit_x1 = (text_pos.x - text_offset_x) - style.TouchExtraPadding.x;
		const float arrow_hit_x2 = (text_pos.x - text_offset_x) + (g.FontSize + padding.x * 2.0f) + style.TouchExtraPadding.x;
		const bool is_mouse_x_over_arrow = (g.IO.MousePos.x >= arrow_hit_x1 && g.IO.MousePos.x < arrow_hit_x2);

		bool previousState = ImGui::TreeNodeBehaviorIsOpen(ImGui::GetID(*strID));

		if (is_mouse_x_over_arrow && isRowClicked)
			ImGui::SetNextItemOpen(!previousState);

		if (!isSelected && isAnyDescendantSelected(entity, isAnyDescendantSelected))
			ImGui::SetNextItemOpen(true);

		const bool opened = UI::TreeNodeWithIcon(nullptr, ImGui::GetID(*strID), flags, name, nullptr);

		int32_t rowIndex = ImGui::TableGetRowIndex();
		if (rowIndex >= m_FirstSelectedRow && rowIndex <= m_LastSelectedRow && !SelectionManager::IsSelected(entity.GetUUID()))
		{
			SelectionManager::Select(s_ActiveSelectionContext, entity.GetUUID());
			m_SelectedEntityID = entity.GetUUID();
		}

		const std::string rightClickPopupID = fmt::format("{0}-ContextMenu", strID);

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem(rightClickPopupID.c_str()))
		{
			{
				UI::ScopedColor colText(ImGuiCol_Text, Colors::Theme::Text);
				UI::ScopedColorStack entitySelection(ImGuiCol_Header, Colors::Theme::GroupHeader,
													  ImGuiCol_HeaderHovered, Colors::Theme::GroupHeader,
													  ImGuiCol_HeaderActive, Colors::Theme::GroupHeader);

				if (!isSelected)
				{
					if (!Input::IsKeyPressed(HL_KEY_LEFT_CONTROL))
						SelectionManager::DeselectAll();

					SelectionManager::Select(s_ActiveSelectionContext, entity.GetUUID());
					m_SelectedEntityID = entity.GetUUID();
				}

				if (isPrefab)
				{
					if (ImGui::MenuItem("Update Prefab"))
					{
						AssetHandle prefabAssetHandle = entity.GetComponent<PrefabComponent>()->PrefabID;
						Ref<Prefab> prefab = AssetManager::Get()->GetAsset<Prefab>(prefabAssetHandle);
						if (prefab)
							prefab->Create(entity);
						else
							HL_CORE_ERROR("Prefab has invalid asset handle: {0}", prefabAssetHandle);
					}
				}

				DrawEntityCreateMenu(entity);

				if (ImGui::MenuItem("Delete"))
					entityDeleted = true;
			}

			ImGui::EndPopup();
		}

		// Type column
		//------------
		if (isRowClicked)
		{
			if (wasRowRightClicked)
			{
				ImGui::OpenPopup(rightClickPopupID.c_str());
			}
			else
			{
				if (!Input::IsKeyPressed(HL_KEY_LEFT_CONTROL) && !Input::IsKeyPressed(HL_KEY_LEFT_SHIFT))
				{
					SelectionManager::DeselectAll();
					SelectionManager::Select(s_ActiveSelectionContext, entity.GetUUID());
					m_SelectedEntityID = entity.GetUUID();
					m_FirstSelectedRow = rowIndex;
					m_LastSelectedRow = -1;
				}
				else if (Input::IsKeyPressed(HL_KEY_LEFT_SHIFT) && SelectionManager::GetSelectionCount(s_ActiveSelectionContext) > 0)
				{
					if (rowIndex < m_FirstSelectedRow)
					{
						m_LastSelectedRow = m_FirstSelectedRow;
						m_FirstSelectedRow = rowIndex;
					}
					else
					{
						m_LastSelectedRow = rowIndex;
					}
				}
				else
				{
					if (isSelected)
					{
						SelectionManager::Deselect(s_ActiveSelectionContext, entity.GetUUID());
					}
					else
					{
						SelectionManager::Select(s_ActiveSelectionContext, entity.GetUUID());
						m_SelectedEntityID = entity.GetUUID();
					}
				}
			}

			ImGui::FocusWindow(ImGui::GetCurrentWindow());
		}

		if (missingMesh)
			ImGui::PopStyleColor();

		if (isSelected)
			ImGui::PopStyleColor();


		// Drag & Drop
		//------------
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
		{
			const auto &selectedEntities = SelectionManager::GetSelections(s_ActiveSelectionContext);
			UUID entityID = entity.GetUUID();

			if (!SelectionManager::IsSelected(s_ActiveSelectionContext, entityID))
			{
				ImGui::TextUnformatted(*entity.Tag());
				ImGui::SetDragDropPayload("scene_entity_hierarchy", &entityID, 1 * sizeof(UUID));
			}
			else
			{
				for (const auto &selectedEntity : selectedEntities)
				{
					Entity e = m_Scene->FindEntityByUUID(selectedEntity);
					ImGui::TextUnformatted(*e.Tag());
				}

				ImGui::SetDragDropPayload("scene_entity_hierarchy", selectedEntities.data(), selectedEntities.size() * sizeof(UUID));
			}

			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("scene_entity_hierarchy", ImGuiDragDropFlags_AcceptNoDrawDefaultRect);

			if (payload)
			{
				size_t count = payload->DataSize / sizeof(UUID);

				for (size_t i = 0; i < count; i++)
				{
					UUID droppedEntityID = *(((UUID *)payload->Data) + i);
					Entity droppedEntity = m_Scene->FindEntityByUUID(droppedEntityID);
					m_Scene->ParentEntity(droppedEntity, entity);
				}
			}

			ImGui::EndDragDropTarget();
		}


		ImGui::TableNextColumn();
		if (isPrefab)
		{
			UI::ShiftCursorX(edgeOffset * 3.0f);

			if (isSelected)
				ImGui::PushStyleColor(ImGuiCol_Text, Colors::Theme::BackgroundDark);

			ImGui::TextUnformatted("Prefab");
			ImGui::PopStyleColor();
		}

		// Draw children
		//--------------

		if (opened)
		{
			for (auto child : entity.Children())
				DrawEntityNode(m_Scene->FindEntityByUUID(child), searchFilter);

			ImGui::TreePop();
		}

		// Defer deletion until end of node UI
		if (entityDeleted)
		{
			auto selectedEntities = SelectionManager::GetSelections(s_ActiveSelectionContext);
			for (auto entityID : selectedEntities)
				m_Scene->DestroyEntity(m_Scene->FindEntityByUUID(entityID));
		}
	}
}

