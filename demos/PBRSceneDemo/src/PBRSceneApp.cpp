#include <Engine/Core/EntryPoint.h>
#include "PBRSceneDemo.h"

highlo::HLApplication *highlo::CreateApp()
{
	return new PBRSceneDemo();
}

