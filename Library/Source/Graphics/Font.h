#pragma once

#include "Math/Rectangle.h"
#include <memory>
#include <string>
#include <unordered_map>

namespace GAL2D
{
	class Texture;

	/**
	 * A font is simply a texture atlas coupled with the meta-data needed
	 * to render out of that altas.
	 */
	class Font
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

		bool GetGlyphInfo(char glyphChar, GlyphInfo& glyphInfo) const;

	private:
		std::shared_ptr<Texture> texture;
		std::unordered_map<char, GlyphInfo> glyphInfoMap;
	};
}