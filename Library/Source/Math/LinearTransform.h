#pragma once

#include "Vector.h"

namespace GAL2D
{
	/**
	 * These are vector-valued functions of a vector variable, each performing
	 * a linear transformation, such as a rotation, scale or shear.
	 */
	class LinearTransform
	{
	public:
		LinearTransform();
		LinearTransform(const Vector& xAxis, const Vector& yAxis);
		LinearTransform(const LinearTransform& linearTransform);
		virtual ~LinearTransform();

		void operator=(const LinearTransform& linearTransform);
		Vector operator*(const Vector& vector) const;

		LinearTransform Inverse() const;
		double Determinant() const;

		void MakeIdentity();
		void MakeRotation(double angle);
		void MakeScale(double xScale, double yScale);

	public:
		Vector xAxis;
		Vector yAxis;
	};

	LinearTransform operator*(const LinearTransform& linearTransformA, const LinearTransform& linearTransformB);
	LinearTransform operator/(const LinearTransform& linearTransformA, const LinearTransform& linearTransformB);
}