#include "TextureOpenGL.h"

using namespace GAL2D;

TextureOpenGL::TextureOpenGL()
{
	this->texture = 0;
}

/*virtual*/ TextureOpenGL::~TextureOpenGL()
{
}

/*virtual*/ bool TextureOpenGL::FromImage(const Image& image)
{
	// STPTODO: Write this.
	return false;
}

/*virtual*/ bool TextureOpenGL::ToImage(Image& image) const
{
	// STPTODO: Write this.
	return false;
}