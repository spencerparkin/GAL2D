#pragma once

#include <Windows.h>
#include "Graphics/GraphicsInterface.h"
#include <memory>

/**
 * This application is designed to test GAL2D on windows.
 */
class App
{
public:
	App(HINSTANCE instanceHandle, int cmdShow);
	virtual ~App();

	bool Setup();
	void Run();
	int Shutdown();

private:
	static LRESULT CALLBACK WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);

	LRESULT HandleMessage(HWND msgWindowHandle, UINT message, WPARAM wParam, LPARAM lParam);

	int cmdShow;
	HINSTANCE instanceHandle;
	HWND windowHandle;
	bool keepRunning;
	int exitCode;

	std::shared_ptr<GAL2D::GraphicsInterface> graphics;
};