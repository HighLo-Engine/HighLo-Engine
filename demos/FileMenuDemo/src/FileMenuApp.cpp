#include <Engine/Core/EntryPoint.h>
#include "FileMenuDemo.h"

highlo::HLApplication *highlo::CreateApp()
{
	return new FileMenuDemo();
}

