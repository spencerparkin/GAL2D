#pragma once

#include "Vector.h"

namespace GAL2D
{
	/**
	 * These are 2-dimensional, axis-aligned rectangles in the plane.
	 */
	class Rectangle
	{
	public:
		Rectangle();
		Rectangle(double minX, double maxX, double minY, double maxY);
		Rectangle(const Vector& minCorner, const Vector& maxCorner);
		Rectangle(const Rectangle& rectangle);
		virtual ~Rectangle();

		void operator=(const Rectangle& rectangle);

		enum AspectRatioMatchMode
		{
			ALL_DELTA_MIN,
			ALL_DELTA_MAX,
			DELTA_MIN_AND_MAX
		};

		bool IsValid() const;
		void PrepareForExpansion();
		bool ContainsPoint(const Vector& point, double eps = 0.0) const;
		bool ContainsRectangle(const Rectangle& rectangle, double eps = 0.0) const;
		void PointFromUVs(const Vector& UVs, Vector& point) const;
		void PointToUVs(Vector& UVs, const Vector& point) const;
		void ExpandToMatchAspectRatio(double aspectRatio, AspectRatioMatchMode mode = DELTA_MIN_AND_MAX);
		void ContractToMatchAspectRatio(double aspectRatio, AspectRatioMatchMode mode = DELTA_MIN_AND_MAX);
		void MinimallyExpandToIncludePoint(const Vector& point);
		void MinimallyExpandToIncludeRect(const Rectangle& rectangle);
		void ApplyMarginDelta(double delta);
		bool Intersect(const Rectangle& rectangleA, const Rectangle& rectangleB);

		double Area() const;
		double Width() const;
		double Height() const;
		double AspectRatio() const;

	public:
		Vector minCorner;
		Vector maxCorner;
	};

	Rectangle operator+(const Rectangle& rectangle, const Vector& vector);
	Rectangle operator-(const Rectangle& rectangle, const Vector& vector);
}