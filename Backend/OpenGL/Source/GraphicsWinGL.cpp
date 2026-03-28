#include "GraphicsWinGL.h"
#include "TextureWinGL.h"
#include "Graphics/Font.h"
#include <gl/GL.h>
#include <gl/GLU.h>

using namespace GAL2D;

GraphicsWinGL::GraphicsWinGL(const DriverInitData* driverInitData) : GraphicsMSWindows(driverInitData)
{
	this->deviceContext = NULL;
	this->openGLContext = NULL;
}

/*virtual*/ GraphicsWinGL::~GraphicsWinGL()
{
}

/*virtual*/ bool GraphicsWinGL::Setup()
{
	if (!GraphicsMSWindows::Setup())
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

	glClearColor(0.0, 0.0, 0.0, 1.0f);

	ShowWindow(this->windowHandle, SW_SHOW);
	UpdateWindow(this->windowHandle);

	return true;
}

/*virtual*/ void GraphicsWinGL::Shutdown()
{
	GraphicsMSWindows::Shutdown();

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

/*virtual*/ std::shared_ptr<Texture> GraphicsWinGL::CreateNewTextureObject()
{
	return std::make_shared<TextureWinGL>();
}

/*virtual*/ void GraphicsWinGL::RenderConvexPolygon(const std::vector<Vertex>& vertexArray, const AffineTransform& worldTransform, std::shared_ptr<Texture> texture /*= nullptr*/)
{
	//glScissor(...)

	glDisable(GL_BLEND);

	glDisable(GL_TEXTURE_2D);
	bool texturing = false;

	if (texture.get())
	{
		TextureWinGL* textureGL = dynamic_cast<TextureWinGL*>(texture.get());
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

/*virtual*/ bool GraphicsWinGL::RenderText(const std::string& text, std::shared_ptr<Font> font, const Rectangle& rectangle, const Color& color, TextAlign textAlign /*= ALIGN_CENTER*/)
{
	if (!font.get())
		return false;

	std::shared_ptr<Texture> atlasTexture = font->GetAtlasTexture();
	if (!atlasTexture.get())
		return false;

	TextureWinGL* textureGL = dynamic_cast<TextureWinGL*>(atlasTexture.get());
	if (!textureGL)
		return false;
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureGL->GetTexture());
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	std::vector<GAL2D::Font::GlyphQuad> quadArray;
	GAL2D::Rectangle textBounds;
	if (!font->MakeGlyphQuadArray(text, quadArray, textBounds))
		return false;

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

	for (const GAL2D::Font::GlyphQuad& quad : quadArray)
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

/*virtual*/ bool GraphicsWinGL::CaptureRegion(const Rectangle& region, Image& image)
{
	// STPTODO: Write this.
	return false;
}

/*virtual*/ void GraphicsWinGL::BeginRendering()
{
	GraphicsMSWindows::BeginRendering();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	RECT windowRect;
	GetClientRect(this->windowHandle, &windowRect);

	int viewportWidth = windowRect.right - windowRect.left;
	int viewportHeight = windowRect.bottom - windowRect.top;

	glViewport(0, 0, viewportWidth, viewportHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(
		this->renderState.worldRegion.minCorner.x,
		this->renderState.worldRegion.maxCorner.x,
		this->renderState.worldRegion.minCorner.y,
		this->renderState.worldRegion.maxCorner.y);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
}

/*virtual*/ void GraphicsWinGL::EndRendering()
{
	GraphicsMSWindows::EndRendering();

	glFlush();

	SwapBuffers(this->deviceContext);
}