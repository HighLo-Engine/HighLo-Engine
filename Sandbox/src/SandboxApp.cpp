#include <Engine/Core/EntryPoint.h>
#include "Sandbox.h"

highlo::HLApplication *highlo::CreateApp(int argc, char *argv[])
{
	return new Sandbox();
}

