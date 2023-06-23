// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2023-06-21) initial release
//

#pragma once

#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>

namespace highlo
{
	struct AssimpLogStream : public Assimp::LogStream
	{
		static void Initialize();
		virtual void write(const char *message) override;
	};
}

