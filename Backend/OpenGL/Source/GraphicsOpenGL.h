#pragma once

#include "Graphics/GraphicsInterface.h"
#include <Windows.h>

namespace GAL2D
{
	/**
	 * Implement GAL2D's graphics interface with OpenGL.
	 */
	class GraphicsOpenGL : public GraphicsInterface
	{
	public:
		GraphicsOpenGL(HWND windowHandle);
		virtual ~GraphicsOpenGL();

		virtual bool Setup() override;
		virtual void Shutdown() override;
		virtual std::shared_ptr<Texture> CreateNewBlankTexture() override;
		virtual void RenderConvexPolygon(const std::vector<Vertex>& vertexArray, std::shared_ptr<Texture> texture = nullptr) override;
		virtual bool RenderText(const std::string& text, const std::string& font, const Rectangle& rectangle, const Color& color) override;
		virtual bool CaptureRegion(const Rectangle& region, Image& image) override;
		virtual void BeginRendering() override;
		virtual void EndRendering() override;

	private:
		HWND windowHandle;
		HDC deviceContext;
		HGLRC openGLContext;
	};
}