#include "HighLoPch.h"
#include "AssetLoader.h"
#include "AssimpLoader.h"

namespace highlo
{
	Model AssetLoader::LoadStaticModel(const HLString& filepath, bool bShouldApplyCorrectionMatrix)
	{
		return AssimpLoader::LoadStatic(filepath, bShouldApplyCorrectionMatrix);
	}
}
