#include "Math/Rectangle.h"

using namespace GAL2D;

Rectangle::Rectangle()
{
}

Rectangle::Rectangle(const Vector& minCorner, const Vector& maxCorner)
{
	this->minCorner = minCorner;
	this->maxCorner = maxCorner;
}

Rectangle::Rectangle(const Rectangle& rectangle)
{
	this->minCorner = rectangle.minCorner;
	this->maxCorner = rectangle.maxCorner;
}

/*virtual*/ Rectangle::~Rectangle()
{
}

void Rectangle::operator=(const Rectangle& rectangle)
{
	this->minCorner = rectangle.minCorner;
	this->maxCorner = rectangle.maxCorner;
}

bool Rectangle::ContainsPoint(const Vector& point) const
{
	if (point.x < this->minCorner.x || point.x > this->maxCorner.x)
		return false;

	if (point.y < this->minCorner.y || point.y > this->maxCorner.y)
		return false;

	return true;
}

void Rectangle::PointFromUVs(const Vector& UVs, Vector& point) const
{
	point.x = this->minCorner.x + UVs.x * this->Width();
	point.y = this->minCorner.y + UVs.y * this->Height();
}

void Rectangle::PointToUVs(Vector& UVs, const Vector& point) const
{
	UVs.x = (point.x - this->minCorner.x) / this->Width();
	UVs.y = (point.y - this->minCorner.y) / this->Height();
}

void Rectangle::ExpandToMatchAspectRatio(double aspectRatio)
{
	double currentAspectRatio = this->AspectRatio();

	if (currentAspectRatio < aspectRatio)
	{
		double delta = (this->Height() * aspectRatio - this->Width()) / 2.0;

		this->minCorner.x -= delta;
		this->maxCorner.x += delta;
	}
	else if(currentAspectRatio > aspectRatio)
	{
		double delta = (this->Width() / aspectRatio - this->Height()) / 2.0;

		this->minCorner.y -= delta;
		this->maxCorner.y += delta;
	}
}

void Rectangle::ContractToMatchAspectRatio(double aspectRatio)
{
	// STPTODO: Write this.
}

void Rectangle::MinimallyExpandToIncludePoint(const Vector& point)
{
	// STPTODO: Write this.
}

double Rectangle::Area() const
{
	return this->Width() * this->Height();
}

double Rectangle::Width() const
{
	return this->maxCorner.x - this->minCorner.x;
}

double Rectangle::Height() const
{
	return this->maxCorner.y - this->minCorner.y;
}

double Rectangle::AspectRatio() const
{
	return this->Width() / this->Height();
}