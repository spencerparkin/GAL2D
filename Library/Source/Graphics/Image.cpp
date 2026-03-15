#include "Graphics/Image.h"

using namespace GAL2D;

Image::Image()
{
}

/*virtual*/ Image::~Image()
{
	this->SetDimensions(0, 0);
}

bool Image::Load(const std::string& imagePath)
{
	// STPTODO: Look at extension to create image format read/writer class.
	return false;
}

bool Image::Save(const std::string& imagePath) const
{
	// STPTODO: Look at extension to create image format read/writer class.
	return false;
}

void Image::Clear(const Color& clearColor)
{
	for (int row = 0; row < this->height; row++)
		for (int col = 0; col < this->width; col++)
			this->matrix[row][col] = clearColor;
}

void Image::SetDimensions(int width, int height)
{
	for (int row = 0; row < this->height; row++)
		delete[] this->matrix[row];
	delete[] this->matrix;

	this->width = width;
	this->height = height;

	if (this->width > 0 && this->height > 0)
	{
		this->matrix = new Color*[this->height];
		for (int row = 0; row < this->height; row++)
			this->matrix[row] = new Color[this->width];
	}
}

int Image::GetWidth() const
{
	return this->width;
}

int Image::GetHeight() const
{
	return this->height;
}

bool Image::SetPixel(int row, int col, const Color& color)
{
	if (0 <= row && row < this->height && 0 <= col && col < this->width)
	{
		this->matrix[row][col] = color;
		return true;
	}

	return false;
}

bool Image::GetPixel(int row, int col, Color& color) const
{
	if (0 <= row && row < this->height && 0 <= col && col < this->width)
	{
		color = this->matrix[row][col];
		return true;
	}

	return false;
}