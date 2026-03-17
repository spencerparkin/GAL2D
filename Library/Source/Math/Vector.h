#pragma once

namespace GAL2D
{
	/**
	 * These are 2-dimensional vectors in the plane.
	 */
	class Vector
	{
	public:
		Vector();
		Vector(double x, double y);
		Vector(const Vector& vector);
		virtual ~Vector();

		void operator=(const Vector& vector);
		void operator+=(const Vector& vector);
		void operator-=(const Vector& vector);
		void operator*=(double scalar);
		void operator/=(double scalar);
		Vector operator-() const;

		double Length() const;
		double Dot(const Vector& vector) const;
		double Cross(const Vector& vector) const;

		void ToPolar(double& radius, double& angle) const;
		void FromPolar(double radius, double angle);

	public:
		double x, y;
	};

	Vector operator+(const Vector& vectorA, const Vector& vectorB);
	Vector operator-(const Vector& vectorA, const Vector& vectorB);
	Vector operator*(const Vector& vector, double scalar);
	Vector operator*(double scalar, const Vector& vector);
	Vector operator/(const Vector& vector, double scalar);
}