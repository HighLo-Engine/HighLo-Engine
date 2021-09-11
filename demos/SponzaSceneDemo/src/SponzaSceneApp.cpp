#include <Engine/Core/EntryPoint.h>
#include "SponzaSceneDemo.h"

highlo::HLApplication *highlo::CreateApp()
{
	return new SponzaSceneDemo();
}

