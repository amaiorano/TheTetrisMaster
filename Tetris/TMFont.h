#ifndef _TM_FONT_H_
#define _TM_FONT_H_

#include "Defs.h"
#include <string>
#include <memory>

const int NUM_FONT_CHARS = 256;

class TMFont
{
public:
	TMFont();
	~TMFont();

	enum eShadowOptions { SHADOWS_ON, SHADOWS_OFF };
	void Initialize(eShadowOptions shadowOptions, const Color3F& fontColor, const Color3F& shadowColor=Color3F::Black());

	Point2d<float>& Pos()				{ return m_pos; }
	const Point2d<float>& Pos() const	{ return m_pos; }

	// Functions that return font character width and height respetively
	int GetCharWidth();
	int GetCharHeight();

	void Write(std::string strText);
	void Write(float x, float y, std::string strText);

	// Writes text centered within input bounding box
	void WriteCentered(float x, float y, float w, float h, std::string strText);

private:
	Point2d<float> m_pos;	// Current font position
	TextureShPtr m_fontList[NUM_FONT_CHARS];

	bool m_bShadows;
	Color3F m_fontColor;
	Color3F m_shadowColor;
};

#endif // _TM_FONT_H_