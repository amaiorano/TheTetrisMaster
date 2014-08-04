#ifndef _TEXTURE_ID_H_
#define _TEXTURE_ID_H_

#include "SmartAssert.h"

// Represents a simple texture id
class TextureId
{
public:
	typedef unsigned int tTextureId;
	enum { FREE_TEX_ID = (tTextureId)-1 }; // Use largest unsigned int value
	
	TextureId() : m_texId(FREE_TEX_ID) {}
	TextureId(tTextureId texId) : m_texId(texId) {}

	operator tTextureId() const
	{ 
		SMART_ASSERT(!IsFree()).msg("Accessing invalid texture id");
		return  m_texId;
	}

	void operator=(tTextureId texId)
	{
		m_texId = texId;
	}

	// Returns true if texture id is a free one
	bool IsFree() const { return m_texId == FREE_TEX_ID; }

	// Call when texture has been unloaded
	void Reset() { m_texId = FREE_TEX_ID; }

private:
	tTextureId m_texId;
};

#endif // _TEXTURE_ID_H_
