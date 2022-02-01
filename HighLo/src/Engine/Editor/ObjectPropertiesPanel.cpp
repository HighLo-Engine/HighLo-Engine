// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "ObjectPropertiesPanel.h"

#include "Engine/ImGui/ImGui.h"
#include "Engine/ImGui/ImGuiWidgets.h"
#include "Engine/Assets/AssetManager.h"

#include "Engine/Renderer/Renderer.h"

namespace highlo
{

	template<typename T, typename UIFunc>
	static void DrawComponent(const HLString &name, Entity entity, UIFunc func, const Ref<Texture2D> &settingsIcon, bool canBeRemoved = true)
	{
		if (entity.HasComponent<T>())
		{
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
				if (ImGui::MenuItem("Reset"))
					resetValues = true;

				if (canBeRemoved)
				{
					if (ImGui::MenuItem("Remove component"))
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

	template<typename UIFunc>
	static void DrawTransformComponent(const HLString &name, Entity entity, UIFunc func, const Ref<Texture2D> &settingsIcon)
	{
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
			if (ImGui::MenuItem("Reset"))
				resetValues = true;

			UI::EndPopup();
		}

		if (open)
		{
			func();
			ImGui::TreePop();
		}

		if (resetValues)
			entity.Transform().FromPosition({ 0.0f, 0.0f, 0.0f });

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
		if (m_IsWindow)
		{
			UI::ScopedStyle padding(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 10.0f));
			ImGui::Begin("Object Properties", pOpen);
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
		ImGui::AlignTextToFramePadding();

		auto id = entity.GetUUID();
		const char *addText = " ADD NEW COMPONENT            ";
		ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
		UI::ShiftCursor(4.0f, 4.0f);

		{
		#ifdef HL_DEBUG
			// Draw ID
			ImGui::SameLine(ImGui::GetContentRegionAvail().x - (ImGui::CalcTextSize(HLString::ToString(id)).x));
			ImGui::TextDisabled("%llx", id);
		#endif // HL_DEBUG

			// Draw Tag
			{
				auto &tag = entity.Tag();

				ImGui::PushItemWidth(contentRegionAvailable.x * 0.5f);

				UI::ScopedStyle frameBorder(ImGuiStyleVar_FrameBorderSize, 0.0f);
				UI::ScopedColor frameColor(ImGuiCol_FrameBg, IM_COL32(0, 0, 0, 0));
				UI::ScopedFont boldFont(ImGui::GetIO().Fonts->Fonts[0]);

				
				if (UI::DrawInputText("", tag))
				{
					entity.SetTag(tag);
					if (m_ActionCallback)
						m_ActionCallback(ObjectPropertiesActionType::TagChanged);
				}

				UI::DrawItemActivityOutline(2.0f, false, Colors::Theme::Accent);
				ImGui::PopItemWidth();
			}

			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImVec2 textSize = ImGui::CalcTextSize(addText);
			textSize.x += GImGui->Style.FramePadding.x * 2.0f;

			{
				UI::ScopedColorStack addComponentButtonColors(ImGuiCol_Button, IM_COL32(70, 70, 70, 200), ImGuiCol_ButtonHovered, IM_COL32(70, 70, 70, 255), ImGuiCol_ButtonActive, IM_COL32(70, 70, 70, 150));

				// align the Add Component on the right side
				ImGui::SameLine(ImGui::GetContentRegionAvail().x - (textSize.x + 4.0f + 10.0f));
				if (ImGui::Button(addText, ImVec2(textSize.x + 4.0f, lineHeight + 2.0f)))
					ImGui::OpenPopup("add_comp_popup");

				const float pad = 4.0f;
				const float iconWidth = ImGui::GetFrameHeight() - pad * 2.0f;
				const float iconHeight = iconWidth;
				ImVec2 iconPos = ImGui::GetItemRectMax();
				iconPos.x -= iconWidth + pad;
				iconPos.y -= iconHeight + pad;

				ImGui::SetCursorScreenPos(iconPos);
				UI::ShiftCursor(-5.0f, -1.0f);
				UI::Image(m_PlusIcon, ImVec2(iconWidth, iconHeight), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), ImColor(160, 160, 160, 255).Value);
			}
		}

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		{
			UI::ScopedFont boldFont(ImGui::GetIO().Fonts->Fonts[0]);

			if (UI::BeginPopup("add_comp_popup"))
			{
				if (!m_SelectedEntity.HasComponent<CameraComponent>())
				{
					if (ImGui::MenuItem("Camera"))
					{
						m_SelectedEntity.AddComponent<CameraComponent>();
						ImGui::CloseCurrentPopup();
					}
				}

				if (!m_SelectedEntity.HasComponent<StaticModelComponent>() && !m_SelectedEntity.HasComponent<DynamicModelComponent>())
				{
					if (ImGui::MenuItem("Static Model"))
					{
						m_SelectedEntity.AddComponent<StaticModelComponent>();
						ImGui::CloseCurrentPopup();
					}
				}

				if (!m_SelectedEntity.HasComponent<DynamicModelComponent>() && !m_SelectedEntity.HasComponent<StaticModelComponent>())
				{
					if (ImGui::MenuItem("Dynamic Model"))
					{
						m_SelectedEntity.AddComponent<DynamicModelComponent>();
						ImGui::CloseCurrentPopup();
					}
				}

				if (!m_SelectedEntity.HasComponent<DirectionalLightComponent>())
				{
					if (ImGui::MenuItem("Directional Light"))
					{
						m_SelectedEntity.AddComponent<DirectionalLightComponent>();
						ImGui::CloseCurrentPopup();
					}
				}

				if (!m_SelectedEntity.HasComponent<PointLightComponent>())
				{
					if (ImGui::MenuItem("Point Light"))
					{
						m_SelectedEntity.AddComponent<PointLightComponent>();
						ImGui::CloseCurrentPopup();
					}
				}

				if (!m_SelectedEntity.HasComponent<SkyLightComponent>())
				{
					if (ImGui::MenuItem("Sky Light"))
					{
						m_SelectedEntity.AddComponent<SkyLightComponent>();
						ImGui::CloseCurrentPopup();
					}
				}



				UI::EndPopup();
			}
		}

		// Draw Transform
		DrawTransformComponent("Transform", entity, [&]()
		{
			UI::ScopedStyle spacing(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
			UI::ScopedStyle padding(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 4.0f));

			ImGui::BeginTable("transformComponent", 2, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_NoClip);
			ImGui::TableSetupColumn("label_col", 0, 100.0f);
			ImGui::TableSetupColumn("value_col", ImGuiTableColumnFlags_IndentEnable | ImGuiTableColumnFlags_NoClip, ImGui::GetContentRegionAvail().x - 100.0f);

			ImGui::TableNextRow();
			UI::DrawVec3("Translation", entity.Transform().GetPosition());

			ImGui::TableNextRow();
			glm::vec3 rotation = entity.Transform().GetRotation();
			glm::vec3 degreeRotation = glm::degrees(rotation);
			UI::DrawVec3("Rotation", degreeRotation);
			entity.Transform().SetRotation(glm::radians(degreeRotation));

			ImGui::TableNextRow();
			UI::DrawVec3("Scale", entity.Transform().GetScale());

			ImGui::EndTable();

			UI::ShiftCursorY(-8.0f);
			UI::DrawUnderline();
			UI::ShiftCursorY(18.0f);
		}, m_SettingsIcon);

		DrawComponent<StaticModelComponent>("Static Model", entity, [](StaticModelComponent &component)
		{
			Ref<StaticModel> mesh = AssetManager::Get()->GetAsset<StaticModel>(component.Model);

			UI::BeginPropertyGrid();
			// TODO: Add mesh related references
			UI::EndPropertyGrid();

			if (mesh && mesh->IsValid())
			{
				if (UI::BeginTreeNode("Materials"))
				{
					UI::BeginPropertyGrid();

					auto &meshMaterialTable = mesh->GetMaterials();
					if (component.Materials->GetMaterialCount() < meshMaterialTable->GetMaterialCount())
						component.Materials->SetMaterialCount(meshMaterialTable->GetMaterialCount());

					for (uint32 i = 0; i < component.Materials->GetMaterialCount(); ++i)
					{
						Ref<MaterialAsset> meshMaterialAsset;
						HLString label = fmt::format("[Material {0}]", i);
						bool hasLocalMaterial = component.Materials->HasMaterial(i);
						bool hasMeshMaterial = meshMaterialTable->HasMaterial(i);

						if (i == meshMaterialTable->GetMaterialCount())
							ImGui::Separator();

						if (hasMeshMaterial)
							meshMaterialAsset = meshMaterialTable->GetMaterial(i);

						Ref<MaterialAsset> materialAsset = hasLocalMaterial ? component.Materials->GetMaterial(i) : meshMaterialAsset;
						if (materialAsset)
						{
							AssetHandle materialAssetHandle = materialAsset->Handle;

							if (hasLocalMaterial || !hasMeshMaterial)
							{
								// TODO
							}
							else
							{
								HLString meshMaterialName = meshMaterialAsset->GetMaterial()->GetName();
								if (meshMaterialName.IsEmpty())
									meshMaterialName = "Unnamed Material";
							}

							if (hasLocalMaterial)
							{
								ImGui::SameLine();
								float prevItemHeight = ImGui::GetItemRectSize().y;
								if (ImGui::Button("X", { prevItemHeight, prevItemHeight }))
									component.Materials->ClearMaterial(i);
								ImGui::NextColumn();
							}
						}
					}

					UI::EndPropertyGrid();
					UI::EndTreeNode();
				}
			}
		}, m_SettingsIcon);

		DrawComponent<DynamicModelComponent>("Dynamic Model", entity, [](DynamicModelComponent &component)
		{
			Ref<DynamicModel> mesh = AssetManager::Get()->GetAsset<DynamicModel>(component.Model);

			UI::BeginPropertyGrid();
			
			UI::PushItemDisabled();
			int32 index = (int32)component.SubmeshIndex;
			UI::DrawDragInt("SubmeshIndex", index);
			UI::PopItemDisabled();

			UI::DrawCheckbox("Fractured?", component.IsFractured);
			
			UI::EndPropertyGrid();

			if (mesh && mesh->IsValid())
			{
				if (UI::BeginTreeNode("Materials"))
				{
					UI::BeginPropertyGrid();

					auto &meshMaterialTable = mesh->GetMaterials();
					if (component.Materials->GetMaterialCount() < meshMaterialTable->GetMaterialCount())
						component.Materials->SetMaterialCount(meshMaterialTable->GetMaterialCount());

					for (uint32 i = 0; i < component.Materials->GetMaterialCount(); ++i)
					{
						Ref<MaterialAsset> meshMaterialAsset;
						HLString label = fmt::format("[Material {0}]", i);
						bool hasLocalMaterial = component.Materials->HasMaterial(i);
						bool hasMeshMaterial = meshMaterialTable->HasMaterial(i);

						if (i == meshMaterialTable->GetMaterialCount())
							ImGui::Separator();

						if (hasMeshMaterial)
							meshMaterialAsset = meshMaterialTable->GetMaterial(i);

						Ref<MaterialAsset> materialAsset = hasLocalMaterial ? component.Materials->GetMaterial(i) : meshMaterialAsset;
						if (materialAsset)
						{
							AssetHandle materialAssetHandle = materialAsset->Handle;

							if (hasLocalMaterial || !hasMeshMaterial)
							{
								// TODO
							}
							else
							{
								HLString meshMaterialName = meshMaterialAsset->GetMaterial()->GetName();
								if (meshMaterialName.IsEmpty())
									meshMaterialName = "Unnamed Material";
							}

							if (hasLocalMaterial)
							{
								ImGui::SameLine();
								float prevItemHeight = ImGui::GetItemRectSize().y;
								if (ImGui::Button("X", { prevItemHeight, prevItemHeight }))
									component.Materials->ClearMaterial(i);
								ImGui::NextColumn();
							}
						}
					}

					UI::EndPropertyGrid();
					UI::EndTreeNode();
				}
			}
		}, m_SettingsIcon);

		DrawComponent<CameraComponent>("Camera", entity, [&](CameraComponent &component)
		{
			UI::BeginPropertyGrid();

			std::vector<HLString> projectionTypes;
			projectionTypes.push_back("Perspective");
			projectionTypes.push_back("Orthographic");
			int32 currentProjectionType = (int32)component.Camera.GetCurrentProjectionType();

			bool primary = component.Primary;
			if (UI::DrawCheckbox("Primary Camera?", primary))
			{
				component.Primary = primary;
			}

			if (UI::DrawDropdown("Projection", projectionTypes, &currentProjectionType))
			{
				component.Camera.SetProjectionType((Camera::ProjectionType)currentProjectionType);
			}

			if (component.Camera.GetCurrentProjectionType() == Camera::ProjectionType::Orthographic)
			{
				float orthographicSize = component.Camera.GetOrthographicSize();
				float nearPlane = component.Camera.GetOrthographicNearPlane();
				float farPlane = component.Camera.GetOrthographicFarPlane();

				if (UI::DrawDragFloat("Size", orthographicSize))
				{
					component.Camera.SetOrthographicSize(orthographicSize);
				}

				glm::vec2 values = { nearPlane, farPlane };
				if (UI::DrawDragFloat2("Near and Far Planes", values))
				{
					component.Camera.SetOrthographicNearPlane(values[0]);
					component.Camera.SetOrthographicFarPlane(values[1]);
				}
			}
			else if (component.Camera.GetCurrentProjectionType() == Camera::ProjectionType::Perspective)
			{
				float verticalFOV = component.Camera.GetPerspectiveFOV();
				float nearPlane = component.Camera.GetPerspectiveNearPlane();
				float farPlane = component.Camera.GetPerspectiveFarPlane();

				if (UI::DrawDragFloat("Vertical FOV", verticalFOV))
				{
					component.Camera.SetPerspectiveFOV(verticalFOV);
				}

				glm::vec2 values = { nearPlane, farPlane };
				if (UI::DrawDragFloat2("Near and Far Planes", values))
				{
					component.Camera.SetPerspectiveNearPlane(values[0]);
					component.Camera.SetPerspectiveFarPlane(values[1]);
				}
			}

			UI::EndPropertyGrid();
		}, m_SettingsIcon);

		DrawComponent<DirectionalLightComponent>("Directional Light", entity, [](DirectionalLightComponent &component)
		{
			UI::BeginPropertyGrid();
			UI::DrawColorPicker("Radiance", component.Radiance);
			UI::DrawDragFloat("Intensity", component.Intensity);
			UI::DrawCheckbox("Cast Shadows", component.CastShadows);
			UI::DrawCheckbox("Soft Shadows", component.SoftShadows);
			UI::DrawDragFloat("Source Size", component.LightSize);
			UI::EndPropertyGrid();
		}, m_SettingsIcon);

		DrawComponent<PointLightComponent>("Point Light", entity, [](PointLightComponent &component)
		{
			UI::BeginPropertyGrid();
			UI::DrawColorPicker("Radiance", component.Radiance);
			UI::DrawDragFloat("Intensity", component.Intensity);
			UI::DrawDragFloat("Radius", component.Radius, 0.1f, 0.0f, std::numeric_limits<float>::max());
			UI::DrawDragFloat("Falloff", component.Falloff, 0.005f, 0.0f, 1.0f);
			UI::DrawCheckbox("Cast Shadows", component.CastShadows);
			UI::DrawCheckbox("Soft Shadows", component.SoftShadows);
			UI::DrawDragFloat("Source Size", component.LightSize);
			UI::EndPropertyGrid();
		}, m_SettingsIcon);

		DrawComponent<SkyLightComponent>("Sky Light", entity, [](SkyLightComponent &component)
		{
			UI::BeginPropertyGrid();
			UI::DrawDragFloat("Intensity", component.Intensity);
			if (AssetManager::Get()->IsAssetHandleValid(component.SceneEnvironment))
			{
				auto env = AssetManager::Get()->GetAsset<Environment>(component.SceneEnvironment);
				if (env && env->GetRadianceMap() != nullptr)
				{
					UI::DrawFloatSlider("Lod", component.Lod, 0.0f, static_cast<float>(env->GetRadianceMap()->GetSpecification().Mips));
				}
				else
				{
					UI::ScopedItemFlags flag(ImGuiItemFlags_Disabled);
					UI::DrawFloatSlider("Lod", component.Lod, 0.0f, 10.0f);
				}
			}

			ImGui::Separator();

			UI::DrawCheckbox("Dynamic Sky", component.DynamicSky);
			if (component.DynamicSky)
			{
				bool changed = UI::DrawDragFloat("Turbidity", component.TurbityAzimuthInclination.x, 0.01f);
				changed |= UI::DrawDragFloat("Azimuth", component.TurbityAzimuthInclination.y, 0.01f);
				changed |= UI::DrawDragFloat("Inclination", component.TurbityAzimuthInclination.z, 0.01f);

				if (changed)
				{
					Ref<Texture3D> preethamEnv = Renderer::CreatePreethamSky(component.TurbityAzimuthInclination.x, component.TurbityAzimuthInclination.y, component.TurbityAzimuthInclination.z);
					Ref<Environment> env = AssetManager::Get()->GetAsset<Environment>(component.SceneEnvironment);
					if (env)
					{
						env->SetRadianceMap(preethamEnv);
						env->SetIrradianceMap(preethamEnv);
					}
				}
				else
				{
					Ref<Texture3D> preethamEnv = Renderer::CreatePreethamSky(component.TurbityAzimuthInclination.x, component.TurbityAzimuthInclination.y, component.TurbityAzimuthInclination.z);
					component.SceneEnvironment = AssetManager::Get()->CreateMemoryOnlyAsset<Environment>(preethamEnv, preethamEnv, preethamEnv);
				}
			}

			UI::EndPropertyGrid();
		}, m_SettingsIcon);
	}
}

