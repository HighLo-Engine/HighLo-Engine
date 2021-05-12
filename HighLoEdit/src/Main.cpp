#include "HighLoEdit.h"

int main(int argc, char *argv[])
{
	std::unique_ptr<HighLoEditor> editor = std::make_unique<HighLoEditor>();
	editor->Run();

	return 0;
}