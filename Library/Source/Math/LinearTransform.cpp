#include "Math/LinearTransform.h"
#include <math.h>

using namespace GAL2D;

LinearTransform::LinearTransform()
{
	this->MakeIdentity();
}

LinearTransform::LinearTransform(const Vector& xAxis, const Vector& yAxis)
{
	this->xAxis = xAxis;
	this->yAxis = yAxis;
}

LinearTransform::LinearTransform(const LinearTransform& linearTransform)
{
	this->xAxis = linearTransform.xAxis;
	this->yAxis = linearTransform.yAxis;
}

/*virtual*/ LinearTransform::~LinearTransform()
{
}

void LinearTransform::operator=(const LinearTransform& linearTransform)
{
	this->xAxis = linearTransform.xAxis;
	this->yAxis = linearTransform.yAxis;
}

Vector LinearTransform::operator*(const Vector& vector) const
{
	return this->xAxis * vector.x + this->yAxis * vector.y;
}

LinearTransform LinearTransform::Inverse() const
{
	double det = this->Determinant();

	LinearTransform inverseTransform;

	inverseTransform.xAxis.x = this->yAxis.y / det;
	inverseTransform.xAxis.y = -this->xAxis.y / det;
	inverseTransform.yAxis.x = -this->yAxis.x / det;
	inverseTransform.yAxis.y = this->xAxis.x / det;

	return inverseTransform;
}

double LinearTransform::Determinant() const
{
	return this->xAxis.Cross(this->yAxis);
}

void LinearTransform::MakeIdentity()
{
	this->xAxis.x = 1.0;
	this->xAxis.y = 0.0;
	this->yAxis.x = 0.0;
	this->yAxis.y = 1.0;
}

void LinearTransform::MakeRotation(double angle)
{
	this->xAxis.FromPolar(1.0, angle);
	this->yAxis.FromPolar(1.0, angle + M_PI / 2.0);
}

void LinearTransform::MakeScale(double xScale, double yScale)
{
	this->xAxis.x = xScale;
	this->xAxis.y = 0.0;
	this->yAxis.x = 0.0;
	this->yAxis.y = yScale;
}

namespace GAL2D
{
	LinearTransform operator*(const LinearTransform& linearTransformA, const LinearTransform& linearTransformB)
	{
		LinearTransform product;

		product.xAxis = linearTransformA * linearTransformB.xAxis;
		product.yAxis = linearTransformA * linearTransformB.yAxis;

		return product;
	}

	LinearTransform operator/(const LinearTransform& linearTransformA, const LinearTransform& linearTransformB)
	{
		return linearTransformA * linearTransformB.Inverse();
	}
}