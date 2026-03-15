#pragma once

#include "LinearTransform.h"

namespace GAL2D
{
	/**
	 * These are vector-valued functions of a vector variable, each performing
	 * an affine transformation, which is a linear transformation followed by
	 * a translation.
	 */
	class AffineTransform
	{
	public:
		AffineTransform();
		AffineTransform(const Vector& xAxis, const Vector& yAxis, const Vector& translation);
		AffineTransform(const LinearTransform& linearTransform, const Vector& translation);
		AffineTransform(const AffineTransform& affineTransform);
		virtual ~AffineTransform();

		void operator=(const AffineTransform& affineTransform);
		Vector operator*(const Vector& vector) const;

		AffineTransform Inverse() const;

		void MakeIdentity();
		void MakeTranslation(const Vector& translation);
		void MakeRigidBodyTransform(double angle, Vector& translation);

	public:
		LinearTransform linearTransform;
		Vector translation;
	};

	AffineTransform operator*(const AffineTransform& affineTransformA, const AffineTransform& affineTransformB);
	AffineTransform operator/(const AffineTransform& affineTransformA, const AffineTransform& affineTransformB);
}