// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include <Engine/Core/EntryPoint.h>
#include "HighLoEditor.h"

#include "Engine/Utils/CommandLineUtils.h"

highlo::HLApplication *highlo::CreateApp(int argc, char *argv[])
{
	highlo::utils::CommandLineHelper cmdHelper(argc, argv);
	return new HighLoEditor(cmdHelper.GetAppSettings());
}

