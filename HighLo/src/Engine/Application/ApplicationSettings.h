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
		/// Determines, whether to use Dear ImGui as the main UI library. This option overrides every other UI framework,
		/// including the engine's own UI framwork, but it is only relevant for the Editor build, runtime builds will always use the engine's own UI code.
		/// </summary>
		bool EnableDearImGui = true;

		/// <summary>
		/// Determines the rendering api that should be used in the backend to render, if Headless is false.
		/// </summary>
		HLString RenderingAPI = "OpenGL";

		/// <summary>
		/// Determines the window api that should be used to display the window. Valid values are 'GLFW' or 'native'.
		/// </summary>
		HLString WindowAPI = "GLFW";

		/// <summary>
		/// Determines the Main Thread ID of the main thread
		/// </summary>
		uint64 MainThreadID;

		/// <summary>
		/// Determines the path to the shader cache config.
		/// </summary>
		FileSystemPath ShaderRegistryPath = "assets/shaders.registry";

		/// <summary>
		/// Determines the path to the asset registry. It holds all assets created in the editor.
		/// </summary>
		FileSystemPath AssetsRegistryPath = "assets/assets.registry";

		/// <summary>
		/// Determines the path to the startup project for the Editor.
		/// </summary>
		FileSystemPath ProjectPath = "";

		/// <summary>
		/// Determines the current Version String.
		/// </summary>
		HLString Version = "1.0.0";
	};
}

