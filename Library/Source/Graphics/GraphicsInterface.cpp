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

/*virtual*/ void GraphicsInterface::Shutdown()
{
	this->resourceCache.clear();
}

bool GraphicsInterface::ResolveResourcePathAndKey(std::string& resourceKey, std::filesystem::path& resourcePath) const
{
	if (resourcePath.is_relative())
		resourcePath = this->resourceBasePath / resourcePath;

	if (!std::filesystem::exists(resourcePath))
		return false;

	resourceKey = resourcePath.string();

	std::transform(resourceKey.begin(), resourceKey.end(), resourceKey.begin(), [](unsigned char ch)
		{
			if (ch == '\\')
				return '/';

			return (char)std::tolower(ch);
		}
	);

	return true;
}

/*virtual*/ std::shared_ptr<Texture> GraphicsInterface::MakeTexture(const std::filesystem::path& texturePath, bool canUseCache /*= true*/)
{
	std::filesystem::path resourcePath = texturePath;
	std::string resourceKey;
	std::shared_ptr<Texture> texture;

	if (this->ResolveResourcePathAndKey(resourceKey, resourcePath))
	{
		if (canUseCache)
			texture = this->FindResource<Texture>(resourceKey);

		if (!texture.get())
		{
			texture = this->CreateNewTextureObject();
			if (texture.get())
			{
				Image image;

				if (!image.Load(resourcePath.string()) || !texture->FromImage(image))
					texture.reset();
				else
					this->resourceCache.insert(std::pair(resourceKey, texture));
			}
		}
	}

	return texture;
}

/*virtual*/ std::shared_ptr<Font> GraphicsInterface::MakeFont(const std::filesystem::path& fontPath, bool canUseCache /*= true*/)
{
	std::filesystem::path resourcePath = fontPath;
	std::string resourceKey;
	std::shared_ptr<Font> font;

	if (this->ResolveResourcePathAndKey(resourceKey, resourcePath))
	{
		if (canUseCache)
			font = this->FindResource<Font>(resourceKey);

		if (!font.get())
		{
			std::shared_ptr<Texture> texture = this->CreateNewTextureObject();
			if (texture.get())
			{
				font = std::make_shared<Font>(texture);

				if (!font->Load(resourcePath.string()))
					font.reset();
				else
					this->resourceCache.insert(std::pair(resourceKey, font));
			}
		}
	}

	return font;
}

/*virtual*/ bool GraphicsInterface::CaptureRegion(const Rectangle& region, Image& image)
{
	return false;
}

void GraphicsInterface::RenderRectangle(const Rectangle& rectangle, const Color& color, std::shared_ptr<Texture> texture /*= nullptr*/)
{
	std::vector<Vertex> vertexArray;
	vertexArray.push_back({ Vector(rectangle.minCorner.x, rectangle.minCorner.y), Vector(0.0, 0.0), color });
	vertexArray.push_back({ Vector(rectangle.maxCorner.x, rectangle.minCorner.y), Vector(1.0, 0.0), color });
	vertexArray.push_back({ Vector(rectangle.maxCorner.x, rectangle.maxCorner.y), Vector(1.0, 1.0), color });
	vertexArray.push_back({ Vector(rectangle.minCorner.x, rectangle.maxCorner.y), Vector(0.0, 1.0), color });

	this->RenderConvexPolygon(vertexArray, AffineTransform(), texture);
}

void GraphicsInterface::SetResourceBasePath(const std::filesystem::path& resourceBasePath)
{
	this->resourceBasePath = resourceBasePath;
}

const std::filesystem::path& GraphicsInterface::GetResourceBasePath() const
{
	return this->resourceBasePath;
}