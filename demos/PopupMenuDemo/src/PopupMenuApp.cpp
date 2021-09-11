#include <Engine/Core/EntryPoint.h>
#include "PopupMenuDemo.h"

highlo::HLApplication *highlo::CreateApp()
{
	return new PopupMenuDemo();
}

