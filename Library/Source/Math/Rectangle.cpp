#include "Math/Rectangle.h"
#include <limits>

using namespace GAL2D;

Rectangle::Rectangle()
{
	// This rectangle is ready for expansion.
	this->minCorner.x = std::numeric_limits<double>::max();
	this->minCorner.y = std::numeric_limits<double>::max();
	this->maxCorner.x = -std::numeric_limits<double>::max();
	this->maxCorner.y = -std::numeric_limits<double>::max();
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
	double currentAspectRatio = this->AspectRatio();

	if (currentAspectRatio < aspectRatio)
	{
		double delta = (this->Height() - this->Width() / aspectRatio) / 2.0;

		this->minCorner.y += delta;
		this->maxCorner.y -= delta;
	}
	else if (currentAspectRatio > aspectRatio)
	{
		double delta = (this->Height() * aspectRatio - this->Width()) / 2.0;

		this->minCorner.x += delta;
		this->maxCorner.x -= delta;
	}
}

void Rectangle::MinimallyExpandToIncludePoint(const Vector& point)
{
	if (this->minCorner.x > point.x)
		this->minCorner.x = point.x;
	if (this->maxCorner.x < point.x)
		this->maxCorner.x = point.x;
	if (this->minCorner.y > point.y)
		this->minCorner.y = point.y;
	if (this->maxCorner.y < point.y)
		this->maxCorner.y = point.y;
}

void Rectangle::MinimallyExpandToIncludeRect(const Rectangle& rectangle)
{
	this->MinimallyExpandToIncludePoint(rectangle.minCorner);
	this->MinimallyExpandToIncludePoint(rectangle.maxCorner);
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

namespace GAL2D
{
	Rectangle operator+(const Rectangle& rectangle, const Vector& vector)
	{
		return Rectangle(
			rectangle.minCorner + vector,
			rectangle.maxCorner + vector
		);
	}

	Rectangle operator-(const Rectangle& rectangle, const Vector& vector)
	{
		return Rectangle(
			rectangle.minCorner - vector,
			rectangle.maxCorner - vector
		);
	}
}