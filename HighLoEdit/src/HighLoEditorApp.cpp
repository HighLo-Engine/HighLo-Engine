// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include <Engine/Core/EntryPoint.h>
#include "HighLoEditor.h"

highlo::HLApplication *highlo::CreateApp(int argc, char *argv[])
{
	highlo::HLString projectPath;
	if (argc > 1)
		projectPath = argv[1];

	highlo::ApplicationSettings settings;
	settings.WindowTitle = "";
	settings.WindowWidth = 1600;
	settings.WindowHeight = 900;
	settings.Maximized = true;
	settings.VSync = true;

	return new HighLoEditor(settings, projectPath);
}

