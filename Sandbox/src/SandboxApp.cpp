#include <Engine/Core/EntryPoint.h>
#include "Sandbox.h"

highlo::HLApplication *highlo::CreateApp(int argc, char *argv[])
{
	highlo::ApplicationSettings settings;
	settings.WorkingDirectory = "../../../";
	return new Sandbox(settings);
}

