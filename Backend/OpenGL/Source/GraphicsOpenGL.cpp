#include "GraphicsOpenGL.h"
#include "TextureOpenGL.h"
#include <gl/GL.h>

using namespace GAL2D;

GraphicsOpenGL::GraphicsOpenGL(HWND windowHandle)
{
	this->windowHandle = windowHandle;
	this->deviceContext = NULL;
	this->openGLContext = NULL;
}

/*virtual*/ GraphicsOpenGL::~GraphicsOpenGL()
{
}

/*virtual*/ bool GraphicsOpenGL::Setup()
{
	if (this->deviceContext != NULL || this->openGLContext != NULL)
		return false;

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

	glClearColor(0.5, 0.5, 0.5, 1.0f);

	return true;
}

/*virtual*/ void GraphicsOpenGL::Shutdown()
{
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

/*virtual*/ std::shared_ptr<Texture> GraphicsOpenGL::CreateNewBlankTexture()
{
	return std::make_shared<TextureOpenGL>();
}

/*virtual*/ void GraphicsOpenGL::RenderConvexPolygon(const std::vector<Vertex>& vertexArray, std::shared_ptr<Texture> texture /*= nullptr*/)
{
	// STPTODO: Write this.
}

/*virtual*/ bool GraphicsOpenGL::RenderText(const std::string& text, const std::string& font, const Rectangle& rectangle, const Color& color)
{
	// STPTODO: Write this.
	return false;
}

/*virtual*/ bool GraphicsOpenGL::CaptureRegion(const Rectangle& region, Image& image)
{
	// STPTODO: Write this.
	return false;
}

/*virtual*/ void GraphicsOpenGL::BeginRendering()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/*virtual*/ void GraphicsOpenGL::EndRendering()
{
	SwapBuffers(this->deviceContext);
}