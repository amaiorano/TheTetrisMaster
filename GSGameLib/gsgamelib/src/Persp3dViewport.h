#ifndef _PERSP_3D_VIEWPORT_H_
#define _PERSP_3D_VIEWPORT_H_

#include "IViewport.h"

// Maintains a 3D view with perspective calculations.

class Persp3dViewport :	public IViewport
{
public:
	// By default, the y-axis is inverted, meaning y=0 is the top
	// row, and increases as it goes downward
	Persp3dViewport(float fFOVAngle=45.0f, float fNearClipPlane=0.1f, float fFarClipPlane=100.0f)
		: m_fFOVAngle(fFOVAngle), m_fNearClipPlane(fNearClipPlane), m_fFarClipPlane(fFarClipPlane)
	{
	}

protected:
	virtual void SetupProjectionMatrix(int iWidth, int iHeight)
	{
		float fAspectRatio = static_cast<float>(iWidth)/iHeight;
		Renderer::Instance().Perspective(m_fFOVAngle, fAspectRatio, m_fNearClipPlane, m_fFarClipPlane);
	}

	virtual void TranslateToStartPosition()
	{
	}

private:
	float m_fFOVAngle;
	float m_fNearClipPlane;
	float m_fFarClipPlane;
};

#endif // _PERSP_3D_VIEWPORT_H_
