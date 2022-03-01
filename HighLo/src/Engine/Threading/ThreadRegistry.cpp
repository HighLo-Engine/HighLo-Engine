// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "ThreadRegistry.h"

#include "ThreadLocker.h"

namespace highlo
{
	static std::unordered_map<UUID, Thread*> s_Registry;
	static ThreadLocker *s_Lock;

	void ThreadRegistry::Init()
	{
		s_Lock = ThreadLocker::Create();
	}

	void ThreadRegistry::Shutdown()
	{
		delete s_Lock;
	}

	Thread *ThreadRegistry::GetThread(UUID uuid)
	{
		Thread *result = nullptr;

		s_Lock->Lock();
		if (s_Registry.find(uuid) != s_Registry.end())
			result = s_Registry[uuid];
		s_Lock->Unlock();

		return result;
	}

	int32 ThreadRegistry::Count()
	{
		s_Lock->Lock();
		int32 count = (int32)s_Registry.size();
		s_Lock->Unlock();

		return count;
	}

	void ThreadRegistry::KillAll()
	{
		s_Lock->Lock();
		for (auto it = s_Registry.begin(); it != s_Registry.end(); ++it)
			it->second->Kill(false);
		s_Lock->Unlock();
	}

	void ThreadRegistry::Add(Thread *thread)
	{
		HL_ASSERT(thread && thread->GetID() != 0);

		s_Lock->Lock();
		HL_ASSERT(s_Registry.find(thread->GetID()) == s_Registry.end());
		s_Registry.insert({ thread->GetID(), thread });
		s_Lock->Unlock();
	}

	void ThreadRegistry::Remove(Thread *thread)
	{
		HL_ASSERT(thread && thread->GetID() != 0);

		s_Lock->Lock();
		HL_ASSERT(s_Registry.find(thread->GetID()) != s_Registry.end());
		s_Registry.erase(thread->GetID());
		s_Lock->Unlock();
	}
}

