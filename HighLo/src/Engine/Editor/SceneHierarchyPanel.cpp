// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "SceneHierarchyPanel.h"

#include "Engine/ImGui/imgui.h"
#include "Engine/Application/Application.h"

namespace highlo
{
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
		}

		if (window)
			ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{

	}

	void SceneHierarchyPanel::DrawEntityComponents(Entity entity)
	{

	}
}

