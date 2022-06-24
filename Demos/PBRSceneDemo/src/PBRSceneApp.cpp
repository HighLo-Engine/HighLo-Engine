#include <Engine/Core/EntryPoint.h>
#include "PBRSceneDemo.h"

highlo::HLApplication *highlo::CreateApp(int argc, char *argv[])
{
	return new PBRSceneDemo();
}

