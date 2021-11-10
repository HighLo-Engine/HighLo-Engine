// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include <any>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include "Components.h"
#include "Engine/Core/UUID.h"

namespace highlo
{
	class ECS_Registry
	{
	public:

		HLAPI ECS_Registry();

		HLAPI static ECS_Registry &Get();

		template <typename T>
		HLAPI const std::vector<T> &GetComponents()
		{
			static std::vector<T> s_empty;

			for (const auto &[index, components] : m_Components)
				if (index == std::type_index(typeid(T)))
					return components;

			return s_empty;
		}

		template <typename T>
		HLAPI T *AddComponent(UUID entityID)
		{
			auto comp = std::make_any<T>();
			auto &type = std::type_index(typeid(T));

			// Add to the list of components
			m_Components[type].push_back({ entityID, comp });

			auto index = m_Components[type].size() - 1;

			// Register the component into the list specific to
			// the entity for faster access in the future.
			m_EntityComponents[entityID].push_back({ type, index });

			T *component_ptr = std::any_cast<T>(&m_Components[type][index].second);

			// Check if it's a transform component
			static std::type_index transform_type = std::type_index(typeid(TransformComponent));
			if (type == transform_type)
				m_TransformComponents[entityID] = static_cast<void*>(component_ptr);

			return component_ptr;
		}

		template <typename T>
		HLAPI T *GetComponent(UUID entityID)
		{
			auto &type = std::type_index(typeid(T));

			if (m_EntityComponents.find(entityID) == m_EntityComponents.end())
				return nullptr;

			for (auto &comp_info : m_EntityComponents[entityID])
				if (comp_info.first == type)
					return std::any_cast<T>(&m_Components[type][comp_info.second].second);

			return nullptr;
		}

		HLAPI void *GetComponentHandle(UUID entityID, std::type_index &type)
		{
			if (m_EntityComponents.find(entityID) == m_EntityComponents.end())
				return nullptr;

			for (auto &comp_info : m_EntityComponents[entityID])
				if (comp_info.first == type)
					return reinterpret_cast<void*>(&m_Components[type][comp_info.second].second);

			return nullptr;
		}

		template <typename T>
		HLAPI bool HasComponent(UUID entityID)
		{
			if (m_EntityComponents.find(entityID) == m_EntityComponents.end())
				return false;

			return true;
		}

		template <typename T>
		HLAPI void RemoveComponent(UUID entityID)
		{
			auto &type = std::type_index(typeid(T));
			if (m_EntityComponents.find(entityID) == m_EntityComponents.end())
				return; // No component with this entityID

			// Find and remove actual component pointer.
			auto &component_infos = m_EntityComponents[entityID];
			for (auto &it = component_infos.begin(); it != component_infos.end(); ++it)
			{
				if (it->first == type)
				{
					m_Components[type].erase(m_Components[type].begin() + it->second);
					m_EntityComponents[entityID].erase(it);
					break;
				}
			}
		}

		template <typename T>
		HLAPI void ForEach(const std::function<void(UUID, TransformComponent&, T&)> &callback)
		{
			auto& type = std::type_index(typeid(T));
			if (m_Components.find(type) != m_Components.end())
				for (auto& comp : m_Components[type])
					callback(comp.first, *static_cast<TransformComponent*>(m_TransformComponents[comp.first]), *std::any_cast<T>(&comp.second));
		}

		template <typename... Args>
		HLAPI void ForEachMultiple(const std::function<void(UUID, TransformComponent&, std::vector<void*>&)> &callback)
		{
			// Create a list of all types
			std::vector<std::type_index> component_types;
			component_types.insert(component_types.end(), { typeid(Args)... });

			HL_ASSERT(component_types.size() > 1, "At least two component types must be specified!");

			auto& first_type = component_types[0];

			// Iterate over components of the first specified type
			if (m_Components.find(first_type) != m_Components.end())
			{
				for (auto& comp : m_Components[first_type])
				{
					EntityID entityID = comp.first;
					std::vector<void*> components = { &comp.second };

					bool should_skip_entity = false;

					// Collect other required components from the found entity
					for (size_t i = 1; i < component_types.size(); ++i)
					{
						// Skip entity if one of the required components is not found
						if (m_Components.find(component_types[i]) == m_Components.end())
						{
							should_skip_entity = true;
							break;
						}

						auto component_handle = GetComponentHandle(entityID, component_types[i]);

						// Skip entity if the required component handle is nullptr
						if (!component_handle)
						{
							should_skip_entity = true;
							break;
						}

						// If all checks are passed, add the component handle to the list
						components.push_back(component_handle);
					}

					// If entity should be skipped, skip
					if (should_skip_entity)
						continue;

					callback(entityID, *static_cast<TransformComponent*>(m_TransformComponents[comp.first]), components);
				}
			}
		}

	private:

		std::unordered_map<std::type_index, std::vector<std::pair<UUID, std::any>>> m_Components;
		std::unordered_map<UUID, std::vector<std::pair<std::type_index, uint64>>> m_EntityComponents;
		std::unordered_map<UUID, void*> m_TransformComponents;
	};
}

