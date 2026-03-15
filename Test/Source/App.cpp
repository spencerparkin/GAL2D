#include "App.h"
#if defined USING_OPENGL
#   include "GraphicsOpenGL.h"
#endif

App::App(HINSTANCE instanceHandle, int cmdShow)
{
    this->cmdShow = cmdShow;
    this->instanceHandle = instanceHandle;
    this->windowHandle = NULL;
    this->keepRunning = false;
    this->exitCode = 0;
}

/*virtual*/ App::~App()
{
}

bool App::Setup()
{
    if (this->windowHandle != NULL)
        return false;

    WNDCLASSEX winClass{};
    winClass.cbSize = sizeof(WNDCLASSEX);
    winClass.style = CS_HREDRAW | CS_VREDRAW;
    winClass.lpfnWndProc = &App::WndProc;
    winClass.cbClsExtra = 0;
    winClass.cbWndExtra = sizeof(App*);
    winClass.hInstance = this->instanceHandle;
    winClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    winClass.lpszClassName = "TestAppClass";
    ATOM atom = RegisterClassEx(&winClass);

    this->windowHandle = CreateWindow(winClass.lpszClassName, "GAL2D Test App", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, nullptr, nullptr, this->instanceHandle, nullptr);
    if (!this->windowHandle)
        return false;

#if defined USING_OPENGL
    this->graphics = std::make_shared<GAL2D::GraphicsOpenGL>(this->windowHandle);
#else
#   error Graphics driver not specified or not supported on Window.
#endif

    if (!this->graphics->Setup())
        return false;

    SetWindowLongPtr(this->windowHandle, 0, (LONG_PTR)this);

    this->keepRunning = true;

    ShowWindow(this->windowHandle, this->cmdShow);
    UpdateWindow(this->windowHandle);

    return true;
}

void App::Run()
{
    while (this->keepRunning)
    {
        MSG msg;
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            DispatchMessage(&msg);
        }

        this->graphics->BeginRendering();
        
        //...

        this->graphics->EndRendering();
    }
}

int App::Shutdown()
{
    return this->exitCode;
}

/*static*/ LRESULT CALLBACK App::WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
    App* app = (App*)GetWindowLongPtr(windowHandle, 0);
    if (app)
        return app->HandleMessage(windowHandle, message, wParam, lParam);

    return DefWindowProc(windowHandle, message, wParam, lParam);
}

LRESULT App::HandleMessage(HWND msgWindowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_DESTROY:
        {
            this->keepRunning = false;
            this->exitCode = (int)wParam;
            return 0;
        }
    }

    return DefWindowProc(msgWindowHandle, message, wParam, lParam);
}