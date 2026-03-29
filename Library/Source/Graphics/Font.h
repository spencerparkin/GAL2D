#pragma once

#include "Math/Rectangle.h"
#include "Graphics/Resource.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace GAL2D
{
	class Texture;

	/**
	 * A font is simply a texture atlas coupled with the meta-data needed
	 * to render out of that altas.
	 */
	class Font : public Resource
	{
	public:
		Font(std::shared_ptr<Texture> texture);
		virtual ~Font();

		bool Load(const std::string& fontPath);

		std::shared_ptr<Texture> GetAtlasTexture();

		struct GlyphInfo
		{
			Rectangle uvRect;
			Vector penOffset;
			Vector penAdvance;
		};

		struct GlyphQuad
		{
			Rectangle localRect;	// This is the actual quad (once transformed from UV-space to world space) that should be drawn.
			Rectangle roomRect;		// This is the quad that takes up space where we need to make room for the text.
			Rectangle uvRect;		// This is the sub-region of the texture atlas where the glyph texels live.
		};

		bool GetGlyphInfo(char glyphChar, GlyphInfo& glyphInfo) const;
		bool MakeGlyphQuadArray(const std::string& text, std::vector<GlyphQuad>& quadArray, Rectangle& textBounds) const;

		/**
		 * Calculate the width of the given text, provided it is render in this font at the given height.
		 */
		bool CalcTextWidth(const std::string& text, double textHeight, double& textWidth) const;

	private:
		std::shared_ptr<Texture> texture;
		std::unordered_map<char, GlyphInfo> glyphInfoMap;
	};
}