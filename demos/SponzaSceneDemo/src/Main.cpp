#include "SponzaSceneTest.h"

int main(int argc, char *argv[])
{
	std::shared_ptr<SponzaSceneTest> app = std::make_shared<SponzaSceneTest>();
	app->Run();

	return 0;
}