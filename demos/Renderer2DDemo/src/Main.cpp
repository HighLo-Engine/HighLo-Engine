#include "Renderer2DDemo.h"

int main(int argc, char *argv[])
{
	std::unique_ptr<Renderer2DDemo> rendererTest = std::make_unique<Renderer2DDemo>();
	rendererTest->Run();

	return 0;
}

