#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <string>
#include <memory>
#include "Renderer.h"
#include "Geom2D.h"
#include "TextureId.h"

typedef Point2d<float> TexPos;
typedef Size2d<float> TexSize;

class Texture;
typedef std::shared_ptr<Texture> TextureShPtr;
typedef std::weak_ptr<Texture> TextureWkPtr;

// Represents a single texture.
class Texture
{
public:
	~Texture();

	// Loads up the input file, creates the texture in texture memory,
	// and returns shared pointer to the instance
	static TextureShPtr Load(const std::string& strFileName);

	// Creates texture from input ImageData instance
	static TextureShPtr Load(const ImageData& rImageData);

	// Selects this texture as the current texture, subsequent
	// texturing calls will now use this texture
	void Select(bool bSelect=true)
	{
		Renderer::Instance().SelectTexture(m_texId, bSelect);
	}

	TexPos& Pos()				{ return m_pos; }
	const TexPos& Pos() const	{ return m_pos; }

	TexSize& Size()				{ return m_size; }
	const TexSize& Size() const	{ return m_size; }

	// Enables/disables alpha blending and optionally sets the blend
	// factor. If fBlendFactor is < 1, the texture will be blended
	// by that factor with the destination pixels.
	// Default is alpha blending disabled
	void SetAlphaBlending(bool bEnable, float fBlendFactor=1.0f);

	// Sets the rendering color. If RGB is set to pure white, the texture is
	// rendered using original source colors, otherwise it is tinted by the set
	// color.
	// Default color is Color3F::White()
	void SetColor(const Color3F& color);

	// Selects and does a simple render of the texture onto a quad.
	// The input position is NOT saved for efficiency. Use Pos() to
	// set it.
	// Note: To get exact representation of image, set current color
	// to white.
	void RenderQuad(float x, float y);

	// Convenient overload for when position does not change often.
	// Make sure to set it first using Pos().
	void RenderQuad() { RenderQuad(m_pos.x, m_pos.y); }

private:
	// Hide constructors
	Texture(const ImageData& rImageData);

	// Frees up the texture memory
	void Free();

private:
	TextureId m_texId;		// Unique texture id	

	TexPos m_pos;			// The texture's (relative) position
	TexSize m_size;			// The texture's current size
	
	Color4F m_color;		// The texture's rendering color and blend factor (alpha)
	bool m_bAlphaBlending;	// On/Off?
};

#endif // _TEXTURE_H_
