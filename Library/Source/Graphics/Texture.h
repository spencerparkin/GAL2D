#pragma once

namespace GAL2D
{
	class Image;

	/**
	 * This is the base class for a platform-specific texture that
	 * can be used in rendering.
	 */
	class Texture
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

	public:
		UVMode uvMode;
	};
}