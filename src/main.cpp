#include "Engine.h"

int main()
{
	Engine::Get().Create("Engine test", 800, 600);

	Log("Hello world!");

	while (Engine::Get().Update())
	{
		Engine::Get().BeginFrame();


		Engine::Get().EndFrame();
	}

	return 0;

}