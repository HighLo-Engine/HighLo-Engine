#include "Sandbox.h"

int main()
{
	std::unique_ptr<Sandbox> sandbox = std::make_unique<Sandbox>();
	sandbox->Run();

	return 0;
}
