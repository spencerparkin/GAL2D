#include "Math/Color.h"

using namespace GAL2D;

Color::Color()
{
	this->r = 1.0;
	this->g = 1.0;
	this->b = 1.0;
	this->a = 1.0;
}

Color::Color(double r, double g, double b, double a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

Color::Color(const Color& color)
{
	this->r = color.r;
	this->g = color.g;
	this->b = color.b;
	this->a = color.a;
}

/*virtual*/ Color::~Color()
{
}

void Color::operator=(const Color& color)
{
	this->r = color.r;
	this->g = color.g;
	this->b = color.b;
	this->a = color.a;
}

namespace GAL2D
{
	Color operator*(const Color& colorA, const Color& colorB)
	{
		return Color(
			colorA.r * colorB.r,
			colorA.g * colorB.g,
			colorA.b * colorB.b,
			colorA.a * colorB.a
		);
	}
}