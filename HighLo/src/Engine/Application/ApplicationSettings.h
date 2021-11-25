// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.1 (2021-10-23) Added MainThreadID
//     - 1.0 (2021-09-26) initial release
//

#pragma once

#include "HighLoPch.h"

#include "Engine/Core/FileSystemPath.h"

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
		/// Determines, whether the rendering window should be maximized, if Headless is false
		/// </summary>
		bool Maximized = false;

		/// <summary>
		/// Determines, whether the framerate should be locked to the maximum display framerate, if Headless is false
		/// </summary>
		bool VSync = false;

		/// <summary>
		/// Determines the rendering api that should be used in the backend to render, if Headless is false
		/// </summary>
		HLString RenderingAPI = "OpenGL";

		/// <summary>
		/// Determines the Main Thread ID of the main thread
		/// </summary>
		uint64 MainThreadID;

		/// <summary>
		/// Determines the path to the cache shader config.
		/// The Cache Shader config stores all shader hashes for later comparison
		/// </summary>
		FileSystemPath ShaderRegistryPath = "assets/shaders/cache/registry.cache";
	};
}

