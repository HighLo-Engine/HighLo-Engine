#include "Sandbox.h"

int main(int argc, char *argv[])
{
	std::unique_ptr<Sandbox> sandbox = std::make_unique<Sandbox>();
	sandbox->Run();

	return 0;
}
