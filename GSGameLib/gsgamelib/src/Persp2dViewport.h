#ifndef _PERSP_2D_VIEWPORT_H_
#define _PERSP_2D_VIEWPORT_H_

#include "IViewport.h"

// Maintains a pseudo-2D view with perspective calculations.
// This is like having an orthographic view, but with the ability
// to use foreshortening when rotating planes.

class Persp2dViewport :	public IViewport
{
public:
	// By default, the y-axis is inverted, meaning y=0 is the top
	// row, and increases as it goes downward
	Persp2dViewport(bool bYpositiveDownwards=true)
		: m_fYmult(bYpositiveDownwards? 1.0f : -1.0f)
	{
	}

protected:
	virtual void SetupProjectionMatrix(int iWidth, int iHeight)
	{
		Renderer::Instance().Perspective(
			-(GetInitViewPortWidth()/20.0f), GetInitViewPortWidth()/20.0f, 
			m_fYmult*(GetInitViewPortHeight()/20.0f), -m_fYmult*(GetInitViewPortHeight()/20.0f),
			0.1f, 10000.0f );
	}

	virtual void TranslateToStartPosition()
	{
		// Move to lower-left (or upper-left if y positive downwards)
		// corner to make it (0,0)
		const float z = -1.0f; // Gives perfect ratio 
		Renderer::Instance().Translate( 
			-GetInitViewPortWidth()/2.0f,
			-GetInitViewPortHeight()/2.0f,
			z);
	}

private:
	float m_fYmult;
};

#endif // _PERSP_2D_VIEWPORT_H_
