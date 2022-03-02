// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-02-05) initial release
//

#pragma once

#include "ImGui.h"
#include "ImGuiWidgets.h"
#include "Engine/Assets/Asset.h"
#include "Engine/ECS/Entity.h"
#include "Engine/Assets/Asset.h"
#include "Engine/Assets/AssetManager.h"
#include "Engine/Scene/Scene.h"

namespace highlo::UI
{
	static AssetHandle s_AssetReferenceAssetHandle;

	enum class AssetReferenceResult
	{
		Ok = 0,
		InvalidMetaData = 1
	};

	struct AssetReferenceSettings
	{
		bool NextColumn = true;
		bool NoItemSpacing = false;
		float WidthOffset = 0.0f;
	};

	template<typename T>
	HLAPI static bool DrawAssetReference(const HLString &label, AssetHandle &outHandle, AssetReferenceResult *outResult = nullptr, const AssetReferenceSettings &settings = AssetReferenceSettings())
	{
		bool modified = false;
		if (outResult)
			*outResult = AssetReferenceResult::Ok;

		UI::ShiftCursor(10.0f, 9.0f);
		ImGui::Text(*label);
		ImGui::NextColumn();
		UI::ShiftCursorY(4.0f);
		ImGui::PushItemWidth(-1);

		ImVec2 backupButtonTextAlign = ImGui::GetStyle().ButtonTextAlign;
		ImGui::GetStyle().ButtonTextAlign = { 0.0f, 0.5f };
		float width = ImGui::GetContentRegionAvail().x - settings.WidthOffset;
		float height = 28.0f;

		HLString buttonText = "Null";
		if (AssetManager::Get()->IsAssetHandleValid(outHandle))
		{
			auto object = AssetManager::Get()->GetAsset<T>(outHandle);
			if (object && !object->IsFlagSet(AssetFlag::Missing))
			{
				buttonText = AssetManager::Get()->GetMetaData(outHandle).FilePath.Filename();
			}
			else
			{
				buttonText = "Missing";
			}
		}

		const char *assetSearchPopupID = UI::GenerateID();
		if (ImGui::Button(fmt::format("{}##{}", *buttonText, (*UI::GetCurrentImGuiIDCounter())++).c_str(), {width, height}))
		{
			ImGui::OpenPopup(assetSearchPopupID);
		}
		ImGui::GetStyle().ButtonTextAlign = backupButtonTextAlign;

		bool clear = false;
		if (Widgets::AssetSearchPopup(assetSearchPopupID, T::GetStaticType(), outHandle, &clear))
		{
			if (clear)
				outHandle = 0;

			modified = true;
		}

		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload *data = ImGui::AcceptDragDropPayload("asset_payload");
			if (data)
			{
				AssetHandle assetHandle = *(AssetHandle*)data->Data;
				s_AssetReferenceAssetHandle = assetHandle;
				const auto &metaData = AssetManager::Get()->GetMetaData(assetHandle);

				if (metaData.Type == T::GetStaticType())
				{
					outHandle = assetHandle;
					modified = true;
				}
			}

			ImGui::EndDragDropTarget();
		}

		if (!UI::IsItemDisabled())
			UI::DrawItemActivityOutline(2.0f, true, Colors::Theme::Accent);

		ImGui::PopItemWidth();
		if (settings.NextColumn)
			ImGui::NextColumn();

		return modified;
	}

	template<typename AssetType, typename ConversionType, typename Func>
	HLAPI static bool DrawAssetReferenceWithConversion(const HLString &label, AssetHandle &outHandle, Func &&conversionFunc, AssetReferenceResult *outResult = nullptr, const AssetReferenceSettings &settings = AssetReferenceSettings())
	{
		bool succeeded = false;
		if (outResult)
			*outResult = AssetReferenceResult::Ok;

		UI::ShiftCursor(10.0f, 9.0f);
		ImGui::Text(*label);
		ImGui::NextColumn();
		UI::ShiftCursorY(4.0f);
		ImGui::PushItemWidth(-1);

		ImVec2 backupButtonTextAlign = ImGui::GetStyle().ButtonTextAlign;
		ImGui::GetStyle().ButtonTextAlign = { 0.0f, 0.5f };
		float width = ImGui::GetContentRegionAvail().x - settings.WidthOffset;
		float height = 28.0f;

		UI::PushID();

		HLString buttonText = "Null";
		if (AssetManager::Get()->IsAssetHandleValid(outHandle))
		{
			auto object = AssetManager::Get()->GetAsset<AssetType>(outHandle);
			if (object && !object->IsFlagSet(AssetFlag::Missing))
			{
				buttonText = AssetManager::Get()->GetMetaData(outHandle).FilePath.Filename();
			}
			else
			{
				buttonText = "Missing";
			}
		}

		if (ImGui::Button(fmt::format("{}##{}", *buttonText, (*UI::GetCurrentImGuiIDCounter())++).c_str(), { width, height }))
		{
			ImGui::OpenPopup("AssetSearchPopup");
		}

		ImGui::GetStyle().ButtonTextAlign = backupButtonTextAlign;

		bool clear = false;
		if (Widgets::AssetSearchPopup("AssetSearchPopup", outHandle, &clear, "Search Assets", ImVec2(250.0f, 350.0f), { ConversionType::GetStaticType(), AssetType::GetStaticType() }))
		{
			if (clear)
				outHandle = 0;

			succeeded = true;
		}

		UI::PopID();

		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("asset_payload");
			if (payload)
			{
				AssetHandle assetHandle = *(AssetHandle*)payload->Data;
				s_AssetReferenceAssetHandle = assetHandle;

				Ref<Asset> asset = AssetManager::Get()->GetAsset<Asset>(assetHandle);
				if (asset)
				{
					if (asset->GetAssetType() == AssetType::GetStaticType())
					{
						outHandle = assetHandle;
						succeeded = true;
					}
					else if (asset->GetAssetType() == ConversionType::GetStaticType())
					{
						conversionFunc(asset.As<ConversionType>());
						succeeded = false;
					}
				}
				else
				{
					if (outResult)
						*outResult = AssetReferenceResult::InvalidMetaData;
				}
			}


			ImGui::EndDragDropTarget();
		}

		if (!UI::IsItemDisabled())
			UI::DrawItemActivityOutline(2.0f, true, Colors::Theme::Accent);

		ImGui::PopItemWidth();
		ImGui::NextColumn();
		UI::DrawUnderline();
		
		return succeeded;
	}

	HLAPI static bool DrawEntityReference(const HLString &label, Entity &entity)
	{
		bool receivedValudEntity = false;

		UI::ShiftCursor(10.0f, 9.0f);
		ImGui::Text(*label);
		ImGui::NextColumn();
		UI::ShiftCursorY(4.0f);
		ImGui::PushItemWidth(-1);

		ImVec2 backupButtonTextAlign = ImGui::GetStyle().ButtonTextAlign;
		ImGui::GetStyle().ButtonTextAlign = { 0.0f, 0.5f };
		float width = ImGui::GetContentRegionAvail().x;
		float height = 28.0f;

		HLString buttonText = "Null";
		if (entity)
			buttonText = entity.Tag();

		ImGui::Button(fmt::format("{}##{}", *buttonText, (*UI::GetCurrentImGuiIDCounter())++).c_str(), {width, height});
		ImGui::GetStyle().ButtonTextAlign = backupButtonTextAlign;

		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("scene_entity_hierarchy");
			if (payload)
			{
				UUID &entityID = *(UUID*)payload->Data;
				Scene *currentScene = Scene::GetActiveScene();
				HL_ASSERT(currentScene);

				entity = currentScene->FindEntityByUUID(entityID);
				receivedValudEntity = true;
			}

			ImGui::EndDragDropTarget();
		}

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return receivedValudEntity;
	}

	template<typename... Components>
	HLAPI static bool DrawEntityReferenceWithComponents(const HLString &label, Entity &entity, bool requireAllComponents = true)
	{
		bool receivedValidEntity = false;

		UI::ShiftCursor(10.0f, 9.0f);
		ImGui::Text(*label);
		ImGui::NextColumn();
		UI::ShiftCursorY(3.0f);
		ImGui::PushItemWidth(-1);

		ImVec2 backupButtonTextAlign = ImGui::GetStyle().ButtonTextAlign;
		ImGui::GetStyle().ButtonTextAlign = { 0.0f, 0.5f };
		float width = ImGui::GetContentRegionAvail().x;
		float height = 28.0f;

		HLString buttonText = "Null";
		if (entity)
			buttonText = entity.Tag();

		ImGui::Button(fmt::format("{}##{}", *buttonText, (*UI::GetCurrentImGuiIDCounter())++).c_str(), { width, height });
		ImGui::GetStyle().ButtonTextAlign = backupButtonTextAlign;

		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("scene_entity_hierarchy");
			if (payload)
			{
				UUID &entityId = *(UUID*)payload->Data;
				Scene *currentScene = Scene::GetActiveScene();
				HL_ASSERT(currentScene);

				Entity &tmp = currentScene->FindEntityByUUID(entityId);
				if (requireAllComponents)
				{
					if (tmp.HasComponents<Components...>())
					{
						entity = tmp;
						receivedValidEntity = true;
					}
				}
				else
				{
					if (tmp.HasAnyOf<Components...>())
					{
						entity = tmp;
						receivedValidEntity = true;
					}
				}
			}

			ImGui::EndDragDropTarget();
		}

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return receivedValidEntity;
	}

	template<typename T, typename Func>
	HLAPI static bool DrawAssetReferenceWithTarget(const HLString &label, const HLString &assetName, AssetHandle &outHandle, Func &&targetFunc, const AssetReferenceSettings &settings = AssetReferenceSettings())
	{
		bool modified = false;

		UI::ShiftCursor(10.0f, 9.0f);
		ImGui::Text(*label);
		ImGui::NextColumn();
		UI::ShiftCursor(4.0f);
		ImGui::PushItemWidth(-1);

		if (settings.NoItemSpacing)
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0.0f, 0.0f });

		ImVec2 backupButtonTextAlign = ImGui::GetStyle().ButtonTextAlign;
		ImGui::GetStyle().ButtonTextAlign = { 0.0f, 0.5f };
		float width = ImGui::GetContentRegionAvail().x - settings.WidthOffset;
		float height = 28.0f;
		UI::PushID();

		HLString buttonText = "Null";
		if (AssetManager::Get()->IsAssetHandleValid(outHandle))
		{
			auto object = AssetManager::Get()->GetAsset<T>(outHandle);
			if (object && !object->IsFlagSet(AssetFlag::Missing))
			{
				if (assetName)
				{
					buttonText = assetName;
				}
				else
				{
					buttonText = AssetManager::Get()->GetMetaData(outHandle).FilePath.Filename();
					assetName = buttonText;
				}
			}
		}

		if (ImGui::Button(fmt::format("{}##{}", *buttonText, (*UI::GetCurrentImGuiIDCounter())++).c_str(), { width, height }))
		{
			ImGui::OpenPopup("AssetSearchPopup");
		}
		ImGui::GetStyle().ButtonTextAlign = backupButtonTextAlign;

		bool clear = false;
		if (Widgets::AssetSearchPopup("AssetSearchPopup", T::GetStaticType(), outHandle, &clear))
		{
			if (clear)
				outHandle = 0;
			else
				targetFunc(AssetManager::Get()->GetAsset<T>(outHandle));

			modified = true;
		}

		UI::PopID();

		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("asset_payload");

			if (payload)
			{
				AssetHandle assetHandle = *(AssetHandle*)payload->Data;
				s_AssetReferenceAssetHandle = assetHandle;

				Ref<Asset> asset = AssetManager::Get()->GetAsset<Asset>(assetHandle);
				if (asset && asset->GetAssetType() == T::GetStaticType())
				{
					targetFunc(asset.As<T>());
					modified = true;
				}
			}

			ImGui::EndDragDropTarget();
		}

		if (!UI::IsItemDisabled())
			UI::DrawItemActivityOutline(2.0f, true, Colors::Theme::Accent);

		ImGui::PopItemWidth();

		if (settings.NextColumn)
			ImGui::NextColumn();

		if (settings.NoItemSpacing)
			ImGui::PopStyleVar();

		return modified;
	}

	template<typename T>
	HLAPI static bool DrawReferenceDropButton(const HLString &label, Ref<T> &object, AssetType supportedType, bool &assetDropped, bool dropTargetEnabled = true)
	{
		bool clicked = false;

		UI::ScopedStyle frameBorderSize(ImGuiStyleVar_FrameBorderSize, 0.0f);
		UI::ScopedColor bg(ImGuiCol_Button, Colors::Theme::PropertyField);
		UI::ScopedColor bgH(ImGuiCol_ButtonHovered, Colors::Theme::PropertyField);
		UI::ScopedColor bgA(ImGuiCol_ButtonActive, Colors::Theme::PropertyField);

		UI::PushID();

		auto *window = ImGui::GetCurrentWindow();
		const bool itemWidthChanged = !window->DC.ItemWidthStack.empty();
		const float buttonWidth = itemWidthChanged ? window->DC.ItemWidth : 0.0f;

		if (object)
		{
			if (!object->IsFlagSet(AssetFlag::Missing))
			{
				auto &assetFileName = AssetManager::Get()->GetMetaData(object->Handle).FilePath.Filename();
				if (ImGui::Button(*assetFileName, { buttonWidth, 0.0f }))
					clicked = true;
			}
			else
			{
				UI::ScopedColor text(ImGuiCol_Text, {});
				if (ImGui::Button("Missing", { buttonWidth, 0.0f }))
					clicked = true;
			}
		}
		else
		{
			UI::ScopedColor text(ImGuiCol_Text, Colors::Theme::Muted);
			if (ImGui::Button("Select Asset", { buttonWidth, 0.0f }))
				clicked = true;
		}

		UI::PopID();

		if (dropTargetEnabled)
		{
			if (ImGui::BeginDragDropTarget())
			{
				const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("asset_payload");
				if (payload)
				{
					AssetHandle assetHandle = *(AssetHandle*)payload->Data;
					Ref<Asset> asset = AssetManager::Get()->GetAsset<Asset>(assetHandle);
					if (asset && asset->GetAssetType == supportedType)
					{
						object = asset.As<T>();
						assetDropped = true;
					}
				}

				ImGui::EndDragDropTarget();
			}
		}

		return clicked;
	}

	template<typename T>
	HLAPI static bool DrawAssetDropdown(const HLString &label, Ref<T> &object, const glm::vec2 &size, AssetHandle *selectedAsset)
	{
		bool modified = false;
		HLString preview;
		float itemHeight = size.y / 10.0f;

		if (AssetManager::Get()->IsAssetHandleValid(*selectedAsset))
			object = AssetManager::Get()->GetAsset<T>(*selectedAsset);

		if (object)
		{
			if (!object->IsFlagSet(AssetFlag::Missing))
			{
				preview = AssetManager::Get()->GetMetaData(*selectedAsset).FilePath.Filename();
			}
			else
			{
				preview = "Missing";
			}
		}
		else
		{
			preview = "Null";
		}

		auto &asset = AssetManager::Get()->GetLoadedAssets();
		AssetHandle current = *selectedAsset;

		if (UI::IsItemDisabled())
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);

		ImGui::SetNextWindowSize({ size.x, size.y });
		if (UI::BeginPopup(*label, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
		{
			ImGui::SetKeyboardFocusHere(0);

			for (auto &[handle, asset] : assets)
			{
				if (asset->GetAssetType() != T::GetStaticType())
					continue;

				auto &metaData = AssetManager::Get()->GetMetaData(handle);
				bool isSelected = (current == handle);
				if (ImGui::Selectable(metaData.FilePath.Filename(), isSelected))
				{
					current = handle;
					*selectedAsset = handle;
					modified = true;
				}

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}

			UI::EndPopup();
		}

		if (UI::IsItemDisabled())
			ImGui::PopStyleVar();

		return modified;
	}
}

