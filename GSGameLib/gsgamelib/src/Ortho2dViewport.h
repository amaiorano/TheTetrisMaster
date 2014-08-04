#ifndef _ORTHO_2D_VIEWPORT_H_
#define _ORTHO_2D_VIEWPORT_H_

#include "IViewport.h"

#include <windows.h>
#include <gl/glu.h>

// Maintains a perfect 2D view with no perspective calculations.
// Likely the fastest mode for 2D.

class Ortho2dViewport :	public IViewport
{
public:
	// By default, the y-axis is inverted, meaning y=0 is the top
	// row, and increases as it goes downward
	Ortho2dViewport(bool bYpositiveDownwards=true)
		: m_bYpositiveDownwards(bYpositiveDownwards)
	{
	}

protected:
	virtual void SetupProjectionMatrix(int iWidth, int iHeight)
	{
		float fTop = (float)GetInitViewPortHeight();
		float fBottom = 0;

		if ( m_bYpositiveDownwards )
			std::swap(fTop, fBottom);

		Renderer::Instance().Orthographic2D(
			0, (float)GetInitViewPortWidth(),
			fBottom, fTop);
	}

	virtual void TranslateToStartPosition()
	{
		// Do nothing for ortho, current position is already
		// bottom-left (or top-left) corner
	}

private:
	float m_bYpositiveDownwards;
};

#endif // _ORTHO_2D_VIEWPORT_H_
