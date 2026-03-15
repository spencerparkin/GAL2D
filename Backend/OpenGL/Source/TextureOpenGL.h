#pragma once

#include "Graphics/Texture.h"

namespace GAL2D
{
	/**
	 * Implement textures for use in GAL2D with OpenGL.
	 */
	class TextureOpenGL : public Texture
	{
	public:
		TextureOpenGL();
		virtual ~TextureOpenGL();

		virtual bool FromImage(const Image& image) override;
		virtual bool ToImage(Image& image) const override;

	private:
		int texture;
	};
}