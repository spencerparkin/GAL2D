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
	 * This is meant to be a base class from which MS-windows-based drivers can derive.
	 */
	class GraphicsMSWindows : public GraphicsInterface
	{
	public:
		GraphicsMSWindows(const DriverInitData* driverInitData);
		virtual ~GraphicsMSWindows();

		virtual bool Setup() override;
		virtual void Shutdown() override;
		virtual bool HandleEvents() override;
		virtual void BeginRendering() override;
		virtual void EndRendering() override;

	private:
		static LRESULT CALLBACK WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);

		LRESULT HandleMessage(HWND msgWindowHandle, UINT message, WPARAM wParam, LPARAM lParam);

		Vector CalcWorldMousePos(LPARAM lParam);

	protected:
		std::string windowTitle;
		int windowWidth;
		int windowHeight;
		HINSTANCE instanceHandle;
		HWND windowHandle;
		bool exitSignaled;
	};
}