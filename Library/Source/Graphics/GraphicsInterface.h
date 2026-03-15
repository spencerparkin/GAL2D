#pragma once

#include "Math/Vector.h"
#include "Math/Rectangle.h"
#include "Math/Color.h"
#include <vector>
#include <string>
#include <memory>

namespace GAL2D
{
	/**
	 * For simplicity, all polygons use this vertex format, even if
	 * not all members are used.
	 */
	struct Vertex
	{
		Vector position;
		Vector UVs;
		Color color;
	};

	class Texture;
	class Image;

	/**
	 * This is a platform-independent interface to a very basic rendering system.
	 * A derivative of this class is meant to be created to provide a platform-specific
	 * implementation of the rendering system.
	 */
	class GraphicsInterface
	{
	public:
		GraphicsInterface();
		virtual ~GraphicsInterface();

		/**
		 * Set the sub-region of the plane where all rendering occurs.
		 * This is used to map from world coordinates into device coordinates.
		 */
		void SetWorldRegion(const Rectangle& worldRegion);

		/**
		 * Return the sub-region of the plane where all renering occurs.
		 */
		const Rectangle& GetWorldRegion() const;

		/**
		 * Set the sub-region of the plane where rendering can occur.
		 * Any rendering that would occur beyond this region is suppressed.
		 */
		void SetClippingRegion(const Rectangle& clippingRegion);

		/**
		 * Return the sub-region of the plane where rendering can occur.
		 */
		const Rectangle& GetClippingRegion() const;

		/**
		 * Get the system ready for rendering.  Whatever the rendering context,
		 * that is up to the underlying driver to configure or decide.
		 * 
		 * @return False is returned here on failure; true on success.
		 */
		virtual bool Setup() = 0;

		/**
		 * This should get called before the graphics interface is disposed.
		 */
		virtual void Shutdown() = 0;

		/**
		 * Load a texture from the given path and return it.
		 * 
		 * @return A valid pointer should be returned here on success; null otherwise.
		 */
		virtual std::shared_ptr<Texture> MakeTexture(const std::string& texturePath);

		/**
		 * Allocate and return a platform-specific texture that, once loaded with texel data, can be used for rendering.
		 */
		virtual std::shared_ptr<Texture> CreateNewBlankTexture() = 0;

		/**
		 * Render a polygon over the top of whatever is in the plane where the given polygon resides.
		 * 
		 * @param[in] vertexArray These are the vertices of the polygon, which is assumed to be convex.
		 * @param[in] texture If given, the UVs of the given vertices are used to map the texture to the polygon.
		 */
		virtual void RenderConvexPolygon(const std::vector<Vertex>& vertexArray, std::shared_ptr<Texture> texture = nullptr) = 0;

		/**
		 * Render text over the top of whatever is in the plane such that it fits inside given rectangle, taking
		 * up as much room as possible within that rectangle.
		 * 
		 * @param[in] text This is the text to render.
		 * @param[in] font This is the named font to use.  The underlying driver is responsible for loading and caching fonts on its own.
		 * @param[in] rectangle This dictates the placement and size of the text in the world.
		 * @param[in] color This is the color to use for the text.
		 * @return False returned here indicates failure, and can happen if, for example, the given font doesn't exit or can't be loaded by the driver.
		 */
		virtual bool RenderText(const std::string& text, const std::string& font, const Rectangle& rectangle, const Color& color) = 0;

		/**
		 * This can be optionally overridden to provide pixel-capture support.
		 * 
		 * @param[in] region This is the sub-region of the plane where pixels should be taken.  It should be a sub-region of the world region.
		 * @param[in,out] image This should get populate with the desired pixel data.  If the image's aspect ratio doesn't match that of the given region, it may be skewed.
		 */
		virtual bool CaptureRegion(const Rectangle& region, Image& image);

		/**
		 * This should be called at the start of a frame before any rendering commands are issued.
		 */
		virtual void BeginRendering() = 0;

		/**
		 * This should be called at the end of a frame after all rendering commands have been issued.
		 * Once called, the next frame should be presented.
		 */
		virtual void EndRendering() = 0;

	protected:

		Rectangle worldRegion;
		Rectangle clippingRegion;
	};
}