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