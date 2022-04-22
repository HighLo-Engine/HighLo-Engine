// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "ObjectPropertiesPanel.h"

#include "Engine/ImGui/ImGui.h"
#include "Engine/ImGui/ImGuiWidgets.h"
#include "Engine/ImGui/ImGuiAsset.h"
#include "Engine/Assets/AssetManager.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Application/Application.h"

namespace highlo
{
	template<typename T, typename UIFunc>
	static void DrawComponent(const HLString &name, Entity entity, UIFunc func, const Ref<Texture2D> &settingsIcon, bool canBeRemoved = true)
	{
		if (entity.HasComponent<T>())
		{
			Translation *translation = HLApplication::Get().GetActiveTranslation();
			HL_ASSERT(translation);

			ImGui::PushID((void*)typeid(T).hash_code());
			auto *component = entity.GetComponent<T>();
			ImVec2 contentRegion = ImGui::GetContentRegionAvail();

			bool open = UI::PropertyGridHeader(name);
			bool rightClicked = ImGui::IsItemClicked(ImGuiMouseButton_Right);
			bool resetValues = false;
			bool removeComponent = false;
			float lineHeight = ImGui::GetItemRectMax().y - ImGui::GetItemRectMin().y;

			ImGui::SameLine(contentRegion.x - lineHeight);
			if (ImGui::InvisibleButton("##options", ImVec2(lineHeight, lineHeight)) || rightClicked)
			{
				ImGui::OpenPopup("settings_comp_popup");
			}

			UI::DrawButtonImage(settingsIcon, IM_COL32(160, 160, 160, 200), IM_COL32(160, 160, 160, 255), IM_COL32(160, 160, 160, 150), UI::RectExpanded(UI::GetItemRect(), -6.0f, -6.0f));

			if (UI::BeginPopup("settings_comp_popup"))
			{
				if (ImGui::MenuItem(translation->GetText("object-properties-reset-component")))
					resetValues = true;

				if (canBeRemoved)
				{
					if (ImGui::MenuItem(translation->GetText("object-properties-remove-component")))
						removeComponent = true;
				}

				UI::EndPopup();
			}

			if (open)
			{
				func(*component);
				ImGui::TreePop();
			}

			if (removeComponent || resetValues)
				entity.RemoveComponent<T>();

			if (resetValues)
				entity.AddComponent<T>();

			if (!open)
				UI::ShiftCursorY(-(ImGui::GetStyle().ItemSpacing.y + 1.0f));

			ImGui::PopID();
		}
	}

	template<typename UIFunction>
	static void DrawTransformComponent(const HLString &name, Ref<Scene> &scene, Entity entity, UIFunction func, const Ref<Texture2D> &settingsIcon)
	{
		Translation *translation = HLApplication::Get().GetActiveTranslation();
		HL_ASSERT(translation);

		ImGui::PushID(HLString("##" + name).C_Str());
		ImVec2 contentRegion = ImGui::GetContentRegionAvail();

		bool open = UI::PropertyGridHeader(name);
		bool rightClicked = ImGui::IsItemClicked(ImGuiMouseButton_Right);
		bool resetValues = false;
		float lineHeight = ImGui::GetItemRectMax().y - ImGui::GetItemRectMin().y;

		ImGui::SameLine(contentRegion.x - lineHeight);
		if (ImGui::InvisibleButton("##options", ImVec2(lineHeight, lineHeight)) || rightClicked)
		{
			ImGui::OpenPopup("settings_comp_popup");
		}

		UI::DrawButtonImage(settingsIcon, IM_COL32(160, 160, 160, 200), IM_COL32(160, 160, 160, 255), IM_COL32(160, 160, 160, 150), UI::RectExpanded(UI::GetItemRect(), -6.0f, -6.0f));

		if (UI::BeginPopup("settings_comp_popup"))
		{
			if (ImGui::MenuItem(translation->GetText("object-properties-reset-component")))
				resetValues = true;

			UI::EndPopup();
		}

		if (open)
		{
			func();
			ImGui::TreePop();
		}

		if (resetValues)
		{
			entity.SetTransform(Transform::Identity());
		}

		if (!open)
			UI::ShiftCursorY(-(ImGui::GetStyle().ItemSpacing.y + 1.0f));

		ImGui::PopID();
	}

	ObjectPropertiesPanel::ObjectPropertiesPanel(const Ref<Scene> &scene, bool isWindow)
		: m_Scene(scene), m_IsWindow(isWindow)
	{
		m_PlusIcon = Texture2D::LoadFromFile("assets/Resources/Icons/plus.png");
		m_SettingsIcon = Texture2D::LoadFromFile("assets/Resources/Icons/settings.png");
	}

	ObjectPropertiesPanel::~ObjectPropertiesPanel()
	{
	}

	void ObjectPropertiesPanel::SetContext(const Ref<Scene> &scene)
	{
		m_Scene = scene;
		m_SelectedEntity = {};
	}

	void ObjectPropertiesPanel::SetSelected(Entity entity)
	{
		m_SelectedEntity = entity;
	}

	void ObjectPropertiesPanel::OnUIRender(bool *pOpen)
	{
		Translation *translation = HLApplication::Get().GetActiveTranslation();

		if (m_IsWindow)
		{
			UI::ScopedStyle padding(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 10.0f));
			ImGui::Begin(translation->GetText("object-properties-window-title"), pOpen, ImGuiWindowFlags_AlwaysVerticalScrollbar);
		}

		if (m_SelectedEntity)
		{
			DrawComponents(m_SelectedEntity);
			m_Scene->UpdateEntity(m_SelectedEntity);
		}

		if (m_IsWindow)
			ImGui::End();
	}

	void ObjectPropertiesPanel::AddActionCallback(const std::function<void(ObjectPropertiesActionType)> &callback)
	{
		m_ActionCallback = callback;
	}
	
	void ObjectPropertiesPanel::DrawComponents(Entity &entity)
	{
		Translation *translation = HLApplication::Get().Get
