#include "Math/AffineTransform.h"

using namespace GAL2D;

AffineTransform::AffineTransform()
{
	this->MakeIdentity();
}

AffineTransform::AffineTransform(const Vector& xAxis, const Vector& yAxis, const Vector& translation)
{
	this->linearTransform.xAxis = xAxis;
	this->linearTransform.yAxis = yAxis;
	this->translation = translation;
}

AffineTransform::AffineTransform(const LinearTransform& linearTransform, const Vector& translation)
{
	this->linearTransform = linearTransform;
	this->translation = translation;
}

AffineTransform::AffineTransform(const AffineTransform& affineTransform)
{
	this->linearTransform = affineTransform.linearTransform;
	this->translation = affineTransform.translation;
}

/*virtual*/ AffineTransform::~AffineTransform()
{
}

void AffineTransform::operator=(const AffineTransform& affineTransform)
{
	this->linearTransform = affineTransform.linearTransform;
	this->translation = affineTransform.translation;
}

Vector AffineTransform::operator*(const Vector& vector) const
{
	return this->linearTransform * vector + this->translation;
}

AffineTransform AffineTransform::Inverse() const
{
	AffineTransform inverse;

	inverse.linearTransform = this->linearTransform.Inverse();
	inverse.translation = inverse.linearTransform * this->translation * -1.0;

	return inverse;
}

void AffineTransform::MakeIdentity()
{
	this->linearTransform.MakeIdentity();
	this->translation.x = 0.0;
	this->translation.y = 0.0;
}

void AffineTransform::MakeTranslation(const Vector& translation)
{
	this->linearTransform.MakeIdentity();
	this->translation = translation;
}

void AffineTransform::MakeRigidBodyTransform(double angle, Vector& translation)
{
	this->linearTransform.MakeRotation(angle);
	this->translation = translation;
}

namespace GAL2D
{
	AffineTransform operator*(const AffineTransform& affineTransformA, const AffineTransform& affineTransformB)
	{
		AffineTransform product;

		product.linearTransform = affineTransformA.linearTransform * affineTransformB.linearTransform;
		product.translation = affineTransformA * affineTransformB.translation + affineTransformA.translation;

		return product;
	}

	AffineTransform operator/(const AffineTransform& affineTransformA, const AffineTransform& affineTransformB)
	{
		return affineTransformA * affineTransformB.Inverse();
	}
}