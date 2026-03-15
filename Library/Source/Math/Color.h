#pragma once

namespace GAL2D
{
	/**
	 * 
	 */
	class Color
	{
	public:
		Color();
		Color(double r, double g, double b, double a);
		Color(const Color& color);
		virtual ~Color();

		void operator=(const Color& color);

	public:
		double r, g, b, a;
	};

	Color operator*(const Color& colorA, const Color& colorB);
}