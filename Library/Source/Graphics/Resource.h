#pragma once

#include <memory>

namespace GAL2D
{
	/**
	 * This is the base class for any kind of resource (e.g., font, texture, etc.) that we want to cache in memory.
	 */
	class Resource : public std::enable_shared_from_this<Resource>
	{
	public:
		Resource();
		virtual ~Resource();
	};
}