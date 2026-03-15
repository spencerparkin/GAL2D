#include "Graphics/Texture.h"

using namespace GAL2D;

Texture::Texture()
{
	this->uvMode = UVMode::Wrap;
}

/*virtual*/ Texture::~Texture()
{
}