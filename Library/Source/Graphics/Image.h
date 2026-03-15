#pragma once

#include "Math/Color.h"
#include <string>

namespace GAL2D
{
	/**
	 * These are platform-independent color matrices that can be used to
	 * intialize textures or any general-purpose image processing.
	 */
	class Image
	{
	public:
		Image();
		virtual ~Image();

		bool Load(const std::string& imagePath);
		bool Save(const std::string& imagePath) const;
		void Clear(const Color& clearColor);
		void SetDimensions(int width, int height);
		int GetWidth() const;
		int GetHeight() const;
		bool SetPixel(int row, int col, const Color& color);
		bool GetPixel(int row, int col, Color& color) const;

	private:
		Color** matrix;
		int width;
		int height;
	};
}