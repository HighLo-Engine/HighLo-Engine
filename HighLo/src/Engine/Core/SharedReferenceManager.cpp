// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "SharedReferenceManager.h"

namespace highlo
{
	static std::unordered_set<void *> s_LiveReferences;
	static std::mutex s_LiveReferenceMutex;

	void SharedReferenceManager::AddToLiveReferences(void *instance)
	{
		std::scoped_lock<std::mutex> lock(s_LiveReferenceMutex);
		HL_ASSERT(instance);
		s_LiveReferences.insert(instance);
	}
	
	void SharedReferenceManager::RemoveFromLiveReferences(void *instance)
	{
		std::scoped_lock<std::mutex> lock(s_LiveReferenceMutex);
		HL_ASSERT(instance);
		HL_ASSERT(s_LiveReferences.find(instance) != s_LiveReferences.end());
		s_LiveReferences.erase(instance);
	}

	bool SharedReferenceManager::IsSharedRefAlive(void *instance)
	{
		HL_ASSERT(instance);
		return s_LiveReferences.find(instance) != s_LiveReferences.end();
	}
}

