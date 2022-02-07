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

		/// <summary>
		/// Constructor which creates a static instance of the registry.
		/// </summary>
		/// <returns></returns>
		HLAPI ECS_Registry();

		/// <summary>
		/// Getter to get the static instance of the registry class. Should only be used in a Entity class.
		/// </summary>
		/// <returns></returns>
		HLAPI static ECS_Registry &Get();

		/// <summary>
		/// Adds a new Component to the given entity. A instance of that created component will be returned.
		/// </summary>
		/// <typeparam name="T">The component which should be created.</typeparam>
		/// <param name="entityID">The entityID in which the component should be created.</param>
		/// <returns>Returns the freshly created instance of the requested component.</returns>
		template<typename T>
		HLAPI T *AddComponent(UUID entityID)
		{
			auto comp = std::make_any<T>();
			auto &type = std::type_index(typeid(T));

			// Add to the list of components
			m_Components[type].push_back({ entityID, comp });
			uint64 index = m_Components[type].size() - 1;

			// Register the component into the list specific to
			// the entity for faster access in the future.
			m_EntityComponents[entityID].push_back({ type, index });

			return std::any_cast<T>(&m_Components[type][index].second);
		}

		/// <summary>
		/// Adds or replaces (if a component of the same type already exists) the component from the srcEntity into the dstEntity.
		/// This is used when a entity should be duplicated.
		/// </summary>
		/// <typeparam name="T">The Template component type, which should be added or replaced.</typeparam>
		/// <param name="dstEntityID">The Destination Entity ID, where the component should be added or replaced.</param>
		/// <param name="srcEntityID">The Source Entity ID, from which the component should be retrieved.</param>
		/// <param name="componentToReplace">A already existing instance of the same template component type. This instance is used, to replace the existing value in the internal storage, if some is found. Can be retrieved with 'srcEntity.GetComponent<T>()'</param>
		/// <returns></returns>
		template<typename T>
		HLAPI T *AddOrReplace(UUID dstEntityID, UUID srcEntityID, T *componentToReplace)
		{
			auto &type = std::type_index(typeid(T));
			std::vector<std::pair<std::type_index, uint64>> &matchingEntity = m_EntityComponents[srcEntityID];

			// First try to find the requested component in the source entity
			int32 componentIndex = -1;
			for (auto &[currentType, index] : matchingEntity)
			{
				std::pair<UUID, std::any> &matchingComponent = m_Components[currentType][index];
				T *component = std::any_cast<T>(&matchingComponent.second);

				// If component is not NULL, it was casted successfully in the requested type, therefore a item of that type exists
				if (component)
				{
					componentIndex = (int32)index;
					break;
				}
			}

			if (componentIndex != -1)
			{
				// The component exists in the source entity, so copy it over into the destination entity
				std::pair<UUID, std::any> &valueToCopy = m_Components[type][componentIndex];

				// Make sure we copy the correct component
				if (valueToCopy.first == srcEntityID)
				{
					m_Components[type].push_back({ dstEntityID, valueToCopy.second });
					uint64 index = m_Components[type].size() - 1;

					m_EntityComponents[dstEntityID].push_back({ type, index });
					return std::any_cast<T>(&m_Components[type][index].second);
				}
			}

			// The component was not found, so add it normally to the destination entity
			auto componentToInsert = std::make_any<T>(*componentToReplace);
			m_Components[type].push_back({ dstEntityID, componentToInsert });
			uint64 index = m_Components[type].size() - 1;

			m_EntityComponents[dstEntityID].push_back({ type, index });
			return std::any_cast<T>(&m_Components[type][index].second);
		}

		/// <summary>
		/// Returns the stored instance of the component specified in the template type.
		/// </summary>
		/// <typeparam name="T">The template component type, which instance should be returned.</typeparam>
		/// <param name="entityID">The entityID, from which the component should be retrieved.</param>
		/// <returns></returns>
		template<typename T>
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

		/// <summary>
		/// Returns whether the entity has the given component.
		/// </summary>
		/// <param name="entityID">The Entity, which should be checked against the given template component type.</param>
		/// <returns>Returns true, if the entity with the given entityID has the given template component.</returns>
		template<typename T>
		HLAPI bool HasComponent(UUID entityID)
		{
			// check if entity exists
			if (m_EntityComponents.find(entityID) == m_EntityComponents.end())
				return false;

			auto &type = std::type_index(typeid(T));
			auto &componentInfos = m_EntityComponents[entityID];
			for (auto &it = componentInfos.begin(); it != componentInfos.end(); ++it)
			{
				if (it->first == type)
					return true;
			}

			return false;
		}

		/// <summary>
		/// Returns whether the entity has all of the given components.
		/// @WARN untested
		/// </summary>
		/// <param name="entityID">The Entity, which should be checked against the given template component types.</param>
		/// <returns>Returns true, if the entity with the given entityID has all of the given template components.</returns>
		template<typename... Args>
		HLAPI bool HasComponents(UUID entityID)
		{
			// Create a list of all types
			std::vector<std::type_index> componentTypes;
			componentTypes.insert(componentTypes.end(), { typeid(Args)... });

			if (m_EntityComponents.find(entityID) == m_EntityComponents.end())
				return false;

			bool anyMissing = false;
			std::vector<std::pair<std::type_index, uint64>> &matchingEntity = m_EntityComponents[entityID];
			for (auto &[currentType, index] : matchingEntity)
			{
				auto it = std::find(componentTypes.begin(), componentTypes.end(), currentType);
				if (it == componentTypes.end())
				{
					anyMissing = true;
					break;
				}
			}

			return !anyMissing;
		}

		/// <summary>
		/// Returns whether the entity has any of the given components.
		/// @WARN untested
		/// </summary>
		/// <param name="entityID">The Entity, which should be checked against the given template component types.</param>
		/// <returns>Returns true, if the entity with the given entityID has at least one or more of the given template components.</returns>
		template<typename... Args>
		HLAPI bool HasAnyOf(UUID entityID)
		{
			// Create a list of all types
			std::vector<std::type_index> componentTypes;
			componentTypes.insert(componentTypes.end(), { typeid(Args)... });

			if (m_EntityComponents.find(entityID) == m_EntityComponents.end())
				return false;

			bool anyFound = false;
			std::vector<std::pair<std::type_index, uint64>> &matchingEntity = m_EntityComponents[entityID];
			for (auto &[currentType, index] : matchingEntity)
			{
				auto it = std::find(componentTypes.begin(), componentTypes.end(), currentType);
				if (it != componentTypes.end())
				{
					anyFound = true;
					break;
				}
			}

			return anyFound;
		}

		/// <summary>
		/// Removes a single component of a specific entity.
		/// </summary>
		/// <param name="entityID">The entity ID, which component should be destroyed.</param>
		template<typename T>
		HLAPI void RemoveComponent(UUID entityID)
		{
			auto &type = std::type_index(typeid(T));
			if (m_EntityComponents.find(entityID) == m_EntityComponents.end())
				return; // No component with this entityID

			// Find and remove actual component pointer.
			auto &componentInfos = m_EntityComponents[entityID];
			for (auto &it = componentInfos.begin(); it != componentInfos.end(); ++it)
			{
				if (it->first == type)
				{
					m_Components[type].erase(m_Components[type].begin() + it->second);
					m_EntityComponents[entityID].erase(it);
					break;
				}
			}
		}

		/// <summary>
		/// Destroys all components with the given entity id.
		/// </summary>
		/// <param name="entityID">The entity ID, which components should be destroyed.</param>
		HLAPI void DestroyAllByEntityId(UUID entityID)
		{
			if (m_EntityComponents.find(entityID) == m_EntityComponents.end())
				return; // No entities found to delete

			m_EntityComponents[entityID].clear();
			m_EntityComponents[entityID].shrink_to_fit();
			m_EntityComponents.erase(entityID);
		}

		/// <summary>
		/// Returns a list of entites with the given template component types.
		/// </summary>
		/// <returns>Returns only entites, which have all of the specified template component types.</returns>
		template<typename... Args>
		HLAPI std::vector<UUID> View()
		{
			std::vector<UUID> result;
			std::vector<std::type_index> componentTypes;

			// Create a list of all types
			componentTypes.insert(componentTypes.end(), { typeid(Args)... });

			for (auto &[entityId, component] : m_EntityComponents)
			{
				auto neededComponentsCount = componentTypes.size();
				auto foundComponentsCount = 0;

				for (auto &currentComponent : component)
				{
					auto &componentType = currentComponent.first;
					auto it = std::find(componentTypes.begin(), componentTypes.end(), componentType);
					if (it != componentTypes.end())
					{
						++foundComponentsCount;
					}
				}

				if (foundComponentsCount == neededComponentsCount)
					result.push_back(entityId);
			}

			return result;
		}

	private:

		std::unordered_map<std::type_index, std::vector<std::pair<UUID, std::any>>> m_Components;		// Component Type -> { Component ID, Component }
		std::unordered_map<UUID, std::vector<std::pair<std::type_index, uint64>>> m_EntityComponents;	// EntityID -> [ Component Type, Component index ]
	};
}

