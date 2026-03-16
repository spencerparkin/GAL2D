#include "Graphics/Image.h"
#include "ThirdParty/stb_image.h"

using namespace GAL2D;

Image::Image()
{
	this->width = 0;
	this->height = 0;
	this->matrix = nullptr;
}

/*virtual*/ Image::~Image()
{
	this->SetDimensions(0, 0);
}

bool Image::Load(const std::string& imagePath)
{
	bool success = false;
	unsigned char* pixelData = nullptr;

	do
	{
		int numChannels = 0;
		int imageWidth = 0;
		int imageHeight = 0;
		
		pixelData = stbi_load(imagePath.c_str(), &imageWidth, &imageHeight, &numChannels, 0);
		if (!pixelData)
			break;

		if (imageWidth <= 0 || imageHeight <= 0)
			break;

		if (numChannels < 1 || numChannels > 4)
			break;

		this->SetDimensions(imageWidth, imageHeight);

		unsigned char* pixel = pixelData;

		for (int row = 0; row < this->height; row++)
		{
			for (int col = 0; col < this->width; col++)
			{
				Color& color = this->matrix[row][col];

				switch (numChannels)
				{
					case 1:
					case 2:
					{
						color.r = double(pixel[0]) / 255.0;
						color.g = double(pixel[0]) / 255.0;
						color.b = double(pixel[0]) / 255.0;
						color.a = (numChannels == 1) ? 1.0 : (double(pixel[1]) / 255.0);
						break;
					}
					case 3:
					case 4:
					{
						color.r = double(pixel[0]) / 255.0;
						color.g = double(pixel[1]) / 255.0;
						color.b = double(pixel[2]) / 255.0;
						color.a = (numChannels == 4) ? 1.0 : (double(pixel[3]) / 255.0);
						break;
					}
				}

				pixel += numChannels;
			}
		}

		success = true;
	} while (false);

	if (pixelData)
		stbi_image_free(pixelData);

	return success;
}

bool Image::Save(const std::string& imagePath) const
{
	// STPTODO: Write this.
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