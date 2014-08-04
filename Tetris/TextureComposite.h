#ifndef _TEXTURE_COMPOSITE_H_
#define _TEXTURE_COMPOSITE_H_

#include <list>

// A composite texture class
// Workaround for now - not sure if it's worth adding to the game lib
class TextureComposite
{
public:
	void AddTexture(TextureShPtr pTexture)
	{
		m_texturePtrs.push_back(pTexture);
	}

	void TileImage(const ImageData& rImgData, int iTileWidth, int iTileHeight, float fPosX=0, float fPosY=0)
	{
		TextureShPtr pTemp;
		for (int x=0; x<rImgData.GetWidth(); x+=iTileWidth)
		{
			for (int y=0; y<rImgData.GetHeight(); y+=iTileHeight)
			{
				ImageData subImgData = rImgData.GetSubImageData(x, y, iTileWidth, iTileHeight);
				pTemp = Texture::Load( subImgData );
				pTemp->Pos().Set((float)x+fPosX, (float)y+fPosY);
				AddTexture(pTemp);
			}
		}
	}

	void SetAlphaBlending(bool bEnable, float fBlendFactor=1.0f)
	{
		TextureShPtrList::iterator it;
		for (it = m_texturePtrs.begin(); it != m_texturePtrs.end(); ++it)
			(*it)->SetAlphaBlending(bEnable, fBlendFactor);
	}

	void RenderQuad()
	{
		TextureShPtrList::iterator it;
		for (it = m_texturePtrs.begin(); it != m_texturePtrs.end(); ++it)
			(*it)->RenderQuad();
	}

private:
	typedef std::list<TextureShPtr> TextureShPtrList;
	TextureShPtrList m_texturePtrs;

};

#endif // _TEXTURE_COMPOSITE_H_
