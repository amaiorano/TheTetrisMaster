#ifndef _ANGLE_H_
#define _ANGLE_H_

#include <assert.h>
#include "MathDefs.h"


// Represents an angle in degrees in the range [0,360[
class Angle
{
public:
	Angle() : m_fAngle(0.0f) {}
	Angle(float fAngle) : m_fAngle(fAngle) {}

	// Conversion operator to float
	operator float() const	{ return m_fAngle; }

	// Conversion operator to double
//	operator double() const	{ return m_fAngle; }

	// Returns angle in radians instead of degrees
	float ToRads() { return (float)DEG2RAD(m_fAngle); }

	// Use to increase angle (don't pass in negative value!)
	void operator+=(float fOffset)
	{
		assert(fOffset>=0.0 && fOffset<360.0f);

		m_fAngle += fOffset;
		if ( m_fAngle > 360.0f )
			m_fAngle -= 360.0f;
	}

	// Use to decrease angle (don't pass in negative value!)
	void operator-=(float fOffset)
	{
		assert(fOffset>=0.0 && fOffset<360.0f);

		m_fAngle -= fOffset;
		if ( m_fAngle < 0.0f )
			m_fAngle += 360.0f;
	}

private:
	float m_fAngle;
};

#endif // _ANGLE_H_
