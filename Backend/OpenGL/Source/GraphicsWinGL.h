#pragma once

#include "GraphicsMSWindows.h"
#include "Math/Rectangle.h"
#include <Windows.h>

namespace GAL2D
{
	/**
	 * Implement GAL2D's graphics interface with OpenGL on Windows.
	 */
	class GraphicsWinGL : public GraphicsMSWindows
	{
	public:
		GraphicsWinGL(const DriverInitData* driverInitData);
		virtual ~GraphicsWinGL();

		virtual bool Setup() override;
		virtual void Shutdown() override;
		virtual std::shared_ptr<Texture> CreateNewTextureObject() override;
		virtual void RenderConvexPolygon(const std::vector<Vertex>& vertexArray, const AffineTransform& worldTransform, std::shared_ptr<Texture> texture = nullptr) override;
		virtual bool RenderText(const std::string& text, std::shared_ptr<Font> font, const Rectangle& rectangle, const Color& color, TextAlign textAlign = ALIGN_CENTER) override;
		virtual bool CaptureRegion(const Rectangle& region, Image& image) override;
		virtual void BeginRendering() override;
		virtual void EndRendering() override;

	private:
		
		HDC deviceContext;
		HGLRC openGLContext;
	};
}