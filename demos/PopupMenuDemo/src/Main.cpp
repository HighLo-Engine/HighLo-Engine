#include "PopupMenuTest.h"

int main(int argc, char *argv[])
{
	std::unique_ptr<PopupMenuTest> popupTest = std::make_unique<PopupMenuTest>();
	popupTest->Run();

	return 0;
}