#include "Math/Rectangle.h"
#include <limits>

using namespace GAL2D;

Rectangle::Rectangle()
{
	this->PrepareForExpansion();
}

Rectangle::Rectangle(double minX, double maxX, double minY, double maxY)
{
	this->minCorner.x = minX;
	this->maxCorner.x = maxX;
	this->minCorner.y = minY;
	this->maxCorner.y = maxY;
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

bool Rectangle::IsValid() const
{
	if (this->minCorner.x > this->maxCorner.x)
		return false;

	if (this->minCorner.y > this->maxCorner.y)
		return false;

	return true;
}

void Rectangle::operator=(const Rectangle& rectangle)
{
	this->minCorner = rectangle.minCorner;
	this->maxCorner = rectangle.maxCorner;
}

void Rectangle::PrepareForExpansion()
{
	this->minCorner.x = std::numeric_limits<double>::max();
	this->minCorner.y = std::numeric_limits<double>::max();
	this->maxCorner.x = -std::numeric_limits<double>::max();
	this->maxCorner.y = -std::numeric_limits<double>::max();
}

bool Rectangle::ContainsPoint(const Vector& point, double eps /*= 0.0*/) const
{
	if (point.x < this->minCorner.x - eps || point.x > this->maxCorner.x + eps)
		return false;

	if (point.y < this->minCorner.y - eps || point.y > this->maxCorner.y + eps)
		return false;

	return true;
}

bool Rectangle::ContainsRectangle(const Rectangle& rectangle, double eps /*= 0.0*/) const
{
	return this->ContainsPoint(rectangle.minCorner, eps) && this->ContainsPoint(rectangle.maxCorner, eps);
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

void Rectangle::ExpandToMatchAspectRatio(double aspectRatio, AspectRatioMatchMode mode /*= DELTA_MIN_AND_MAX*/)
{
	double currentAspectRatio = this->AspectRatio();

	if (currentAspectRatio < aspectRatio)
	{
		double delta = (this->Height() * aspectRatio - this->Width()) / 2.0;

		switch (mode)
		{
			case DELTA_MIN_AND_MAX:
			{
				this->minCorner.x -= delta;
				this->maxCorner.x += delta;
				break;
			}
			case ALL_DELTA_MIN:
			{
				this->minCorner.x -= 2.0 * delta;
				break;
			}
			case ALL_DELTA_MAX:
			{
				this->maxCorner.x += 2.0 * delta;
				break;
			}
		}
	}
	else if(currentAspectRatio > aspectRatio)
	{
		double delta = (this->Width() / aspectRatio - this->Height()) / 2.0;

		switch (mode)
		{
			case DELTA_MIN_AND_MAX:
			{
				this->minCorner.y -= delta;
				this->maxCorner.y += delta;
				break;
			}
			case ALL_DELTA_MIN:
			{
				this->minCorner.y -= 2.0 * delta;
				break;
			}
			case ALL_DELTA_MAX:
			{
				this->minCorner.y += 2.0 * delta;
				break;
			}
		}
	}
}

void Rectangle::ContractToMatchAspectRatio(double aspectRatio, AspectRatioMatchMode mode /*= DELTA_MIN_AND_MAX*/)
{
	double currentAspectRatio = this->AspectRatio();

	if (currentAspectRatio < aspectRatio)
	{
		double delta = (this->Height() - this->Width() / aspectRatio) / 2.0;

		switch (mode)
		{
			case DELTA_MIN_AND_MAX:
			{
				this->minCorner.y += delta;
				this->maxCorner.y -= delta;
				break;
			}
			case ALL_DELTA_MIN:
			{
				this->minCorner.y += 2.0 * delta;
				break;
			}
			case ALL_DELTA_MAX:
			{
				this->maxCorner.y -= 2.0 * delta;
				break;
			}
		}
	}
	else if (currentAspectRatio > aspectRatio)
	{
		double delta = (this->Width() - this->Height() * aspectRatio) / 2.0;

		switch (mode)
		{
			case DELTA_MIN_AND_MAX:
			{
				this->minCorner.x += delta;
				this->maxCorner.x -= delta;
				break;
			}
			case ALL_DELTA_MIN:
			{
				this->minCorner.x += 2.0 * delta;
				break;
			}
			case ALL_DELTA_MAX:
			{
				this->maxCorner.x -= 2.0 * delta;
				break;
			}
		}
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

void Rectangle::ApplyMarginDelta(double delta)
{
	this->minCorner.x -= delta;
	this->maxCorner.x += delta;
	this->minCorner.y -= delta;
	this->maxCorner.y += delta;
}

bool Rectangle::Intersect(const Rectangle& rectangleA, const Rectangle& rectangleB)
{
	// STPTODO: Write this.

	return this->IsValid();
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