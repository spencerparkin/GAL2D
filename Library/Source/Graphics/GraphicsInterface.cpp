#include "Graphics/GraphicsInterface.h"
#include "Graphics/Texture.h"
#include "Graphics/Image.h"
#include "Graphics/Font.h"

using namespace GAL2D;

GraphicsInterface::GraphicsInterface()
{
}

/*virtual*/ GraphicsInterface::~GraphicsInterface()
{
}

/*virtual*/ std::shared_ptr<Texture> GraphicsInterface::MakeTexture(const std::string& texturePath)
{
	std::shared_ptr<Texture> texture = this->CreateNewTextureObject();
	
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

/*virtual*/ std::shared_ptr<Font> GraphicsInterface::MakeFont(const std::string& fontPath)
{
	std::shared_ptr<Font> font;
	std::shared_ptr<Texture> texture = this->CreateNewTextureObject();

	if (texture.get())
	{
		font = std::make_shared<Font>(texture);
		
		if (!font->Load(fontPath))
		{
			font.reset();
		}
	}

	return font;
}

/*virtual*/ bool GraphicsInterface::CaptureRegion(const Rectangle& region, Image& image)
{
	return false;
}