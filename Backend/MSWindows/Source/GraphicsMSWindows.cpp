#include "GraphicsMSWindows.h"
#include <windowsx.h>

using namespace GAL2D;

GraphicsMSWindows::GraphicsMSWindows(const DriverInitData* driverInitData)
{
	this->instanceHandle = driverInitData->instanceHandle ? driverInitData->instanceHandle : (HINSTANCE)GetModuleHandle(NULL);
	this->windowTitle = driverInitData->windowTitle;
	this->windowWidth = driverInitData->windowWidth;
	this->windowHeight = driverInitData->windowHeight;
	this->windowHandle = NULL;
	this->exitSignaled = false;
}

/*virtual*/ GraphicsMSWindows::~GraphicsMSWindows()
{
}

/*virtual*/ bool GraphicsMSWindows::Setup()
{
	if (this->windowHandle != NULL)
		return false;

	WNDCLASSEX winClass{};
	winClass.cbSize = sizeof(WNDCLASSEX);
	winClass.style = CS_HREDRAW | CS_VREDRAW;
	winClass.lpfnWndProc = &GraphicsMSWindows::WndProc;
	winClass.cbClsExtra = 0;
	winClass.cbWndExtra = sizeof(GraphicsMSWindows*);
	winClass.hInstance = this->instanceHandle;
	winClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	winClass.lpszClassName = "GAL2D_MSWindow";
	ATOM atom = RegisterClassEx(&winClass);

	this->windowHandle = CreateWindow(winClass.lpszClassName, this->windowTitle.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, this->windowWidth, this->windowHeight, nullptr, nullptr, this->instanceHandle, nullptr);
	if (!this->windowHandle)
		return false;

	SetWindowLongPtr(this->windowHandle, 0, (LONG_PTR)this);

	this->exitSignaled = false;

	return true;
}

/*virtual*/ void GraphicsMSWindows::Shutdown()
{
	GraphicsInterface::Shutdown();
}

/*static*/ LRESULT CALLBACK GraphicsMSWindows::WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
	GraphicsMSWindows* graphics = (GraphicsMSWindows*)GetWindowLongPtr(windowHandle, 0);
	if (graphics)
		return graphics->HandleMessage(windowHandle, message, wParam, lParam);

	return DefWindowProc(windowHandle, message, wParam, lParam);
}

LRESULT GraphicsMSWindows::HandleMessage(HWND msgWindowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_DESTROY:
		{
			this->exitSignaled = true;
			break;
		}
		case WM_KEYDOWN:
		{
			if (this->keyboardEventHandler)
			{
				// STPTODO: Maybe we need to platform-agnostic key code here?
				int keyCode = (int)wParam;
				this->keyboardEventHandler(keyCode, ButtonState::Down);
			}
			return 0;
		}
		case WM_KEYUP:
		{
			if (this->keyboardEventHandler)
			{
				// STPTODO: Maybe we need to platform-agnostic key code here?
				int keyCode = (int)wParam;
				this->keyboardEventHandler(keyCode, ButtonState::Up);
			}
			return 0;
		}
		case WM_LBUTTONDOWN:
		{
			if (this->mouseClickEventHandler)
			{
				Vector worldMousePos = this->CalcWorldMousePos(lParam);
				this->mouseClickEventHandler(worldMousePos, MouseButton::Left, ButtonState::Down);
			}
			return 0;
		}
		case WM_LBUTTONUP:
		{
			if (this->mouseClickEventHandler)
			{
				Vector worldMousePos = this->CalcWorldMousePos(lParam);
				this->mouseClickEventHandler(worldMousePos, MouseButton::Left, ButtonState::Up);
			}
			return 0;
		}
		case WM_RBUTTONDOWN:
		{
			if (this->mouseClickEventHandler)
			{
				Vector worldMousePos = this->CalcWorldMousePos(lParam);
				this->mouseClickEventHandler(worldMousePos, MouseButton::Right, ButtonState::Down);
			}
			return 0;
		}
		case WM_RBUTTONUP:
		{
			if (this->mouseClickEventHandler)
			{
				Vector worldMousePos = this->CalcWorldMousePos(lParam);
				this->mouseClickEventHandler(worldMousePos, MouseButton::Right, ButtonState::Up);
			}
			return 0;
		}
		case WM_MOUSEMOVE:
		{
			if (this->mouseMotionEventHandler)
			{
				Vector worldMousePos = this->CalcWorldMousePos(lParam);
				this->mouseMotionEventHandler(worldMousePos);
			}
			return 0;
		}
	}

	return DefWindowProc(msgWindowHandle, message, wParam, lParam);
}

Vector GraphicsMSWindows::CalcWorldMousePos(LPARAM lParam)
{
	Vector mousePos;
	mousePos.x = (double)GET_X_LPARAM(lParam);
	mousePos.y = (double)GET_Y_LPARAM(lParam);

	RECT windowRect;
	GetClientRect(this->windowHandle, &windowRect);

	Rectangle windowRegion;
	windowRegion.minCorner.x = (double)windowRect.left;
	windowRegion.maxCorner.x = (double)windowRect.right;
	windowRegion.minCorner.y = (double)windowRect.bottom;
	windowRegion.maxCorner.y = (double)windowRect.top;

	Vector UVs;
	windowRegion.PointToUVs(UVs, mousePos);

	Vector worldMousePos;
	this->renderState.worldRegion.PointFromUVs(UVs, worldMousePos);

	return worldMousePos;
}

/*virtual*/ bool GraphicsMSWindows::HandleEvents()
{
	MSG msg;
	while (PeekMessage(&msg, this->windowHandle, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return !this->exitSignaled;
}

/*virtual*/ void GraphicsMSWindows::BeginRendering()
{
	RECT clientRect;
	GetClientRect(this->windowHandle, &clientRect);

	this->renderState.worldRegion.minCorner.x = 0.0;
	this->renderState.worldRegion.minCorner.y = 0.0;
	this->renderState.worldRegion.maxCorner.x = double(clientRect.right - clientRect.left);
	this->renderState.worldRegion.maxCorner.y = double(clientRect.bottom - clientRect.top);
}

/*virtual*/ void GraphicsMSWindows::EndRendering()
{
}