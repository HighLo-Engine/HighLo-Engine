// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "SceneHierarchyPanel.h"

#include "Engine/Assets/AssetManager.h"
#include "Engine/ImGui/imgui.h"
#include "Engine/ImGui/ImGuiWidgets.h"
#include "Engine/Application/Application.h"
#include "EditorColors.h"

namespace highlo
{
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene> &scene, bool isWindow)
		: m_Context(scene), m_IsWindow(isWindow)
	{
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene> &scene)
	{
		m_Context = scene;
		m_SelectedEntity = {};
	}

	void SceneHierarchyPanel::SetSelected(Entity entity)
	{
		m_SelectedEntity = entity;
		if (m_SelectionChangedCallback)
			m_SelectionChangedCallback(entity);
	}

	void SceneHierarchyPanel::OnUIRender(bool *pOpen)
	{
		if (m_IsWindow)
		{
			UI::ScopedStyle padding(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::Begin("Scene Hierarchy", pOpen);
		}

		ImRect windowRect = { ImGui::GetWindowContentRegionMin(), ImGui::GetWindowContentRegionMax() };

		if (m_Context)
		{
			{
				const float edgeOffset = 4.0f;
				static HLString searchedString;
				UI::ShiftCursorX(edgeOffset * 3.0f);
				UI::ShiftCursorY(edgeOffset * 2.0f);

				// ===============================================================================================
				// =========================================  Search bar  ========================================
				// ===============================================================================================

				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - edgeOffset * 3.0f);
			//	UI::Widgets::SearchWidget(searchedString);

				ImGui::Spacing();
				ImGui::Spacing();

				// ===============================================================================================
				// =====================================  Entity List  ===========================================
				// ===============================================================================================

				UI::ScopedStyle cellPadding(ImGuiStyleVar_CellPadding, ImVec2(4.0f, 0.0f));

				const ImU32 colRowAlt = UI::ColorWithMultipliedValue(Colors::Theme::BackgroundDark, 1.3f);
				UI::ScopedColor tableBGAlt(ImGuiCol_TableRowBgAlt, colRowAlt);

				// Table
				{
					UI::ScopedColor tableBg(ImGuiCol_ChildBg, Colors::Theme::BackgroundDark);

					ImGuiTableFlags flags = ImGuiTableFlags_NoPadInnerX
										  | ImGuiTableFlags_Resizable
										  | ImGuiTableFlags_Reorderable
										  | ImGuiTableFlags_ScrollY
										  | ImGuiTableFlags_RowBg
										  | ImGuiTableFlags_Sortable;

					const int32 numCols = 3;
					ImGui::BeginTable(UI::GenerateID(), numCols, flags, ImVec2(ImGui::GetContentRegionAvail()));

					ImGui::TableSetupColumn("Label");
					ImGui::TableSetupColumn("Type");
					ImGui::TableSetupColumn("Visibility");

					// Headers
					{
						const ImU32 colorActive = UI::ColorWithMultipliedValue(Colors::Theme::GroupHeader, 1.2f);
						UI::ScopedColorStack headerColors(ImGuiCol_HeaderHovered, colorActive, ImGuiCol_HeaderActive, colorActive);
						
						ImGui::TableSetupScrollFreeze(ImGui::TableGetColumnCount(), 1);

						ImGui::TableNextRow(ImGuiTableRowFlags_Headers, 22.0f);
						for (int32 column = 0; column < ImGui::TableGetColumnCount(); ++column)
						{
							ImGui::TableSetColumnIndex(column);
							const char *columnName = ImGui::TableGetColumnName(column);
							UI::ScopedID columnId(column);

							UI::ShiftCursor(edgeOffset * 3.0f, edgeOffset * 2.0f);
							ImGui::TableHeader(columnName);
							UI::ShiftCursor(-edgeOffset * 3.0f, -edgeOffset * 2.0f);
						}

						ImGui::SetCursorPosX(ImGui::GetCurrentTable()->OuterRect.Min.x);
						// TODO: add DrawUnderLine UI function and call here
					}
					
					// List
					{
						UI::ScopedColorStack entitySelection(ImGuiCol_Header, IM_COL32_DISABLE, ImGuiCol_HeaderHovered, IM_COL32_DISABLE, ImGuiCol_HeaderActive, IM_COL32_DISABLE);
						
						auto& view = m_Context->m_Registry.View<IDComponent, RelationshipComponent>();

						for (UUID entityId : view)
						{
							Entity e = m_Context->FindEntityByUUID(entityId);

							if (!e)
								continue;

							// If the parent UUID is 0, there is no parent -> this node is a root node
							if (e.GetParentUUID() == 0)
								DrawEntityNode(e, searchedString);
						}
					}

					if (ImGui::BeginPopupContextWindow(nullptr, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
					{
						if (ImGui::BeginMenu("Create"))
						{
							if (ImGui::MenuItem("Null Entity"))
							{
								auto newEntity = m_Context->CreateEntity("Null Entity");
								SetSelected(newEntity);
							}

							ImGui::Separator();

							if (ImGui::MenuItem("Directional Light"))
							{

							}

							if (ImGui::MenuItem("Point Light"))
							{

							}

							if (ImGui::MenuItem("Sky Light"))
							{

							}

							ImGui::EndMenu();
						}

						ImGui::EndPopup();
					}

					ImGui::EndTable();
				}
			}

			if (ImGui::BeginDragDropTargetCustom(windowRect, ImGui::GetCurrentWindow()->ID))
			{
				const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("scene_entity_hierarchy", ImGuiDragDropFlags_AcceptNoDrawDefaultRect);

				if (payload)
				{
					Entity &e = *(Entity*)payload->Data;
					m_Context->UnparentEntity(e);
				}

				ImGui::EndDragDropTarget();
			}
		}

		if (m_IsWindow)
			ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity, const HLString &searchFilter)
	{
		const char *name = entity.Tag().C_Str();
		HL_CORE_TRACE("DRAWING ENTITY {0}", name);

		const bool hasChildMatchingSearch = [&]
		{
			if (searchFilter.IsEmpty())
				return false;

			for (auto child : entity.Children())
			{
				Entity e = m_Context->FindEntityByUUID(child);
				if (UI::IsMatchingSearch(e.Tag(), searchFilter))
				{
					return true;
				}
			}

			return false;
		}();

		if (!UI::IsMatchingSearch(name, searchFilter) && !hasChildMatchingSearch)
			return;

		const float edgeOffset = 4.0f;
		const float rowHeight = 21.0f;

		auto *window = ImGui::GetCurrentWindow();
		window->DC.CurrLineSize.y = rowHeight;
		ImGui::TableNextRow(0, rowHeight);

		ImGui::TableNextColumn();
		window->DC.CurrLineTextBaseOffset = 3.0f;

		const ImVec2 rowAreaMin = ImGui::TableGetCellBgRect(ImGui::GetCurrentTable(), 0).Min;
		const ImVec2 rowAreaMax = {
			ImGui::TableGetCellBgRect(ImGui::GetCurrentTable(), ImGui::TableGetColumnCount() - 1).Max.x,
			rowAreaMin.y + rowHeight
		};

		const bool isSelected = entity == m_SelectedEntity;
		ImGuiTreeNodeFlags flags = (isSelected ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

		if (hasChildMatchingSearch)
			flags |= ImGuiTreeNodeFlags_DefaultOpen;

		if (entity.Children().empty())
			flags |= ImGuiTreeNodeFlags_Leaf;

		const HLString strId = HLString(name) + HLString::ToString((uint64)entity.GetUUID());

		ImGui::PushClipRect(rowAreaMin, rowAreaMax, false);
		bool isRowHovered;
		bool held;
		bool isRowClicked = ImGui::ButtonBehavior(ImRect(rowAreaMin, rowAreaMax), ImGui::GetID(strId.C_Str()), &isRowHovered, &held, ImGuiButtonFlags_AllowItemOverlap |ImGuiButtonFlags_PressedOnClickRelease);
		ImGui::SetItemAllowOverlap();
		ImGui::PopClipRect();

		const bool isWindowFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

		// Row coloring
		auto isAnyDescendantSelected = [&](Entity e, auto isAnyDescendantSelected) -> bool
		{
			if (e == m_SelectedEntity)
				return true;

			if (!e.Children().empty())
			{
				for (auto &child : e.Children())
				{
					Entity childEntity = m_Context->FindEntityByUUID(child);
					if (isAnyDescendantSelected(childEntity, isAnyDescendantSelected))
						return true;
				}
			}

			return false;
		};

		auto fillRowWithColor = [&](const ImColor &color)
		{
			for (int32 column = 0; column < ImGui::TableGetColumnCount(); ++column)
			{
				ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, color, column);
			}
		};

		if (isSelected)
		{
			if (isWindowFocused) // || UI::NavigatedTo())
			{
				fillRowWithColor(Colors::Theme::Selection);
			}
			else
			{
				const ImColor col = UI::ColorWithMultipliedValue(Colors::Theme::Selection, 0.9f);
				fillRowWithColor(col);
			}
		}
		else if (isRowHovered)
		{
			fillRowWithColor(Colors::Theme::GroupHeader);
		}
		else if (isAnyDescendantSelected(entity, isAnyDescendantSelected))
		{
			fillRowWithColor(Colors::Theme::SelectionMuted);
		}



		// Text coloring
		if (isSelected)
			ImGui::PushStyleColor(ImGuiCol_Text, Colors::Theme::BackgroundDark);

		bool missingMesh = false;
		/*missingMesh = entity.HasComponent<StaticModelComponent>()
			&& AssetManager::Get()->IsAssetHandleValid(entity.GetComponent<StaticModelComponent>()->Model->Handle)
			&& AssetManager::Get()->GetAsset<StaticModel>(entity.GetComponent<StaticModelComponent>()->Model->Handle)
			&& AssetManager::Get()->GetAsset<StaticModel>(entity.GetComponent<StaticModelComponent>()->Model->Handle)->IsFlagSet(AssetFlag::Missing);*/

		if (missingMesh)
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.4f, 0.3f, 1.0f));

		bool isPrefab = entity.HasComponent<PrefabComponent>();
		if (isPrefab && !isSelected)
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.32f, 0.7f, 0.87f, 1.0f));

		ImGuiContext &g = *GImGui;
		auto &style = ImGui::GetStyle();
		const ImVec2 labelSize = ImGui::CalcTextSize(strId.C_Str(), nullptr, false);
		const ImVec2 padding = ((flags & ImGuiTreeNodeFlags_FramePadding)) ? style.FramePadding : ImVec2(style.FramePadding.x, ImMin(window->DC.CurrLineTextBaseOffset, style.FramePadding.y));
		const float textOffsetX = g.FontSize + padding.x * 2.0f;
		const float textOffsetY = ImMax(padding.y, window->DC.CurrLineTextBaseOffset);
		const float textWidth = g.FontSize + (labelSize.x > 0.0f ? labelSize.x + padding.x * 2.0f : 0.0f);
		ImVec2 textPos(window->DC.CursorPos.x + textOffsetX, window->DC.CursorPos.y + textOffsetY);

		const float arrowHitX1 = (textPos.x - textOffsetX) - style.TouchExtraPadding.x;
		const float arrowHitX2 = (textPos.x - textOffsetX) + (g.FontSize + padding.x * 2.0f) + style.TouchExtraPadding.x;
		const bool isMouseXOverArrow = (g.IO.MousePos.x >= arrowHitX1 && g.IO.MousePos.y < arrowHitX2);
		bool prevState = ImGui::TreeNodeBehaviorIsOpen(ImGui::GetID(strId.C_Str()));

		if (isMouseXOverArrow && isRowClicked)
			ImGui::SetNextItemOpen(!prevState);

		const bool opened = false; // ImGui::TreeNodeWithIcon(nullptr, ImGui::GetID(strId.C_Str()), flags, name, nullptr);
		bool entityDeleted = false;
		/*if (ImGui::BeginPopupContextItem())
		{


			ImGui::EndPopup();
		}*/

		if (isRowClicked)
		{
			m_SelectedEntity = entity;
			if (m_SelectionChangedCallback)
				m_SelectionChangedCallback(m_SelectedEntity);

			ImGui::FocusWindow(ImGui::GetCurrentWindow());
		}

		if (missingMesh)
			ImGui::PopStyleColor();

		if (isSelected)
			ImGui::PopStyleColor();

		// Drag && drop
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
		{
			ImGui::Text(entity.Tag().C_Str());
			ImGui::SetDragDropPayload("scene_entity_hierarchy", &entity, sizeof(Entity));
			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("scene_entity_hierarchy", ImGuiDragDropFlags_AcceptNoDrawDefaultRect);
			if (payload)
			{
				Entity &droppedEntity = *(Entity*)payload->Data;
				m_Context->ParentEntity(droppedEntity, entity);
			}

			ImGui::EndDragDropTarget();
		}

		ImGui::TableNextColumn();

		if (isPrefab)
		{
			UI::ShiftCursorY(edgeOffset * 3.0f);
			if (isSelected)
				ImGui::PushStyleColor(ImGuiCol_Text, Colors::Theme::BackgroundDark);

			ImGui::TextUnformatted("Prefab");
			ImGui::PopStyleColor();
		}

		// Draw all children
		if (opened)
		{
			for (auto child : entity.Children())
			{
				Entity e = m_Context->FindEntityByUUID(child);
				if (e)
					DrawEntityNode(e, searchFilter);
			}

			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			m_Context->DestroyEntity(entity);
			if (entity == m_SelectedEntity)
				m_SelectedEntity = {};

			if (m_EntityDeletedCallback)
				m_EntityDeletedCallback(entity);
		}
	}
}

