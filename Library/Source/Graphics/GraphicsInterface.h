#pragma once

#include "Math/Vector.h"
#include "Math/Rectangle.h"
#include "Math/Color.h"
#include "Math/AffineTransform.h"
#include <vector>
#include <string>
#include <memory>
#include <functional>

// STPTODO: Maybe leverage code in font-system I had made years ago to make image and char-to-uv map.  Use freetype lib.

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

	/**
	 * This structure holds all state that presists between render calls
	 * until a new render state is desired.
	 */
	struct RenderState
	{
		Rectangle worldRegion;
		Rectangle clippingRegion;
	};

	enum MouseButton
	{
		Left,
		Right,
		Middle
	};

	enum ButtonState
	{
		Up,
		Down
	};

	typedef std::function<void(const Vector&, MouseButton, ButtonState)> MouseEventHandler;
	typedef std::function<void(int, ButtonState)> KeyboardEventHandler;

	class Texture;
	class Image;
	class Font;

	/**
	 * This is a platform-independent interface to a very basic rendering system.
	 * A derivatives of this class are meant to provide a platform-specific
	 * implementation of the rendering system.
	 * 
	 * The system isn't just for output to the screen.  It also handles input from
	 * the keyboard and mouse.  It makes sense to handle input here too, because some
	 * of that is in the context of the world region, such as where a mouse is clicked.
	 */
	class GraphicsInterface
	{
	public:
		GraphicsInterface();
		virtual ~GraphicsInterface();

		enum TextAlign
		{
			ALIGN_LEFT,
			ALIGN_RIGHT,
			ALIGN_CENTER
		};

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
		 * This is called to detect and dispatch keyboard and mouse events.  False is returned
		 * when it is time for the program to exit.
		 */
		virtual bool HandleEvents() = 0;

		/**
		 * Load a texture from the given path and return it.
		 * 
		 * @return A valid pointer should be returned here on success; null otherwise.
		 */
		virtual std::shared_ptr<Texture> MakeTexture(const std::string& texturePath);

		/**
		 * Load a font from the given path and return it.
		 * 
		 * @return A valid pointer should be returned here on success; null otherwise.
		 */
		virtual std::shared_ptr<Font> MakeFont(const std::string& fontPath);

		/**
		 * Allocate and return a platform-specific texture object that, once loaded with texel data, can be used for rendering.
		 */
		virtual std::shared_ptr<Texture> CreateNewTextureObject() = 0;

		/**
		 * Render a polygon over the top of whatever is in the plane where the given polygon resides.
		 * 
		 * @param[in] vertexArray These are the vertices of the polygon, which is assumed to be convex.
		 * @param[in] worldTransform This is the transform applied to the vertices to get them into world space.
		 * @param[in] texture If given, the UVs of the given vertices are used to map the texture to the polygon.
		 */
		virtual void RenderConvexPolygon(const std::vector<Vertex>& vertexArray, const AffineTransform& worldTransform, std::shared_ptr<Texture> texture = nullptr) = 0;

		/**
		 * This is provided for convenience and just calls @ref RenderConvexPolygon under the hood.
		 * 
		 * @param[in] rectangle This is the world rectangle to draw.
		 * @param[in] color This is the solid color to make the rectangle unless a texture is given.
		 * @param[in] texture If non-null, this is used to draw the rectangle.
		 */
		void RenderRectangle(const Rectangle& rectangle, const Color& color, std::shared_ptr<Texture> texture = nullptr);

		/**
		 * Render text over the top of whatever is in the plane such that it fits inside the given rectangle, taking
		 * up as much room as possible within that rectangle.
		 * 
		 * @param[in] text This is the text to render.
		 * @param[in] font This is the font to use when rendering the text.  See @ref MakeFont.
		 * @param[in] rectangle This dictates the placement and size of the text in the world.
		 * @param[in] color This is the color to use for the text.
		 * @return False returned here indicates failure, and can happen if, for example, the given font doesn't exit or can't be loaded by the driver.
		 */
		virtual bool RenderText(const std::string& text, std::shared_ptr<Font> font, const Rectangle& rectangle, const Color& color, TextAlign textAlign = ALIGN_CENTER) = 0;

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

		/**
		 * @param[out] screenSize Will hold the screen resolution.
		 * @return True is returne if and only if the function is successful.
		 */
		virtual bool GetScreenResolution(Vector& screenSize) = 0;

	public:
		MouseEventHandler mouseEventHandler;
		KeyboardEventHandler keyboardEventHandler;
		RenderState renderState;
	};
}