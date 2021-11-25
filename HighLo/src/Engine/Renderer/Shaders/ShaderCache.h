#pragma once

#include "Engine/Core/FileSystemPath.h"

namespace highlo
{
	class ShaderCache
	{
	public:

		HLAPI static bool HasChanged(const FileSystemPath &filePath, const HLString &source);

	private:

		static void Serialize(const std::map<HLString, uint64> &shaderCache);
		static void Deserialize(std::map<HLString, uint64> &shaderCache);
	};
}

