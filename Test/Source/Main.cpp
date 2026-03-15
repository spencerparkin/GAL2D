#include "Main.h"
#include "App.h"

int WINAPI WinMain(HINSTANCE instanceHandle, HINSTANCE prevInstanceHandle, LPSTR cmdLine, int cmdShow)
{
	App app(instanceHandle, cmdShow);

	if (!app.Setup())
	{
		MessageBoxA(NULL, "Failed to setup app!", "Error!", MB_OK | MB_ICONERROR);
		return -1;
	}
	
	app.Run();

	return app.Shutdown();
}