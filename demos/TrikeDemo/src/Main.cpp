#include "TrikeDemo.h"

int main(int argc, char *argv[])
{
	std::shared_ptr<TrikeDemo> app = std::make_shared<TrikeDemo>();
	app->Run();

	return 0;
}