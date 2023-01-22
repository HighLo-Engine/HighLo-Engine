#include "HighLoPch.h"
#include "MonoGarbageCollector.h"

#ifdef HIGHLO_API_MONO_SCRIPTING

#include <mono/metadata/object.h>
#include <mono/metadata/mono-gc.h>
#include <mono/metadata/profiler.h>

namespace highlo
{
	void MonoGarbageCollector::Init()
	{

	}

	void MonoGarbageCollector::Shutdown()
	{

	}

	void MonoGarbageCollector::CollectGarbage(bool blockUntilFinalized)
	{

	}

	void *MonoGarbageCollector::CreateObjectReference(MonoObject *managedObject, bool weakReference, bool pinned, bool track)
	{
		return nullptr;
	}

	MonoObject *MonoGarbageCollector::GetReferencedObject(void *handle)
	{
		return nullptr;
	}

	void MonoGarbageCollector::ReleaseObjectReference(void *handle)
	{

	}
}

#endif // HIGHLO_API_MONO_SCRIPTING

