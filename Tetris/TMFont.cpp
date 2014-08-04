#include "TMFont.h"

using namespace std;

TMFont::TMFont() : m_bShadows(true)
{
}

TMFont::~TMFont()
{
}

void TMFont::Initialize(eShadowOptions shadowOptions, const Color3F& fontColor, const Color3F& shadowColor/*=Color3F::Black()*/)
{
	// Set data members
	m_bShadows = (shadowOptions==SHADOWS_ON);
	m_fontColor = fontColor;
	m_shadowColor = shadowColor;

	// Load up font data and extract characters
	ImageData fontImageData = ImageData::LoadRAW("graphics/font.raw", 320, 50);

	// Loop and load up all characters
	int i;
	for (i=0; i<26; ++i)
	{
		int x = i * (RAW_SZ(FONT_CHAR_WIDTH)+1) + 1;

		// Upper-case first...
		int y = 1;

		// Load up the character image data
		ImageData charImageData = fontImageData.GetSubImageData(x, y, RAW_SZ(FONT_CHAR_WIDTH), RAW_SZ(FONT_CHAR_HEIGHT));
		charImageData.AddAlphaChannel();

		// Create Texture in list and load up the character
		m_fontList['A' + i] = Texture::Load(charImageData.Scale(SCALE_FACTOR, SCALE_FACTOR));

		// And now the lower-case...
		y += RAW_SZ(FONT_CHAR_HEIGHT) + 1; // Move down to next row

		charImageData = fontImageData.GetSubImageData(x, y, RAW_SZ(FONT_CHAR_WIDTH), RAW_SZ(FONT_CHAR_HEIGHT));
		charImageData.AddAlphaChannel();
		m_fontList['a' + i] = Texture::Load(charImageData.Scale(SCALE_FACTOR, SCALE_FACTOR));

		// Numbers and special chars...
		y += RAW_SZ(FONT_CHAR_HEIGHT) + 1; // Move down to next row
		if ( i <= 9 )
		{
			charImageData = fontImageData.GetSubImageData(x, y, RAW_SZ(FONT_CHAR_WIDTH), RAW_SZ(FONT_CHAR_HEIGHT));
			charImageData.AddAlphaChannel();
			m_fontList['0' + i] = Texture::Load(charImageData.Scale(SCALE_FACTOR, SCALE_FACTOR));
		}
		else if ( i <= 21 )
		{
			charImageData = fontImageData.GetSubImageData(x, y, RAW_SZ(FONT_CHAR_WIDTH), RAW_SZ(FONT_CHAR_HEIGHT));
			charImageData.AddAlphaChannel();

			char currChar;
			switch (i)
			{
			case 10: currChar = ':'; break;
			case 11: currChar = '-'; break;
			case 12: currChar = '('; break;
			case 13: currChar = ')'; break;
			case 14: currChar = '"'; break;
			case 15: currChar = ','; break;
			case 16: currChar = '!'; break;
			case 17: currChar = '?'; break;
			case 18: currChar = '\''; break;
			case 19: currChar = '.'; break;
			case 20: currChar = '&'; break;
			case 21: currChar = ' '; break;
			}

			m_fontList[currChar] = Texture::Load(charImageData.Scale(SCALE_FACTOR, SCALE_FACTOR));
		}
	} // End for all letters
}

int TMFont::GetCharWidth()
{
	return FONT_CHAR_WIDTH;
}

int TMFont::GetCharHeight()
{
	return FONT_CHAR_HEIGHT;
}

void TMFont::Write(std::string strText)
{
	for (string::size_type i=0; i<strText.size(); ++i)
	{
		char currChar = strText[i];
		Texture* pCurrFontTexture = m_fontList[currChar].get();

		// TODO: Should we just assert here? This happens during runtime...
		if ( pCurrFontTexture == NULL )
			throw std::invalid_argument("Font character not available: " + currChar);

		// Determine top-left x,y position of current character
		float x = m_pos.x + (i * FONT_CHAR_WIDTH) + (i * FONT_INTER_CHAR_SPACE);
		float y = m_pos.y;

		if ( m_bShadows )
		{
			// Render character shadow
			pCurrFontTexture->SetColor(m_shadowColor);
			pCurrFontTexture->Pos().Set(x + FONT_SHADOW_OFFSET, y /*+ FONT_SHADOW_OFFSET*/);
			pCurrFontTexture->RenderQuad();
		}

		// Render actual character
		pCurrFontTexture->SetColor(m_fontColor);
		pCurrFontTexture->Pos().Set(x, y);
		pCurrFontTexture->RenderQuad();
	}
}

void TMFont::Write(float x, float y, std::string strText)
{
	m_pos.Set(x, y);
	Write(strText);
}

void TMFont::WriteCentered(float x, float y, float w, float h, std::string strText)
{
	float xCenter = (w/2.0f) - GetCharWidth() * (strText.size()/2.0f);
	float yCenter = (h/2.0f) - (GetCharHeight()/2.0f);
	Write(xCenter, yCenter, strText);
}
