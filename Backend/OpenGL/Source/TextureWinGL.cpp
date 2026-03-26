#include "TextureWinGL.h"
#include "Graphics/Image.h"
#include <memory>

using namespace GAL2D;

TextureWinGL::TextureWinGL()
{
	this->texture = 0;
	this->width = 0;
	this->height = 0;
}

/*virtual*/ TextureWinGL::~TextureWinGL()
{
	if (this->texture != 0)
		glDeleteTextures(1, &this->texture);
}

/*virtual*/ bool TextureWinGL::FromImage(const Image& image)
{
	if (image.GetHeight() == 0 || image.GetWidth() == 0)
		return false;

	this->width = image.GetWidth();
	this->height = image.GetHeight();

	std::unique_ptr<unsigned char> pixelData(new unsigned char[this->width * this->height * 4]);

	unsigned char* pixel = pixelData.get();

	for (int row = 0; row < (int)this->height; row++)
	{
		for (int col = 0; col < (int)this->width; col++)
		{
			Color color;
			image.GetPixel((int)this->height - row - 1, col, color);

			pixel[0] = unsigned char(255.0 * color.r);
			pixel[1] = unsigned char(255.0 * color.g);
			pixel[2] = unsigned char(255.0 * color.b);
			pixel[3] = unsigned char(255.0 * color.a);

			pixel += 4;
		}
	}

	if (this->texture != 0)
	{
		glDeleteTextures(1, &this->texture);
		this->texture = 0;
	}

	glGenTextures(1, &this->texture);
	if (this->texture == 0)
		return false;

	glBindTexture(GL_TEXTURE_2D, this->texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelData.get());

	return true;
}

/*virtual*/ bool TextureWinGL::ToImage(Image& image) const
{
	// STPTODO: Write this.
	return false;
}

/*virtual*/ Vector TextureWinGL::GetSize() const
{
	return Vector((double)this->width, (double)this->height);
}

GLuint TextureWinGL::GetTexture() const
{
	return this->texture;
}