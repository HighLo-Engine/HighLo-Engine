// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-21) initial release
//

#pragma once

// @DEPRECATED
#if 0

// undefine Windows constants
#undef INFINITE
#include <msdf-atlas-gen.h>

namespace highlo
{
	struct MSDFData
	{
		msdf_atlas::FontGeometry FontGeometry;
		std::vector<msdf_atlas::GlyphGeometry> Glyphs;
	};
}

#endif

