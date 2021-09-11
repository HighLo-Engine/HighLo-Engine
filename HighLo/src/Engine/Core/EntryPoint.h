#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Application/Application.h"

extern highlo::HLApplication *highlo::CreateApp();

int main(int argc, char *argv)
{
	highlo::HLApplication *app = highlo::CreateApp();
	app->Run();
	delete app;

	return 0;
}

