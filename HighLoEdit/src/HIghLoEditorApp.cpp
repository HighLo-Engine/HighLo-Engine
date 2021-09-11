#include <Engine/Core/EntryPoint.h>
#include "HighLoEditor.h"

highlo::HLApplication *highlo::CreateApp()
{
	return new HighLoEditor();
}

