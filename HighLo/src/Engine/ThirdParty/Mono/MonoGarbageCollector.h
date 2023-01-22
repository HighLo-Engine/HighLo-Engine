#pragma once

#ifdef HIGHLO_API_MONO_SCRIPTING

extern "C" {
	typedef struct _MonoObject MonoObject;
}

namespace highlo
{
	class MonoGarbageCollector
	{
	public:

		HLAPI static void Init();
		HLAPI static void Shutdown();

		HLAPI static void CollectGarbage(bool blockUntilFinalized = true);

		HLAPI static void *CreateObjectReference(MonoObject *managedObject, bool weakReference, bool pinned = false, bool track = true);
		HLAPI static MonoObject *GetReferencedObject(void *handle);
		HLAPI static void ReleaseObjectReference(void *handle);
	};
}

#endif // HIGHLO_API_MONO_SCRIPTING

