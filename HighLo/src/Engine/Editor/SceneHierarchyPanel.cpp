// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "SceneHierarchyPanel.h"

#include "Engine/ImGui/imgui.h"
#include "Engine/Application/Application.h"
#include "EditorColors.h"

namespace highlo
{
	template<typename T, typename UIFunc>
	static void DrawComponent(const HLString &name, Entity entity, UIFunc func, bool canBeRemoved = true)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed 
			| ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

		if (entity.HasComponent<T>())
		{
			ImGui::PushID((void*)typeid(T).hash_code());
			auto &component = entity.GetComponent<T>();
			ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();
			bool removeComponent = false;
			bool resetValues = false;

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool opened = ImGui::TreeNodeEx("##todoMakeMeARealId", treeNodeFlags, *name);
			bool rightClicked = ImGui::IsItemClicked(ImGuiMouseButton_Right);

			ImGui::SameLine(contentRegionAvail.x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2(lineHeight, lineHeight)) || rightClicked)
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Reset"))
					resetValues = true;

				if (canBeRemoved)
				{
					if (ImGui::MenuItem("Remove Component"))
						removeComponent = true;
				}

				ImGui::EndPopup();
			}


			if (opened)
			{
				func(component);
				ImGui::TreePop();
			}

			if (removeComponent || resetValues)
				entity.RemoveComponent<T>();

			if (resetValues)
				entity.AddComponent<T>();

			ImGui::PopID();
		}
	}

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene> &scene)
		: m_Context(scene)
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

	void SceneHierarchyPanel::OnUIRender(bool window)
	{
		if (window)
			ImGui::Begin("Scene Hierarchy");

		ImRect windowRect = { ImGui::GetWindowContentRegionMin(), ImGui::GetContentRegionMax() };

		if (m_Context)
		{
			/*
			HLApplication::Get().GetECSRegistry().ForEachMultiple<IDComponent, RelationshipComponent>([=](Entity e, TransformComponent &transform, std::vector<void*> components)
			{
				if (e.GetParentUUID() == 0)
					DrawEntityNode(e);
			});
			*/

			if (ImGui::BeginDragDropTargetCustom(windowRect, ImGui::GetCurrentWindow()->ID))
			{
				const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("scene_entity_hierarchy", ImGuiDragDropFlags_AcceptNoDrawDefaultRect);
				if (payload)
				{
					Entity &entity = *(Entity*)payload->Data;
					m_Context->UnparentEntity(entity);
				}

				ImGui::EndDragDropTarget();
			}

			if (ImGui::BeginPopupContextWindow(0, 1, false))
			{
				if (ImGui::BeginMenu("Create"))
				{
					if (ImGui::MenuItem("Null Object"))
					{
						auto newEntity = m_Context->CreateEntity("Null Object");
						SetSelected(newEntity);
					}

					if (ImGui::MenuItem("Camera"))
					{
						auto newEntity = m_Context->CreateEntity("Camera");
						newEntity.AddComponent<CameraComponent>();
						SetSelected(newEntity);
					}



					ImGui::EndMenu();
				}

				ImGui::EndPopup();
			}
		}

		if (window)
			ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		HLString name = entity.Tag;

		ImGuiTreeNodeFlags flags = (entity == m_SelectedEntity ? ImGuiTreeNodeFlags_Selected : 0)
			| ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

		/*
		if (entity.Children().empty())
			flags |= ImGuiTreeNodeFlags_Leaf;
		*/

		// TODO: move this to another function and checks are missing (asset functions are not implemented yet)
		const bool missingMesh = entity.HasComponent<ModelComponent>();
		if (missingMesh)
			ImGui::PushStyleColor(ImGuiCol_Text, HL_EDITOR_ERROR_COLOR);

		const bool opened = ImGui::TreeNodeEx((void*)(uint64)(uint32)entity.ID, flags, *name);
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_None) && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
		{
			m_SelectedEntity = entity;
			if (m_SelectionChangedCallback)
				m_SelectionChangedCallback(m_SelectedEntity);
		}

		if (missingMesh)
			ImGui::PopStyleColor();

		// Delete Entity
		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		// Define Drag Drop source
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
		{
			ImGui::Text(entity.Tag.C_Str());
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

		if (opened)
		{
			/*
			for (auto child : entity.Children())
			{
				Entity e = m_Context->FindEntityByUUID(child->ID);
				if (e)
					DrawEntityNode(e);
			}
			*/
		}

		if (entityDeleted)
		{
			m_Context->DestroyEntity(entity);
			if (entity == m_SelectedEntity)
				m_SelectedEntity = {};

			m_EntityDeletedCallback(entity);
		}
	}

	void SceneHierarchyPanel::DrawEntityComponents(Entity entity)
	{
		ImGui::AlignTextToFramePadding();
		ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();

		// Render Tag
		auto &tag = entity.Tag;
		char buffer[256];
		memset(buffer, 0, 256);
		memcpy(buffer, *tag, tag.Length());
		ImGui::PushItemWidth(contentRegionAvail.x * 0.5f);
		if (ImGui::InputText("##Tag", buffer, 256))
		{
			tag = HLString(buffer);
		}
		ImGui::PopItemWidth();

		// Render ID
		ImGui::SameLine();
		ImGui::TextDisabled("%u", entity.ID);
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 textSize = ImGui::CalcTextSize("Add Component");
		ImGui::SameLine(contentRegionAvail.x - (textSize.x + GImGui->Style.FramePadding.y));
		if (ImGui::Button("Add Component"))
		{
			ImGui::OpenPopup("AddComponentPanel");
		}

		if (ImGui::BeginPopup("AddComponentPanel"))
		{
			// TODO
			std::vector<HLString> options =
			{
				"Test",
				"Test 2"
			};

			UI::DrawDropdown("Add Component", options, 2, 0);

			ImGui::EndPopup();
		}


	}
}

