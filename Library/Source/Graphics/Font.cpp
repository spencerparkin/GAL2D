#include "Graphics/Font.h"
#include "Graphics/Image.h"
#include "Graphics/Texture.h"
#include "ft2build.h"
#include "freetype/freetype.h"

using namespace GAL2D;

Font::Font(std::shared_ptr<Texture> texture)
{
	this->texture = texture;
}

/*virtual*/ Font::~Font()
{
}

bool Font::Load(const std::string& fontPath)
{
	bool success = false;
	FT_Library library = nullptr;
	FT_Face face = nullptr;

	// STPTODO: Add support for kerning later.

	do
	{
		FT_Error error = FT_Init_FreeType(&library);
		if (error)
			break;

		error = FT_New_Face(library, fontPath.c_str(), 0, &face);
		if (error)
			break;

		error = FT_Set_Pixel_Sizes(face, 45, 45);
		if (error)
			break;

		this->glyphInfoMap.clear();

		Image atlasImage;
		atlasImage.SetDimensions(1024, 1024);
		atlasImage.Clear(Color(1.0, 1.0, 1.0, 0.0));

		int charWidth = 64;
		int charHeight = 64;

		int numAtlasRows = atlasImage.GetHeight() / charHeight;
		int numAtlasCols = atlasImage.GetWidth() / charWidth;

		if (256 > numAtlasRows * numAtlasCols)
			break;

		FT_UInt i;
		for (i = 0; i < 256; i++)
		{
			FT_UInt glyphIndex = FT_Get_Char_Index(face, i);

			error = FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT);
			if (error)
				break;

			if (face->glyph->format != FT_GLYPH_FORMAT_BITMAP)
			{
				error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
				if (error)
					break;
			}

			int atlasRow = i / numAtlasCols;
			int atlasCol = i % numAtlasCols;

			int atlasRowOffset = atlasRow * charHeight;
			int atlasColOffset = atlasCol * charWidth;

			FT_GlyphSlot slot = face->glyph;

			// Calculate the UV-space rectangle that fits the glyph.
			GlyphInfo glyphInfo{};
			glyphInfo.uvRect.minCorner.x = double(atlasColOffset) / double(atlasImage.GetWidth());
			glyphInfo.uvRect.minCorner.y = 1.0 - double(atlasRowOffset + slot->bitmap.rows) / double(atlasImage.GetHeight());
			glyphInfo.uvRect.maxCorner.x = double(atlasColOffset + slot->bitmap.width) / double(atlasImage.GetWidth());
			glyphInfo.uvRect.maxCorner.y = 1.0 - double(atlasRowOffset) / double(atlasImage.GetHeight());

			// We're going to work with these variables in UV-space as well.
			glyphInfo.penOffset.x = double(slot->bitmap_left) / double(atlasImage.GetWidth());
			glyphInfo.penOffset.y = (double(slot->bitmap_top) - double(slot->bitmap.rows)) / double(atlasImage.GetHeight());
			glyphInfo.penAdvance.x = double(slot->advance.x / 64) / double(atlasImage.GetWidth());
			glyphInfo.penAdvance.y = double(slot->advance.y / 64) / double(atlasImage.GetHeight());

			this->glyphInfoMap.insert(std::pair(char(i), glyphInfo));

			// Some glyphs (such as spaces) don't have any associated pixel data.
			if (!slot->bitmap.buffer)
				continue;
			
			FT_UInt bytesPerGlyphPixel = 0;
			if (slot->bitmap.pixel_mode == FT_PIXEL_MODE_GRAY)
				bytesPerGlyphPixel = 1;

			if (bytesPerGlyphPixel == 0)
				break;

			for (int row = 0; row < (int)slot->bitmap.rows; row++)
			{
				for (int col = 0; col < (int)slot->bitmap.width; col++)
				{
					unsigned char* glyphPixel = &slot->bitmap.buffer[row * slot->bitmap.pitch + col * bytesPerGlyphPixel];

					Color atlasPixel;

					switch (slot->bitmap.pixel_mode)
					{
						case FT_PIXEL_MODE_GRAY:
						{
							double alpha = double(*glyphPixel) / double(slot->bitmap.num_grays);

							if (alpha > 1.0)
								alpha = 1.0;
							else if (alpha < 0.0)
								alpha = 0.0;

							atlasPixel.r = 1.0;
							atlasPixel.g = 1.0;
							atlasPixel.b = 1.0;
							atlasPixel.a = alpha;

							break;
						}
					}

					atlasImage.SetPixel(atlasRowOffset + row, atlasColOffset + col, atlasPixel);
				}
			}
		}

		if (i < 256)
			break;

		if (!this->texture->FromImage(atlasImage))
			break;

		success = true;
	} while (false);

	if (face)
	{
		FT_Done_Face(face);
		face = nullptr;
	}

	if (library)
	{
		FT_Done_FreeType(library);
		library = nullptr;
	}

	return success;
}

std::shared_ptr<Texture> Font::GetAtlasTexture()
{
	return this->texture;
}

bool Font::GetGlyphInfo(char glyphChar, GlyphInfo& glyphInfo) const
{
	std::unordered_map<char, GlyphInfo>::const_iterator iter = this->glyphInfoMap.find(glyphChar);
	if (iter == this->glyphInfoMap.cend())
		return false;

	glyphInfo = iter->second;
	return true;
}

bool Font::MakeGlyphQuadArray(const std::string& text, std::vector<GlyphQuad>& quadArray, Rectangle& textBounds) const
{
	Vector penLocation(0.0, 0.0);

	for (int i = 0; text.c_str()[i] != '\0'; i++)
	{
		char glyphChar = text.c_str()[i];

		Font::GlyphInfo glyphInfo;
		if (!this->GetGlyphInfo(glyphChar, glyphInfo))
			return false;

		GlyphQuad quad;
		quad.localRect = glyphInfo.uvRect + (penLocation + glyphInfo.penOffset - glyphInfo.uvRect.minCorner);
		quad.roomRect = glyphInfo.uvRect + (penLocation - glyphInfo.uvRect.minCorner);
		quad.uvRect = glyphInfo.uvRect;

		quadArray.push_back(quad);

		penLocation += glyphInfo.penAdvance;
	}

	for (const GlyphQuad& quad : quadArray)
		textBounds.MinimallyExpandToIncludeRect(quad.roomRect);

	return true;
}

bool Font::CalcTextWidth(const std::string& text, double textHeight, double& textWidth) const
{
	Rectangle textBounds;
	std::vector<GlyphQuad> quadArray;
	if (!this->MakeGlyphQuadArray(text, quadArray, textBounds))
		return false;

	textWidth = textBounds.Width() * (textHeight / textBounds.Height());
	return true;
}