#include "Main.h"
#if defined USING_OPENGL
#	include "GraphicsOpenGL.h"
#endif

int WINAPI WinMain(HINSTANCE instanceHandle, HINSTANCE prevInstanceHandle, LPSTR cmdLine, int cmdShow)
{
#if defined USING_OPENGL
	std::shared_ptr<GAL2D::GraphicsInterface> graphics = std::make_shared<GAL2D::GraphicsOpenGL>(instanceHandle, cmdShow, "GAL2D Test App");
#else
#   error Graphics driver not specified or not supported.
#endif

	if (!graphics->Setup())
	{
		MessageBoxA(NULL, "Failed to setup graphics!", "Error!", MB_OK | MB_ICONERROR);
		return -1;
	}

	graphics->renderState.worldRegion.minCorner.x = -10.0;
	graphics->renderState.worldRegion.maxCorner.x = 10.0;
	graphics->renderState.worldRegion.minCorner.y = -10.0;
	graphics->renderState.worldRegion.maxCorner.y = 10.0;

	std::shared_ptr<GAL2D::Texture> texture = graphics->MakeTexture("D:\\git_repos\\GAL2D\\Test\\flower.jpg");

	while (graphics->HandleEvents())
	{
		graphics->BeginRendering();

		std::vector<GAL2D::Vertex> vertexArray;
		vertexArray.push_back({ GAL2D::Vector(-5.0, -5.0), GAL2D::Vector(0.0, 0.0), GAL2D::Color(1.0, 0.0, 0.0, 1.0) });
		vertexArray.push_back({ GAL2D::Vector(5.0, -5.0), GAL2D::Vector(1.0, 0.0), GAL2D::Color(0.0, 1.0, 0.0, 1.0) });
		vertexArray.push_back({ GAL2D::Vector(5.0, 5.0), GAL2D::Vector(1.0, 1.0), GAL2D::Color(0.0, 0.0, 1.0, 1.0) });
		vertexArray.push_back({ GAL2D::Vector(-5.0, 5.0), GAL2D::Vector(0.0, 1.0), GAL2D::Color(1.0, 0.0, 1.0, 1.0) });

		GAL2D::AffineTransform worldTransform;
		worldTransform.MakeIdentity();

		graphics->RenderConvexPolygon(vertexArray, worldTransform, texture);

		graphics->EndRendering();
	}

	graphics->Shutdown();

	return 0;
}