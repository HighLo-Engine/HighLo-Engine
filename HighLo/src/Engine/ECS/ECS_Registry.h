#pragma once

#include <any>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include "Components.h"

namespace highlo
{
	typedef uint64 EntityID;

	class ECS_Registry
	{
		friend class HLApplication;
		ECS_Registry();

	public:
		HLAPI static ECS_Registry& Get();

		HLAPI EntityID GenerateEntityID();

		template <typename T>
		HLAPI const std::vector<T>& GetComponents()
		{
			static std::vector<T> s_empty;

			for (const auto& [index, components] : m_Components)
				if (index == std::type_index(typeid(T)))
					return components;

			return s_empty;
		}

		template <typename T>
		HLAPI T* AddComponent(EntityID entityID)
		{
			auto comp = std::make_any<T>();
			auto& type = std::type_index(typeid(T));

			// Add to the list of components
			m_Components[type].push_back({ entityID, comp });

			auto index = m_Components[type].size() - 1;

			// Register the component into the list specific to
			// the entity for faster access in the future.
			m_EntityComponents[entityID].push_back({ type, index });

			T* component_ptr = std::any_cast<T>(&m_Components[type][index].second);

			// Check if it's a transform component
			static std::type_index transform_type = std::type_index(typeid(TransformComponent));
			if (type == transform_type)
				m_TransformComponents[entityID] = static_cast<void*>(component_ptr);

			return component_ptr;
		}

		template <typename T>
		HLAPI T* GetComponent(EntityID entityID)
		{
			auto& type = std::type_index(typeid(T));

			if (m_EntityComponents.find(entityID) == m_EntityComponents.end())
				return nullptr;

			for (auto& comp_info : m_EntityComponents[entityID])
				if (comp_info.first == type)
					return std::any_cast<T>(&m_Components[type][comp_info.second].second);

			return nullptr;
		}

		template <typename T>
		HLAPI void ForEach(const std::function<void(EntityID, T&, TransformComponent&)>& callback)
		{
			auto& type = std::type_index(typeid(T));
			if (m_Components.find(type) != m_Components.end())
				for (auto& comp : m_Components[type])
					callback(comp.first, *std::any_cast<T>(&comp.second), *static_cast<TransformComponent*>(m_TransformComponents[comp.first]));
		}

	private:

		std::unordered_map<std::type_index, std::vector<std::pair<EntityID, std::any>>> m_Components;
		std::unordered_map<EntityID, std::vector<std::pair<std::type_index, uint64>>> m_EntityComponents;
		std::unordered_map<EntityID, void*> m_TransformComponents;
	};
}
