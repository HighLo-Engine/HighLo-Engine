#include <Engine/Core/EntryPoint.h>
#include "Sandbox.h"

highlo::HLApplication *highlo::CreateApp()
{
	return new Sandbox();
}

