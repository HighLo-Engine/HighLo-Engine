#include <Engine/Core/EntryPoint.h>
#include "Renderer3DDemo.h"

highlo::HLApplication *highlo::CreateApp(int argc, char *argv[])
{
	return new Renderer3DDemo();
}

