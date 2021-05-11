#include "HighLoEdit.h"

int main(int argc, char *argv[])
	{
	std::unique_ptr<HighLoEditor> levelEditor = std::make_unique<HighLoEditor>();
	levelEditor->Run();

	return 0;
	}