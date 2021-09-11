#include <Engine/Core/EntryPoint.h>
#include "Renderer2DDemo.h"

highlo::HLApplication *highlo::CreateApp()
{
	return new Renderer2DDemo();
}

