#include "Texture.h"

TextureShPtr Texture::Load(const std::string& strFileName)
{
	return Load( ImageData::Load(strFileName) );
}

TextureShPtr Texture::Load(const ImageData& rImageData)
{
	// Create and returned shared pointer to instance
	TextureShPtr pTexture(new Texture(rImageData));	
	return pTexture;
}

Texture::Texture(const ImageData& rImageData) : m_color(Color3F::White()), m_bAlphaBlending(false)
{
	Size2d<float> texSize;
	m_texId = Renderer::Instance().LoadTexture(rImageData, texSize);

	// If an alpha channel exists, assume alpha blending should be enabled
	if ( rImageData.GetImageInfo().iChannels == 4 )
		m_bAlphaBlending = true;
	
	m_size = texSize;
}

Texture::~Texture()
{
	Free();
}

void Texture::Free()
{
	Renderer::Instance().FreeTexture(m_texId);
}


void Texture::SetAlphaBlending(bool bEnable, float fBlendFactor/*=1.0f*/)
{
	m_bAlphaBlending = bEnable;
	m_color.a = fBlendFactor; // Set alpha to blend factor
}


void Texture::SetColor(const Color3F& color)
{
	m_color = color; // Does not overwrite alpha
}

void Texture::RenderQuad(float x, float y)
{
	// NOTE: SEE GLRenderer::DrawSprite() TO SEE CODE FOR NON-POWER-OF-TWO
	// TEXTURES.
	IRenderer& rRenderer = Renderer::Instance();

	// Save current modelview matrix
	rRenderer.PushMatrix();

	// Move to position
	rRenderer.Translate(x, y);

	// Calculate corner coordinates
	float x1 = 0;
	float y1 = 0;
	float x2 = m_size.w;// - 1;
	float y2 = m_size.h;// - 1;

	// Do alpha blending?
	if ( m_bAlphaBlending )
		rRenderer.SetAlphaBlending(true);

	// Set rendering color
	rRenderer.SetColor(m_color);

	// Select the texture
	Select();

	// Now render it onto a quad
	rRenderer.BeginRender(QUADS);
		rRenderer.TexCoord(0,0); rRenderer.Vertex(x1, y1);
		rRenderer.TexCoord(1,0); rRenderer.Vertex(x2, y1);
		rRenderer.TexCoord(1,1); rRenderer.Vertex(x2, y2);
		rRenderer.TexCoord(0,1); rRenderer.Vertex(x1, y2);		
	rRenderer.EndRender();

	// Restore alpha blending state
	if ( m_bAlphaBlending )
		rRenderer.SetAlphaBlending(false);

	// Restore modelview matrix
	rRenderer.PopMatrix();
}
