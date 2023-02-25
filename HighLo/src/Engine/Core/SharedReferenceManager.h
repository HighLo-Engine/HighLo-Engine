// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-04-05) initial release
//

#pragma once

namespace highlo
{
	class SharedReferenceManager : public Singleton<SharedReferenceManager>
	{
	public:

		HLAPI void AddToLiveReferences(void *instance);
		HLAPI void RemoveFromLiveReferences(void *instance);
		HLAPI bool IsSharedRefAlive(void *instance);
	};
}

