// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include <Engine/Core/EntryPoint.h>
#include "HighLoEditor.h"

highlo::HLApplication *highlo::CreateApp()
{
	return new HighLoEditor();
}

