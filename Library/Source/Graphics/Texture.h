#pragma once

#include "Math/Vector.h"
#include "Graphics/Resource.h"

namespace GAL2D
{
	class Image;

	/**
	 * This is the base class for a platform-specific texture that
	 * can be used in rendering.
	 */
	class Texture : public Resource
	{
	public:
		enum UVMode
		{
			Clip,
			Wrap
		};

		Texture();
		virtual ~Texture();

		virtual bool FromImage(const Image& image) = 0;
		virtual bool ToImage(Image& image) const = 0;
		virtual Vector GetSize() const = 0;

	public:
		UVMode uvMode;
	};
}