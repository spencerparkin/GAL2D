#pragma once

#include "Graphics/GraphicsInterface.h"
#include "Math/Rectangle.h"
#include <Windows.h>

namespace GAL2D
{
	/**
	 * Every backend driver should make a structure with this very same name.
	 * The contents can very, but the structure name should be the same.
	 */
	struct DriverInitData
	{
		HINSTANCE instanceHandle;
		std::string windowTitle;
		int windowWidth;
		int windowHeight;
	};

	/**
	 * Implement GAL2D's graphics interface with OpenGL on Windows.
	 */
	class GraphicsOpenGL : public GraphicsInterface
	{
	public:
		GraphicsOpenGL(const DriverInitData* driverInitData);
		virtual ~GraphicsOpenGL();

		virtual bool Setup() override;
		virtual void Shutdown() override;
		virtual bool HandleEvents() override;
		virtual std::shared_ptr<Texture> CreateNewTextureObject() override;
		virtual void RenderConvexPolygon(const std::vector<Vertex>& vertexArray, const AffineTransform& worldTransform, std::shared_ptr<Texture> texture = nullptr) override;
		virtual bool RenderText(const std::string& text, std::shared_ptr<Font> font, const Rectangle& rectangle, const Color& color) override;
		virtual bool CaptureRegion(const Rectangle& region, Image& image) override;
		virtual void BeginRendering() override;
		virtual void EndRendering() override;

	private:
		static LRESULT CALLBACK WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);

		LRESULT HandleMessage(HWND msgWindowHandle, UINT message, WPARAM wParam, LPARAM lParam);

		Vector CalcWorldMousePos(LPARAM lParam);

	private:
		std::string windowTitle;
		int windowWidth;
		int windowHeight;
		HINSTANCE instanceHandle;
		HWND windowHandle;
		HDC deviceContext;
		HGLRC openGLContext;
		Rectangle adjustedWorldRegion;
		bool exitSignaled;
	};
}