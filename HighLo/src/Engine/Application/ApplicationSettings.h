// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-26) initial release
//

#pragma once

#include "HighLoPch.h"

namespace highlo
{
	struct ApplicationSettings
	{
		/// <summary>
		/// Determines whether the engine should launch without a rendering window (useful for game-servers)
		/// </summary>
		bool Headless = false;

		/// <summary>
		/// Determines whether the engine should start in fullscreen mode, if Headless is false
		/// </summary>
		bool Fullscreen = false;

		/// <summary>
		/// Determines the width of the window, if Headless is false
		/// </summary>
		uint32 WindowWidth = 1270;

		/// <summary>
		/// Determines the height of the window, if Headless is false
		/// </summary>
		uint32 WindowHeight = 920;

		/// <summary>
		/// Determines the window title, if Headless is false
		/// </summary>
		HLString WindowTitle = "HighLo Engine";

		/// <summary>
		/// Disables the log output into a file
		/// </summary>
		bool NoLog = false;

		/// <summary>
		/// Determines the rendering api that should be used in the backend to render, if Headless is false
		/// </summary>
		HLString RenderingAPI = "OpenGL";
	};
}

