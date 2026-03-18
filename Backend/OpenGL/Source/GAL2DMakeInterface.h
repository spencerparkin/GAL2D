#pragma once

#include "GraphicsOpenGL.h"

namespace GAL2D
{
	/**
	 * Every backend should have a header named the same as this one, and have a prototype here by the same name and signature.
	 */
	std::shared_ptr<GraphicsInterface> CreateGraphicsInterface(const DriverInitData* driverInitData);
}