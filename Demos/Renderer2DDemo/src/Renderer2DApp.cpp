#include <Engine/Core/EntryPoint.h>
#include "Renderer2DDemo.h"

highlo::HLApplication *highlo::CreateApp(int argc, char *argv[])
{
	return new Renderer2DDemo();
}

