#include <Engine/Core/EntryPoint.h>
#include "Renderer3DDemo.h"

highlo::HLApplication *highlo::CreateApp()
{
	return new Renderer3DDemo();
}

