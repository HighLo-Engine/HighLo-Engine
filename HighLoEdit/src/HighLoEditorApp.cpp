// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include <Engine/Core/EntryPoint.h>
#include "HighLoEditor.h"

#include "Engine/Utils/CommandLineUtils.h"

highlo::HLApplication *highlo::CreateApp(int argc, char *argv[])
{
	highlo::FileSystem::Get()->SetCurrentWorkingDirectory("../../../");
	highlo::utils::CommandLineHelper cmdHelper(argc, argv);
	highlo::ApplicationSettings &settings = cmdHelper.GetAppSettings();
	settings.WorkingDirectory = "../../../";
	
	if (cmdHelper.ShouldShowHelpMenu())
	{
		std::cout << "HighLoEdit Help menu" << std::endl;
		std::cout << "Usage: HighLoEdit.exe [--option] {value}" << std::endl;
		std::cout << std::endl;

		std::cout << "Engine options:" << std::endl;
		std::cout << "--working-directory {value}                             : Determines the path, from which the engine should search for all other paths" << std::endl;
		std::cout << "--headless                                              : Determines, if the engine should run without a rendering window (is used by server applications)" << std::endl;
		std::cout << "--fullscreen                                            : Determines, if the engine should start in fullscreen mode (mostly relevant for the runtime)" << std::endl;
		std::cout << "--maximized                                             : Determines, if the engine should start in a maximized window (mostly relevant for the editor)" << std::endl;
		std::cout << "--nvsync                                                : Determines, if V-Sync should be deactivated" << std::endl;
		std::cout << "--width {value}                                         : Determines the initial window width" << std::endl;
		std::cout << "--height {value}                                        : Determines the initial window height" << std::endl;
		std::cout << "--title {value}                                         : Determines the initial window title" << std::endl;
		std::cout << "--nlog                                                  : Determines, whether the engine should not log the log output into log files" << std::endl;
		std::cout << "--target-framerate {value}                              : Determines the target fps the engine should run on (INOP at the moment)" << std::endl;
		std::cout << "--active-translation {value} [en-US|de-DE]              : Determines the initial language, in which the engine should display text" << std::endl;
		std::cout << "--enable-dear-imgui                                     : Determines, whether Dear ImGui should be used over the engines own UI framework (mostly relevant for the editor, but will be removed at a later time)" << std::endl;
		std::cout << "--rendering-api {value} [OpenGL|DX11|DX12|Vulkan|Metal] : Determines the rendering api, which should be used for rendering" << std::endl;
		std::cout << "--window-api {value} [GLFW]                             : Determines the window api, which should be used for displaying the window. Default is GLFW at the moment, but will eventually change to WinAPI for windows and other libs for other platforms." << std::endl;
		std::cout << std::endl;
		
		std::cout << "Editor options:" << std::endl;
		return nullptr;
	}

	if (cmdHelper.ShouldShowVersion())
	{
		std::cout << "HighLoEdit Version 1.0.0 Build 0.0.0.1" << std::endl;
		return nullptr;
	}
	
	return new HighLoEditor(settings);
}

