#include <Engine/Core/EntryPoint.h>
#include "SponzaSceneDemo.h"

highlo::HLApplication *highlo::CreateApp(int argc, char *argv[])
{
	return new SponzaSceneDemo();
}

