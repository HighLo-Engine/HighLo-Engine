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
		Translation *translation = HLApplication::Get().GetActiveTranslation();

		ImGui::AlignTextToFramePadding();

		auto id = entity.GetUUID();

		const char *addText = " ADD NEW COMPONENT            ";
		if (translation->GetLanguageCode() == "de-DE")
		{
			addText = u8" Komponent hinzufügen            ";
		}

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

					if (m_SelectionChangedCallback)
						m_SelectionChangedCallback(entity);
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
				if (!entity.HasComponent<CameraComponent>())
				{
					if (ImGui::MenuItem(translation->GetText("object-properties-camera-component")))
					{
						entity.AddComponent<CameraComponent>();
						ImGui::CloseCurrentPopup();
					}
				}

				if (!entity.HasComponent<StaticModelComponent>() && !m_SelectedEntity.HasComponent<DynamicModelComponent>())
				{
					if (ImGui::MenuItem(translation->GetText("object-properties-static-model-component")))
					{
						entity.AddComponent<StaticModelComponent>();
						ImGui::CloseCurrentPopup();
					}
				}

				if (!entity.HasComponent<DynamicModelComponent>() && !m_SelectedEntity.HasComponent<StaticModelComponent>())
				{
					if (ImGui::MenuItem(translation->GetText("object-properties-dynamic-model-component")))
					{
						entity.AddComponent<DynamicModelComponent>();
						ImGui::CloseCurrentPopup();
					}
				}

				if (!entity.HasComponent<DirectionalLightComponent>())
				{
					if (ImGui::MenuItem(translation->GetText("object-properties-directional-light-component")))
					{
						entity.AddComponent<DirectionalLightComponent>();
						ImGui::CloseCurrentPopup();
					}
				}

				if (!entity.HasComponent<PointLightComponent>())
				{
					if (ImGui::MenuItem(translation->GetText("object-properties-point-light-component")))
					{
						entity.AddComponent<PointLightComponent>();
						ImGui::CloseCurrentPopup();
					}
				}

				if (!entity.HasComponent<SkyLightComponent>())
				{
					if (ImGui::MenuItem(translation->GetText("object-properties-sky-light-component")))
					{
						entity.AddComponent<SkyLightComponent>();
						ImGui::CloseCurrentPopup();
					}
				}

				UI::EndPopup();
			}
		}

		// Draw Transform
		DrawTransformComponent(translation->GetText("object-properties-transform-component"), m_Scene, entity, [&]()
		{
			UI::ScopedStyle spacing(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
			UI::ScopedStyle padding(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 4.0f));

			ImGui::BeginTable("transformComponent", 2, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_NoClip);
			ImGui::TableSetupColumn("label_col", 0, 100.0f);
			ImGui::TableSetupColumn("value_col", ImGuiTableColumnFlags_IndentEnable | ImGuiTableColumnFlags_NoClip, ImGui::GetContentRegionAvail().x - 100.0f);

			Transform &entityTransform = entity.Transform();

			ImGui::TableNextRow();
			bool translationChanged = UI::DrawVec3("Translation", entityTransform.GetPosition());

			ImGui::TableNextRow();
			glm::vec3 rotation = entityTransform.GetRotation();
			glm::vec3 degreeRotation = glm::degrees(rotation);
			bool rotationChanged = UI::DrawVec3("Rotation", degreeRotation);
			entityTransform.SetRotation(glm::radians(degreeRotation));

			ImGui::TableNextRow();
			bool scaleChanged = UI::DrawVec3(translation->GetText("object-properties-transform-component-scale"), entityTransform.GetScale(), 1.0f);

			ImGui::EndTable();

			UI::ShiftCursorY(-8.0f);
			UI::DrawUnderline();
			UI::ShiftCursorY(18.0f);

			if (translationChanged || rotationChanged || scaleChanged)
			{
				entity.SetTransform(entityTransform);

				if (m_SelectionChangedCallback)
					m_SelectionChangedCallback(entity);
			}

		}, m_SettingsIcon);

		DrawComponent<StaticModelComponent>(translation->GetText("object-properties-static-model-component"), entity, [&](StaticModelComponent &component)
		{
			Ref<StaticModel> mesh = AssetManager::Get()->GetAsset<StaticModel>(component.Model);

			UI::BeginPropertyGrid();
			UI::AssetReferenceResult result;
			if (UI::DrawAssetReferenceWithConversion<StaticModel, MeshFile>(translation->GetText("object-properties-static-model-component"), component.Model, [=](Ref<MeshFile> meshAsset)
			{
				// TODO: Add MeshConversionCallback that is going to be called here
			}, &result))
			{
				// TODO: When we have mesh colliders we could set them here
			}

			if (result == UI::AssetReferenceResult::InvalidMetaData)
			{
				// TODO: Add InvalidMetaCallback that is going to be called here
			}

			UI::EndPropertyGrid();

			if (mesh && mesh->IsValid())
			{
				if (UI::BeginTreeNode(translation->GetText("object-properties-materials")))
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
									meshMaterialName = translation->GetText("object-properties-unnamed-material");
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

		DrawComponent<DynamicModelComponent>(translation->GetText("object-properties-dynamic-model-component"), entity, [&](DynamicModelComponent &component)
		{
			Ref<DynamicModel> mesh = AssetManager::Get()->GetAsset<DynamicModel>(component.Model);

			UI::BeginPropertyGrid();
			
			UI::AssetReferenceResult result;
			if (UI::DrawAssetReferenceWithConversion<StaticModel, MeshFile>(translation->GetText("object-properties-dynamic-model-component"), component.Model, [=](Ref<MeshFile> meshAsset)
			{
				// TODO: Add MeshConversionCallback that is going to be called here
			}, &result))
			{
				// TODO: When we have mesh colliders we could set them here
			}

			if (result == UI::AssetReferenceResult::InvalidMetaData)
			{
				// TODO: Add InvalidMetaCallback that is going to be called here
			}

			if (mesh)
			{
				if (UI::DrawUIntSlider("Submesh index", component.SubmeshIndex, 0, (int32)(mesh->Get()->GetSubmeshes().size() - 1)))
				{
					component.SubmeshIndex = glm::clamp<uint32>(component.SubmeshIndex, 0, (uint32)(mesh->Get()->GetSubmeshes().size() - 1));
				}

				UI::DrawCheckbox("Fractured?", component.IsFractured);
			}

			UI::EndPropertyGrid();

			if (mesh && mesh->IsValid())
			{
				if (UI::BeginTreeNode(translation->GetText("object-properties-materials")))
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
									meshMaterialName = translation->GetText("object-properties-unnamed-material");
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

		DrawComponent<CameraComponent>(translation->GetText("object-properties-camera-component"), entity, [&](CameraComponent &component)
		{
			UI::BeginPropertyGrid();

			std::vector<HLString> projectionTypes;
			projectionTypes.push_back(translation->GetText("object-properties-camera-component-perspective"));
			projectionTypes.push_back(translation->GetText("object-properties-camera-component-orthographic"));
			int32 currentProjectionType = (int32)component.Camera.GetCurrentProjectionType();

			bool primary = component.Primary;
			if (UI::DrawCheckbox(translation->GetText("object-properties-camera-component-primary"), primary))
			{
				component.Primary = primary;
			}

			if (UI::DrawDropdown(translation->GetText("object-properties-camera-component-projection"), projectionTypes, &currentProjectionType))
			{
				component.Camera.SetProjectionType((Camera::ProjectionType)currentProjectionType);
			}

			if (component.Camera.GetCurrentProjectionType() == Camera::ProjectionType::Orthographic)
			{
				float orthographicSize = component.Camera.GetOrthographicSize();
				float nearPlane = component.Camera.GetOrthographicNearPlane();
				float farPlane = component.Camera.GetOrthographicFarPlane();

				if (UI::DrawDragFloat(translation->GetText("object-properties-camera-component-orthographic-size"), orthographicSize))
				{
					component.Camera.SetOrthographicSize(orthographicSize);
				}

				glm::vec2 values = { nearPlane, farPlane };
				if (UI::DrawDragFloat2(translation->GetText("object-properties-camera-component-near-far-planes"), values))
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

				if (UI::DrawDragFloat(translation->GetText("object-properties-camera-component-vertical-fov"), verticalFOV))
				{
					component.Camera.SetPerspectiveFOV(verticalFOV);
				}

				glm::vec2 values = { nearPlane, farPlane };
				if (UI::DrawDragFloat2(translation->GetText("object-properties-camera-component-near-far-planes"), values))
				{
					component.Camera.SetPerspectiveNearPlane(values[0]);
					component.Camera.SetPerspectiveFarPlane(values[1]);
				}
			}

			UI::EndPropertyGrid();
		}, m_SettingsIcon);

		DrawComponent<DirectionalLightComponent>(translation->GetText("object-properties-directional-light-component"), entity, [&](DirectionalLightComponent &component)
		{
			UI::BeginPropertyGrid();
			UI::DrawColorPicker(translation->GetText("object-properties-light-component-radiance"), component.Radiance);
			UI::DrawDragFloat(translation->GetText("object-properties-light-component-intensity"), component.Intensity);
			UI::DrawCheckbox(translation->GetText("object-properties-light-component-cast-shadows"), component.CastShadows);
			UI::DrawCheckbox(translation->GetText("object-properties-light-component-soft-shadows"), component.SoftShadows);
			UI::DrawDragFloat(translation->GetText("object-properties-light-component-source-size"), component.LightSize);
			UI::EndPropertyGrid();
		}, m_SettingsIcon);

		DrawComponent<PointLightComponent>(translation->GetText("object-properties-point-light-component"), entity, [&](PointLightComponent &component)
		{
			UI::BeginPropertyGrid();
			UI::DrawColorPicker(translation->GetText("object-properties-light-component-radiance"), component.Radiance);
			UI::DrawDragFloat(translation->GetText("object-properties-light-component-intensity"), component.Intensity);
			UI::DrawDragFloat(translation->GetText("object-properties-light-component-radius"), component.Radius, 0.1f, 0.0f, std::numeric_limits<float>::max());
			UI::DrawDragFloat(translation->GetText("object-properties-light-component-falloff"), component.Falloff, 0.005f, 0.0f, 1.0f);
			UI::DrawCheckbox(translation->GetText("object-properties-light-component-cast-shadows"), component.CastShadows);
			UI::DrawCheckbox(translation->GetText("object-properties-light-component-soft-shadows"), component.SoftShadows);
			UI::DrawDragFloat(translation->GetText("object-properties-light-component-source-size"), component.LightSize);
			UI::EndPropertyGrid();
		}, m_SettingsIcon);

		DrawComponent<SkyLightComponent>(translation->GetText("object-properties-sky-light-component"), entity, [&](SkyLightComponent &component)
		{
			UI::BeginPropertyGrid();
			UI::DrawDragFloat(translation->GetText("object-properties-light-component-intensity"), component.Intensity);
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

			UI::DrawCheckbox(translation->GetText("object-properties-light-component-dynamic-sky"), component.DynamicSky);
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

