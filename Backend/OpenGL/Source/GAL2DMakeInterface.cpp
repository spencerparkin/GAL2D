#include "GAL2DMakeInterface.h"

namespace GAL2D
{
	std::shared_ptr<GraphicsInterface> CreateGraphicsInterface(const DriverInitData* driverInitData)
	{
		return std::make_shared<GraphicsWinGL>(driverInitData);
	}
}