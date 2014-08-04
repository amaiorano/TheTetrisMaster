#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <math.h>
#include <assert.h>
#include "MathDefs.h"
#include "Angle.h"

// Some vector equations:
//
// A dot B = |a||b|cos(t)   -> t is angle between a and b
// A  X  B = |a||b|sin(t)n  -> n is unit vector perpendicular to a-b plane
//
// Two vectors are perpendicular if they are both non-zero (mag > 0) and
// their dot product is 0 (because cos(90) == 0).



// NOTE: This file defines 2 vector classes: Vector2d, Vector2dMA (see below)
class Vector2d;
class Vector2dMA;

// Represents a 2D vector by its components.
class Vector2d
{
public:
	Vector2d() : m_x(0.0f), m_y(0.0f) {}
	Vector2d(float x, float y) : m_x(x), m_y(y) {}

	void SetValues(float x, float y) { m_x=x; m_y=y; }

	const float& X() const { return m_x; }
	const float& Y() const { return m_y; }

	float& X() { return m_x; }
	float& Y() { return m_y; }

	// Calculates and returns magnitude of vector
	float Mag() const { return sqrtf(MagSquared());  }

	// Calculates and returns angle the vector makes with x-axis in x-y plane (phi)
	float Phi() const { return RAD2DEG(atanf(m_y/m_x)); }

	// Conversion to Vector2dMA (implementation in CPP)
	operator Vector2dMA();

	// Explicit conversion
	Vector2dMA ToVector2dMA();

	///////////////////////////////////////////////////////////////////////////
	// Standard vector operations
	///////////////////////////////////////////////////////////////////////////

	// Functions that modify the vector (non-const)

	// Makes vector a zero vector
	void MakeZero() { m_x = m_y = 0.0f; }

	// Compares two vectors
	bool operator==(const Vector2d& rhs) const { return FLOAT_EQUALS(m_x,rhs.m_x) && FLOAT_EQUALS(m_y,rhs.m_y); }

	// Addition and subtraction
	void operator+=(const Vector2d& rhs) { m_x += rhs.m_x; m_y += rhs.m_y; }
	void operator-=(const Vector2d& rhs) { m_x -= rhs.m_x; m_y -= rhs.m_y; }

	// Scalar multiplication
	void operator*=(float fScalar) { m_x *= fScalar; m_y *= fScalar; }

    // Cross product
	void CrossWith(const Vector2d& rhs) { assert(false); /* NOT IMPLEMENTED */ }

	// Negates the vector (same as v *= -1)
	void Negate() { m_x=-m_x; m_y=-m_y; }

	// Normalizes the vector (make vector have magnitude = 1 without changing direction)
	void Normalize() { float fMag=Mag(); m_x/=fMag; m_y/=fMag; }

	// Rotates vector input angle amount around input axis
	void Rotate(float angle, const Vector2d& axis) { assert(false); /* NOT IMPLEMENTED */ }


	// Functions that do not modify the vector (const)

	// Addition and subtraction
	Vector2d operator+(const Vector2d& rhs) const { return Vector2d(m_x+rhs.m_x, m_y+rhs.m_y); }
	Vector2d operator-(const Vector2d& rhs) const { return Vector2d(m_x-rhs.m_x, m_y-rhs.m_y); }

	Vector2d operator*(float fScalar) { return Vector2d(m_x*fScalar, m_y*fScalar); }
	
	// Cross product
	Vector2d Cross(const Vector2d& rhs) { assert(false); /* NOT IMPLEMENTED */ }

	// Returns negated vector
	Vector2d operator-() const { return Vector2d(-m_x, -m_y); }

	// Dot product (sum of component products)
	float Dot(const Vector2d& rhs) const { return (m_x*rhs.m_x + m_y*rhs.m_y); }

	// Returns magnitude squared (to avoid square root operation)
	float MagSquared() const { return SQUARED(m_x) + SQUARED(m_y); }

	// Returns angle between both vectors (note: relatively expensive operation)
	Angle AngleBetween(const Vector2d& rhs) const
	{
		float fDot = Dot(rhs);
		float fRads = acosf( fDot / (Mag()*rhs.Mag()) );
		return RAD2DEG(fRads);
	}

	///////////////////////////////////////////////////////////////////////////
	// State query functions
	///////////////////////////////////////////////////////////////////////////

	// Returns true if vector is a unit vector (magnitude == 1)
	bool IsUnit() const { float fMag=Mag(); return FLOAT_EQUALS(fMag, 1.0f); }

	// Returns true if both vectors are perpendicular to each other
	bool IsPerpendicularTo(const Vector2d& rhs) const { return Dot(rhs)==0; }

private:
	float m_x, m_y;
};






// Represents a 2D vector by its magnitude and angle that it makes with x-axis
// in x-y plane (in degrees). This class is mostly just a container for this
// alternate representation - to perform calculations, convert to Vector2d.
class Vector2dMA
{
public:
	Vector2dMA() : m_fMag(0) {}
	Vector2dMA(float fMag, const Angle& phi) : m_fMag(fMag), m_phi(phi) {}

	void SetValues(float fMag, const Angle& phi) { m_fMag=fMag; m_phi=phi; }

	const float& Mag() const	{ return m_fMag; }
	const Angle& Phi() const	{ return m_phi; }

	float& Mag()				{ return m_fMag; }
	Angle& Phi()				{ return m_phi; }

	// These functions calculate and return the components of the vector
	float X() const				{ return m_fMag * cosf( DEG2RAD(m_phi) ); }
	float Y() const				{ return m_fMag * sinf( DEG2RAD(m_phi) ); }

	// Implicit conversion to Vector2d
	operator Vector2d() const { return Vector2d(X(), Y()); }

	// Explicit conversion
	Vector2d ToVector2d() const { return Vector2d(X(), Y()); }

private:
	float m_fMag;
	Angle m_phi;
};



#endif // _VECTOR_H_
