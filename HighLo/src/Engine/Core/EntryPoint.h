// Copyright (c) 2021 Albert Slepak and Can Karka. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Application/Application.h"
#include "Engine/Core/Profiler/ProfilerTimer.h"

extern highlo::HLApplication *highlo::CreateApp();

int main(int argc, char *argv)
{
	// We have three phases:
	// - Init: The Renderer, Window and Engine Services are being started
	// - Runtime: The Engine runs until the user closes the window, either by close it via the in-game menu or the Windows X button 
	// - Shutdown: If the user decides to shut the engine down, all allocated objects are being freed by the operating system

	// HL_PROFILE_BEGIN_SESSION("Startup", "start.json");
	highlo::HLApplication *app = highlo::CreateApp();
	// HL_PROFILE_END_SESSION();

	// HL_PROFILE_BEGIN_SESSION("Runtime", "runtime.json");
	app->Run();
	// HL_PROFILE_END_SESSION();

	// HL_PROFILE_BEGIN_SESSION("Shutdown", "shutdown.json");
	delete app;
	// HL_PROFILE_END_SESSION();

	return 0;
}

