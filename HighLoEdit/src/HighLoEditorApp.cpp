// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include <Engine/Core/EntryPoint.h>
#include "HighLoEditor.h"

#include "Engine/Utils/CommandLineUtils.h"

highlo::HLApplication *highlo::CreateApp(int argc, char *argv[])
{
	highlo::utils::CommandLineHelper cmdHelper(argc, argv);
	highlo::ApplicationSettings &settings = cmdHelper.GetAppSettings();

	// DEBUG
	for (int32 i = 0; i < argc; ++i)
	{
		std::cout << argv[i] << std::endl;
	}

	FileSystemPath test = FileSystem::Get()->GetCurrentWorkingDirectory();
	settings.WorkingDirectory = "../../../";

	return new HighLoEditor(settings);
}

