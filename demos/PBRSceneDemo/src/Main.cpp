#include "PBRSceneTest.h"

int main(int argc, char *argv[])
{
	std::unique_ptr<PBRSceneTest> demo = std::make_unique<PBRSceneTest>();
	demo->Run();

	return 0;
}