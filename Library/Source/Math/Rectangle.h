#pragma once

#include "Vector.h"

namespace GAL2D
{
	/**
	 * These are 2-dimensional rectangles in the plane.
	 */
	class Rectangle
	{
	public:
		Rectangle();
		Rectangle(const Vector& minCorner, const Vector& maxCorner);
		Rectangle(const Rectangle& rectangle);
		virtual ~Rectangle();

		void operator=(const Rectangle& rectangle);

		bool ContainsPoint(const Vector& point) const;
		void PointFromUVs(const Vector& UVs, Vector& point) const;
		void PointToUVs(Vector& UVs, const Vector& point) const;
		void ExpandToMatchAspectRatio(double aspectRatio);
		void ContractToMatchAspectRatio(double aspectRatio);
		void MinimallyExpandToIncludePoint(const Vector& point);

		double Area() const;
		double Width() const;
		double Height() const;
		double AspectRatio() const;

	public:
		Vector minCorner;
		Vector maxCorner;
	};
}