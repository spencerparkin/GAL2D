#include "GraphicsOpenGL.h"
#include "TextureOpenGL.h"
#include "Graphics/Font.h"
#include <windowsx.h>
#include <gl/GL.h>
#include <gl/GLU.h>

using namespace GAL2D;

GraphicsOpenGL::GraphicsOpenGL(const DriverInitData* driverInitData)
{
	this->instanceHandle = driverInitData->instanceHandle ? driverInitData->instanceHandle : (HINSTANCE)GetModuleHandle(NULL);
	this->windowTitle = driverInitData->windowTitle;
	this->windowWidth = driverInitData->windowWidth;
	this->windowHeight = driverInitData->windowHeight;
	this->windowHandle = NULL;
	this->deviceContext = NULL;
	this->openGLContext = NULL;
	this->exitSignaled = false;
}

/*virtual*/ GraphicsOpenGL::~GraphicsOpenGL()
{
}

/*virtual*/ bool GraphicsOpenGL::Setup()
{
	if (this->windowHandle != NULL)
		return false;

	WNDCLASSEX winClass{};
	winClass.cbSize = sizeof(WNDCLASSEX);
	winClass.style = CS_HREDRAW | CS_VREDRAW;
	winClass.lpfnWndProc = &GraphicsOpenGL::WndProc;
	winClass.cbClsExtra = 0;
	winClass.cbWndExtra = sizeof(GraphicsOpenGL*);
	winClass.hInstance = this->instanceHandle;
	winClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	winClass.lpszClassName = "GraphicsOpenGLWindow";
	ATOM atom = RegisterClassEx(&winClass);

	this->windowHandle = CreateWindow(winClass.lpszClassName, this->windowTitle.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, this->windowWidth, this->windowHeight, nullptr, nullptr, this->instanceHandle, nullptr);
	if (!this->windowHandle)
		return false;

	SetWindowLongPtr(this->windowHandle, 0, (LONG_PTR)this);

	this->deviceContext = GetDC(this->windowHandle);
	if (this->deviceContext == NULL)
		return false;

	PIXELFORMATDESCRIPTOR pixelFormatDesc{};
	pixelFormatDesc.nSize = sizeof(pixelFormatDesc);
	pixelFormatDesc.nVersion = 1;
	pixelFormatDesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pixelFormatDesc.iPixelType = PFD_TYPE_RGBA;
	pixelFormatDesc.cColorBits = 32;
	pixelFormatDesc.cDepthBits = 24;
	pixelFormatDesc.cStencilBits = 8;

	int format = ChoosePixelFormat(this->deviceContext, &pixelFormatDesc);
	SetPixelFormat(this->deviceContext, format, &pixelFormatDesc);

	this->openGLContext = wglCreateContext(this->deviceContext);
	if (this->openGLContext == NULL)
		return false;

	wglMakeCurrent(this->deviceContext, this->openGLContext);

	glClearColor(0.0, 0.0, 0.0, 1.0f);

	ShowWindow(this->windowHandle, SW_SHOW);
	UpdateWindow(this->windowHandle);

	this->exitSignaled = false;

	return true;
}

/*virtual*/ void GraphicsOpenGL::Shutdown()
{
	GraphicsInterface::Shutdown();

	wglMakeCurrent(NULL, NULL);

	if (this->openGLContext != NULL)
	{
		wglDeleteContext(this->openGLContext);
		this->openGLContext = NULL;
	}

	if (this->deviceContext != NULL)
	{
		ReleaseDC(this->windowHandle, this->deviceContext);
		this->deviceContext = NULL;
	}
}

/*static*/ LRESULT CALLBACK GraphicsOpenGL::WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
	GraphicsOpenGL* graphics = (GraphicsOpenGL*)GetWindowLongPtr(windowHandle, 0);
	if (graphics)
		return graphics->HandleMessage(windowHandle, message, wParam, lParam);

	return DefWindowProc(windowHandle, message, wParam, lParam);
}

LRESULT GraphicsOpenGL::HandleMessage(HWND msgWindowHandle, UINT message, WPARAM wParam, LPARAM lParam)
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
	}

	return DefWindowProc(msgWindowHandle, message, wParam, lParam);
}

Vector GraphicsOpenGL::CalcWorldMousePos(LPARAM lParam)
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

	UVs.y = 1.0 - UVs.y;

	Vector worldMousePos;
	this->adjustedWorldRegion.PointFromUVs(UVs, worldMousePos);

	return worldMousePos;
}

/*virtual*/ bool GraphicsOpenGL::HandleEvents()
{
	MSG msg;
	while (PeekMessage(&msg, this->windowHandle, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return !this->exitSignaled;
}

/*virtual*/ std::shared_ptr<Texture> GraphicsOpenGL::CreateNewTextureObject()
{
	return std::make_shared<TextureOpenGL>();
}

/*virtual*/ void GraphicsOpenGL::RenderConvexPolygon(const std::vector<Vertex>& vertexArray, const AffineTransform& worldTransform, std::shared_ptr<Texture> texture /*= nullptr*/)
{
	//glScissor(...)

	glDisable(GL_BLEND);

	glDisable(GL_TEXTURE_2D);
	bool texturing = false;

	if (texture.get())
	{
		TextureOpenGL* textureGL = dynamic_cast<TextureOpenGL*>(texture.get());
		if (textureGL)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textureGL->GetTexture());
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			texturing = true;
		}
	}

	glBegin(GL_POLYGON);

	if (texturing)
		glColor4d(1.0, 1.0, 1.0, 1.0);

	for (const Vertex& vertex : vertexArray)
	{
		Vector worldPosition = worldTransform * vertex.position;

		if (texturing)
			glTexCoord2d(vertex.UVs.x, vertex.UVs.y);
		else
			glColor4d(vertex.color.r, vertex.color.g, vertex.color.b, vertex.color.a);
		
		glVertex2d(worldPosition.x, worldPosition.y);
	}

	glEnd();
}

/*virtual*/ bool GraphicsOpenGL::RenderText(const std::string& text, std::shared_ptr<Font> font, const Rectangle& rectangle, const Color& color, TextAlign textAlign /*= ALIGN_CENTER*/)
{
	if (!font.get())
		return false;

	std::shared_ptr<Texture> atlasTexture = font->GetAtlasTexture();
	if (!atlasTexture.get())
		return false;

	TextureOpenGL* textureGL = dynamic_cast<TextureOpenGL*>(atlasTexture.get());
	if (!textureGL)
		return false;
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureGL->GetTexture());
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// STPTODO: Some of this code should be pushed into the base class so it can be used by other graphics drivers.

	Vector penLocation(0.0, 0.0);
	
	struct GlyphQuad
	{
		Rectangle localRect;
		Rectangle uvRect;
	};

	std::vector<GlyphQuad> quadArray;

	for (int i = 0; text.c_str()[i] != '\0'; i++)
	{
		char glyphChar = text.c_str()[i];

		Font::GlyphInfo glyphInfo;
		if (!font->GetGlyphInfo(glyphChar, glyphInfo))
			return false;

		GlyphQuad quad;
		quad.localRect = glyphInfo.uvRect + (penLocation + glyphInfo.penOffset - glyphInfo.uvRect.minCorner);
		quad.uvRect = glyphInfo.uvRect;

		quadArray.push_back(quad);

		penLocation += glyphInfo.penAdvance;
	}

	Rectangle textBounds;
	for (const GlyphQuad& quad : quadArray)
		textBounds.MinimallyExpandToIncludeRect(quad.localRect);

	GAL2D::Rectangle::AspectRatioMatchMode aspectRatioMatchMode;
	switch (textAlign)
	{
		default:
		case ALIGN_CENTER:
		{
			aspectRatioMatchMode = GAL2D::Rectangle::DELTA_MIN_AND_MAX;
			break;
		}
		case ALIGN_LEFT:
		{
			aspectRatioMatchMode = GAL2D::Rectangle::ALL_DELTA_MAX;
			break;
		}
		case ALIGN_RIGHT:
		{
			aspectRatioMatchMode = GAL2D::Rectangle::ALL_DELTA_MIN;
			break;
		}
	}

	// Here we calculate the largest rectangle (targetRect) that fits inside
	// the given rectangle (rectangle) but also matches the aspect ratio of
	// the the text bounds.
	Rectangle targetRect = rectangle;
	targetRect.ContractToMatchAspectRatio(textBounds.AspectRatio(), aspectRatioMatchMode);

	AffineTransform worldTransform;
	worldTransform.MakeTransform(textBounds, targetRect);

	glBegin(GL_QUADS);

	glColor4d(color.r, color.g, color.b, color.a);

	for (const GlyphQuad& quad : quadArray)
	{
		Rectangle worldRect = worldTransform * quad.localRect;
		
		glTexCoord2d(quad.uvRect.minCorner.x, quad.uvRect.minCorner.y);
		glVertex2d(worldRect.minCorner.x, worldRect.minCorner.y);

		glTexCoord2d(quad.uvRect.maxCorner.x, quad.uvRect.minCorner.y);
		glVertex2d(worldRect.maxCorner.x, worldRect.minCorner.y);

		glTexCoord2d(quad.uvRect.maxCorner.x, quad.uvRect.maxCorner.y);
		glVertex2d(worldRect.maxCorner.x, worldRect.maxCorner.y);

		glTexCoord2d(quad.uvRect.minCorner.x, quad.uvRect.maxCorner.y);
		glVertex2d(worldRect.minCorner.x, worldRect.maxCorner.y);
	}

	glEnd();

	return true;
}

/*virtual*/ bool GraphicsOpenGL::CaptureRegion(const Rectangle& region, Image& image)
{
	// STPTODO: Write this.
	return false;
}

/*virtual*/ void GraphicsOpenGL::BeginRendering()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	RECT windowRect;
	GetClientRect(this->windowHandle, &windowRect);

	int viewportWidth = windowRect.right - windowRect.left;
	int viewportHeight = windowRect.bottom - windowRect.top;

	glViewport(0, 0, viewportWidth, viewportHeight);

	double aspectRatio = double(viewportWidth) / double(viewportHeight);

	this->adjustedWorldRegion = this->renderState.worldRegion;
	this->adjustedWorldRegion.ExpandToMatchAspectRatio(aspectRatio);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(
		this->adjustedWorldRegion.minCorner.x,
		this->adjustedWorldRegion.maxCorner.x,
		this->adjustedWorldRegion.minCorner.y,
		this->adjustedWorldRegion.maxCorner.y);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
}

/*virtual*/ void GraphicsOpenGL::EndRendering()
{
	glFlush();

	SwapBuffers(this->deviceContext);
}

/*virtual*/ bool GraphicsOpenGL::GetScreenResolution(Vector& screenSize)
{
	RECT clientRect;
	if (!GetClientRect(this->windowHandle, &clientRect))
		return false;

	screenSize.x = double(clientRect.right - clientRect.left);
	screenSize.y = double(clientRect.bottom - clientRect.top);
	return true;
}