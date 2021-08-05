#include "Renderer2DTest.h"

int main(int argc, char *argv[])
{
	std::unique_ptr<Renderer2DTest> rendererTest = std::make_unique<Renderer2DTest>();
	rendererTest->Run();

	return 0;
}

