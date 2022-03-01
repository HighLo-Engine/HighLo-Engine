// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

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
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene> &scene, bool isWindow)
		: m_Scene(scene), m_IsWindow(isWindow)
	{
		m_TreeNodeRoot = Texture2D::LoadFromFile("assets/Resources/Icons/treeNode_root.png");
		m_TreeNodeChildren = Texture2D::LoadFromFile("assets/Resources/Icons/treeNode_with_children.png");
		m_TreeNodeChildrenOpened = Texture2D::LoadFromFile("assets/Resources/Icons/treeNode_with_children_opened.png");
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene> &scene)
	{
		m_Scene = scene;
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

		if (m_Scene)
		{
			{
				const float edgeOffset = 4.0f;
				static HLString searchedString = "";
				UI::ShiftCursorX(edgeOffset * 3.0f);
				UI::ShiftCursorY(edgeOffset * 2.0f);

				// ===============================================================================================
				// =========================================  Search bar  ========================================
				// ===============================================================================================

				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - edgeOffset * 3.0f);
				UI::Widgets::SearchWidget(searchedString);

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
										  | ImGuiTableFlags_NoBordersInBody;

					const int32 numCols = 3;
					ImGui::BeginTable(UI::GenerateID(), numCols, flags, ImVec2(ImGui::GetContentRegionAvail()));

					// If the user clicks anywhere on the scene hierarchy panel (except on a entity itself), deselect the currently selected entity
					if (m_SelectedEntity && ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
					{
						SetSelected({});
					}

					ImGuiTableFlags columnFlags = ImGuiTableColumnFlags_NoHeaderLabel | ImGuiTableColumnFlags_NoReorder | ImGuiTableColumnFlags_NoResize | ImGuiTableColumnFlags_NoHide | ImGuiTableColumnFlags_NoSort;
					ImGui::TableSetupColumn("", columnFlags); // Label
					ImGui::TableSetupColumn("", columnFlags); // Visibilty
					ImGui::TableSetupColumn("", columnFlags); // Materials and other extra settings

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
						UI::DrawUnderline(true, 0.0f, 5.0f);
					}
					
					// List
					{
						UI::ScopedColorStack entitySelection(ImGuiCol_Header, IM_COL32_DISABLE, ImGuiCol_HeaderHovered, IM_COL32_DISABLE, ImGuiCol_HeaderActive, IM_COL32_DISABLE);
						
						auto& view = m_Scene->m_Registry.View<RelationshipComponent>();

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
						if (ImGui::BeginMenu("Create"))
						{
							if (ImGui::MenuItem("Null Object"))
							{
								auto newEntity = m_Scene->CreateEntity("Null Object");
								SetSelected(newEntity);

								if (m_EntityAddedCallback)
									m_EntityAddedCallback(newEntity);
							}

							if (ImGui::MenuItem("Camera"))
							{
								auto newEntity = m_Scene->CreateEntity("Camera");
								newEntity.AddComponent<CameraComponent>();
								SetSelected(newEntity);

								if (m_EntityAddedCallback)
									m_EntityAddedCallback(newEntity);
							}

							ImGui::Separator();

							if (ImGui::MenuItem("Cube"))
							{
								auto newEntity = m_Scene->CreateEntity("Cube");
								StaticModelComponent *component = newEntity.AddComponent<StaticModelComponent>();
								component->Model = AssetFactory::CreateCube({ 1.0f, 1.0f, 1.0f });
								SetSelected(newEntity);

								if (m_EntityAddedCallback)
									m_EntityAddedCallback(newEntity);
							}

							if (ImGui::MenuItem("Sphere"))
							{
								auto newEntity = m_Scene->CreateEntity("Sphere");
								StaticModelComponent *component = newEntity.AddComponent<StaticModelComponent>();
								component->Model = AssetFactory::CreateSphere(4.0f);
								SetSelected(newEntity);

								if (m_EntityAddedCallback)
									m_EntityAddedCallback(newEntity);
							}

							if (ImGui::MenuItem("Capsule"))
							{
								auto newEntity = m_Scene->CreateEntity("Capsule");
								StaticModelComponent *component = newEntity.AddComponent<StaticModelComponent>();
								component->Model = AssetFactory::CreateCapsule(4.0f, 8.0f);
								SetSelected(newEntity);

								if (m_EntityAddedCallback)
									m_EntityAddedCallback(newEntity);
							}

							if (ImGui::MenuItem("Cylinder"))
							{
								auto newEntity = m_Scene->CreateEntity("Cylinder");
								StaticModelComponent *component = newEntity.AddComponent<StaticModelComponent>();
// TODO: Add Cylinders to AssetFactory and MeshFactory
							//	component->Model = AssetFactory::CreateCylinder();
								SetSelected(newEntity);

								if (m_EntityAddedCallback)
									m_EntityAddedCallback(newEntity);
							}

							ImGui::Separator();

							if (ImGui::MenuItem("Directional Light"))
							{
								auto newEntity = m_Scene->CreateEntity("Directional Light");
								newEntity.AddComponent<DirectionalLightComponent>();
								newEntity.Transform().FromRotation({ 80.0f, 10.0f, 0.0f });
								SetSelected(newEntity);

								if (m_EntityAddedCallback)
									m_EntityAddedCallback(newEntity);
							}

							if (ImGui::MenuItem("Point Light"))
							{
								auto newEntity = m_Scene->CreateEntity("Point Light");
								newEntity.AddComponent<PointLightComponent>();
								newEntity.Transform().FromPosition({ 0.0f, 0.0f, 0.0f });
								SetSelected(newEntity);

								if (m_EntityAddedCallback)
									m_EntityAddedCallback(newEntity);
							}

							if (ImGui::MenuItem("Sky Light"))
							{
								auto newEntity = m_Scene->CreateEntity("Sky Light");
								newEntity.AddComponent<SkyLightComponent>();
								SetSelected(newEntity);

								if (m_EntityAddedCallback)
									m_EntityAddedCallback(newEntity);
							}

							ImGui::EndMenu();
						}

						ImGui::EndPopup();
					}

					// Hide selected entity
					if (ImGui::IsWindowFocused() && m_SelectedEntity && Input::IsKeyPressed(HL_KEY_H))
					{
						if (m_SelectedEntity.IsHidden())
							m_SelectedEntity.Show();
						else
							m_SelectedEntity.Hide();
					}

					if (ImGui::IsWindowFocused() && (Input::IsKeyPressed(HL_KEY_DELETE) || Input::IsKeyPressed(HL_KEY_BACKSPACE)) && m_SelectedEntity)
						DeleteEntity(m_SelectedEntity);

					ImGui::EndTable();
				}
			}

			if (ImGui::BeginDragDropTargetCustom(windowRect, ImGui::GetCurrentWindow()->ID))
			{
				const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("scene_entity_hierarchy", ImGuiDragDropFlags_AcceptNoDrawDefaultRect);

				if (payload)
				{
					Entity &e = *(Entity*)payload->Data;
					m_Scene->UnparentEntity(e);
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

		const bool hasChildMatchingSearch = [&]
		{
			if (searchFilter.IsEmpty())
				return false;

			for (auto child : entity.Children())
			{
				Entity e = m_Scene->FindEntityByUUID(child);
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
					Entity childEntity = m_Scene->FindEntityByUUID(child);
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
			if (isWindowFocused || UI::NavigatedTo())
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


		bool isPrefab = entity.HasComponent<PrefabComponent>();

		// Text coloring
		if (isSelected)
			ImGui::PushStyleColor(ImGuiCol_Text, Colors::Theme::BackgroundDark);

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

		const bool opened = UI::TreeNodeWithIcon(nullptr, ImGui::GetID(strId.C_Str()), flags, name, nullptr);
		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			{
				UI::ScopedColor colorText(ImGuiCol_Text, Colors::Theme::Text);
				UI::ScopedColorStack entitySelection(ImGuiCol_Header, Colors::Theme::GroupHeader, ImGuiCol_HeaderHovered, Colors::Theme::GroupHeader, ImGuiCol_HeaderActive, Colors::Theme::GroupHeader);

				if (ImGui::MenuItem("Duplicate Entity"))
				{
					Entity newEntity = m_Scene->DuplicateEntity(m_SelectedEntity);
					m_Scene->AddEntity(newEntity);
					SetSelected(newEntity);
				}

				if (ImGui::MenuItem("Remove Entity"))
					entityDeleted = true;

				if (isPrefab)
				{
					if (ImGui::MenuItem("Update prefab"))
					{
						AssetHandle prefabAssetHandle = entity.GetComponent<PrefabComponent>()->PrefabID;
						Ref<Prefab> prefab = AssetManager::Get()->GetAsset<Prefab>(prefabAssetHandle);
						if (prefab)
							prefab->Create(entity);
						else
							HL_CORE_ERROR("Error: Prefab has invalid asset handle! {0}", prefabAssetHandle);
					}
				}
			}

			ImGui::EndPopup();
		}

		if (isRowClicked)
		{
			SetSelected(entity);
			ImGui::FocusWindow(ImGui::GetCurrentWindow());
		}

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
				m_Scene->ParentEntity(droppedEntity, entity);
			}

			ImGui::EndDragDropTarget();
		}

		ImGui::TableNextColumn();

		if (entity.IsHidden())
		{
			UI::ScopedColor textColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
			ImVec2 calculatedSize = UI::CenteredText(ICON_FA_TIMES);
		}
		else
		{
			UI::ScopedColor textColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
			ImVec2 calculatedSize = UI::CenteredText(ICON_FA_CHECK);
		}

		ImGui::TableNextColumn();

		UI::DrawText("CHECKED 2");

		// Draw all children
		if (opened)
		{
			for (auto child : entity.Children())
			{
				Entity e = m_Scene->FindEntityByUUID(child);
				if (e)
					DrawEntityNode(e, searchFilter);
			}

			ImGui::TreePop();
		}

		if (entityDeleted)
			DeleteEntity(entity);
	}
	
	void SceneHierarchyPanel::DeleteEntity(Entity entity)
	{
		if (m_EntityDeletedCallback)
			m_EntityDeletedCallback(entity);

		m_Scene->DestroyEntity(entity);
		if (entity == m_SelectedEntity)
		{
			if (m_SelectionChangedCallback)
				m_SelectionChangedCallback({});

			m_SelectedEntity = {};
		}
	}
}

