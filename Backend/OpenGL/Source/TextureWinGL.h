#pragma once

#include "Graphics/Texture.h"
#include <Windows.h>
#include <gl/GL.h>

namespace GAL2D
{
	/**
	 * Implement textures for use in GAL2D with OpenGL on Windows.
	 */
	class TextureWinGL : public Texture
	{
	public:
		TextureWinGL();
		virtual ~TextureWinGL();

		virtual bool FromImage(const Image& image) override;
		virtual bool ToImage(Image& image) const override;
		virtual Vector GetSize() const override;

		GLuint GetTexture() const;

	private:
		GLuint texture;
		GLuint width;
		GLuint height;
	};
}