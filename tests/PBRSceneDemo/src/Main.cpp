#include "PBRSceneDemo.h"

int main(int argc, char *argv[])
{
	std::unique_ptr<PBRSceneDemo> demo = std::make_unique<PBRSceneDemo>();
	demo->Run();

	return 0;
}