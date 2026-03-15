#include "Graphics/GraphicsInterface.h"
#include "Graphics/Texture.h"
#include "Graphics/Image.h"

using namespace GAL2D;

GraphicsInterface::GraphicsInterface()
{
}

/*virtual*/ GraphicsInterface::~GraphicsInterface()
{
}

void GraphicsInterface::SetWorldRegion(const Rectangle& worldRegion)
{
	this->worldRegion = worldRegion;
}

const Rectangle& GraphicsInterface::GetWorldRegion() const
{
	return this->worldRegion;
}

void GraphicsInterface::SetClippingRegion(const Rectangle& clippingRegion)
{
	this->clippingRegion = clippingRegion;
}

const Rectangle& GraphicsInterface::GetClippingRegion() const
{
	return this->clippingRegion;
}

/*virtual*/ std::shared_ptr<Texture> GraphicsInterface::MakeTexture(const std::string& texturePath)
{
	std::shared_ptr<Texture> texture = this->CreateNewBlankTexture();
	
	if (texture.get())
	{
		Image image;

		if (!image.Load(texturePath) || !texture->FromImage(image))
		{
			texture.reset();
		}
	}

	return texture;
}

/*virtual*/ bool GraphicsInterface::CaptureRegion(const Rectangle& region, Image& image)
{
	return false;
}