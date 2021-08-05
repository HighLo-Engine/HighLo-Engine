#include "Renderer3DTest.h"

int main(int argc, char *argv[])
{
	std::unique_ptr<Renderer3DTest> rendererTest = std::make_unique<Renderer3DTest>();
	rendererTest->Run();

	return 0;
}