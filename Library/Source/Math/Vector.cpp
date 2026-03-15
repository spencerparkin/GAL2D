#include "Math/Vector.h"
#include <math.h>

using namespace GAL2D;

Vector::Vector()
{
	this->x = 0.0;
	this->y = 0.0;
}

Vector::Vector(double x, double y)
{
	this->x = x;
	this->y = y;
}

Vector::Vector(const Vector& vector)
{
	this->x = vector.x;
	this->y = vector.y;
}

/*virtual*/ Vector::~Vector()
{
}

void Vector::operator=(const Vector& vector)
{
	this->x = vector.x;
	this->y = vector.y;
}

void Vector::operator*=(double scalar)
{
	this->x *= scalar;
	this->y *= scalar;
}

void Vector::operator/=(double scalar)
{
	this->x /= scalar;
	this->y /= scalar;
}

double Vector::Length() const
{
	return sqrt(this->Dot(*this));
}

double Vector::Dot(const Vector& vector) const
{
	return this->x * vector.x + this->y * vector.y;
}

double Vector::Cross(const Vector& vector) const
{
	return this->x * vector.y - this->y * vector.x;
}

void Vector::ToPolar(double& radius, double& angle) const
{
	angle = atan2(this->y, this->x);
	radius = this->Length();
}

void Vector::FromPolar(double radius, double angle)
{
	this->x = radius * cos(angle);
	this->y = radius * sin(angle);
}

namespace GAL2D
{
	Vector operator+(const Vector& vectorA, const Vector& vectorB)
	{
		return Vector(
			vectorA.x + vectorB.x,
			vectorA.y + vectorB.y
		);
	}

	Vector operator-(const Vector& vectorA, const Vector& vectorB)
	{
		return Vector(
			vectorA.x - vectorB.x,
			vectorA.y - vectorB.y
		);
	}

	Vector operator*(const Vector& vector, double scalar)
	{
		return Vector(
			vector.x * scalar,
			vector.y * scalar
		);
	}

	Vector operator*(double scalar, const Vector& vector)
	{
		return Vector(
			vector.x * scalar,
			vector.y * scalar
		);
	}

	Vector operator/(const Vector& vector, double scalar)
	{
		return Vector(
			vector.x / scalar,
			vector.y / scalar
		);
	}
}