#include "LevelEditor.h"

int main(int argc, char *argv[])
	{
	std::unique_ptr<LevelEditor> levelEditor = std::make_unique<LevelEditor>();
	levelEditor->Run();

	return 0;
	}