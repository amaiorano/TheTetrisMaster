#include "Tile.h"

#include <limits>
#include "SmartAssert.h"
#include "Renderer.h"
#include "AnimSeq.h"
#include "GameClock.h"

// Helper that just renders a simple quad
void RenderQuad(const TextureId& texId, float x, float y, float w, float h)
{
	IRenderer& rRenderer = Renderer::Instance();

	// Select the texture
	rRenderer.SelectTexture(texId);

	// Now render it onto a quad
	rRenderer.BeginRender(QUADS);
		rRenderer.TexCoord(0,0); rRenderer.Vertex(x, y);
		rRenderer.TexCoord(1,0); rRenderer.Vertex(x+w, y);
		rRenderer.TexCoord(1,1); rRenderer.Vertex(x+w, y+h);
		rRenderer.TexCoord(0,1); rRenderer.Vertex(x, y+h);		
	rRenderer.EndRender();
}

///////////////////////////////////////////////////////////////////////////////
// TileLayer implementation

struct TileLayer::PIMPL
{
	Color4F color;		// The texture's rendering color and blend factor (alpha)
	bool bAlphaBlending;	// On/Off?
	Angle rotAngle;
	TextureId texId;

	PIMPL() : color(Color3F::White()), bAlphaBlending(false)
	{
	}
};

TileLayer::TileLayer() : m_pPrivData(new PIMPL)
{
}

TileLayer::~TileLayer()
{
}

void TileLayer::SetAlphaBlending(bool bEnable, float fBlendFactor/*=1.0f*/)
{
	m_pPrivData->bAlphaBlending = bEnable;
	m_pPrivData->color.a = fBlendFactor; // Set alpha to blend factor

}

void TileLayer::SetColorTint(const Color3F& color)
{
	m_pPrivData->color = color; // Does not overwrite alpha
}

void TileLayer::SetRotation(const Angle& rotAngle)
{
	m_pPrivData->rotAngle = rotAngle;
}

void TileLayer::SetTexture(const TextureId& texId)
{
	m_pPrivData->texId = texId;
}

void TileLayer::Render(float x, float y, float w, float h)
{
	IRenderer& rRenderer = Renderer::Instance();

	// Do alpha blending?
	if ( m_pPrivData->bAlphaBlending )
		rRenderer.SetAlphaBlending(true);

	// Set rendering color
	rRenderer.SetColor(m_pPrivData->color);

	// Do we rotate?
	if ( m_pPrivData->rotAngle != 0 )
	{
		float xTileCenterOffset = (w/2.0f);
		float yTileCenterOffset = (h/2.0f);

		rRenderer.PushMatrix();
		rRenderer.Translate(x+xTileCenterOffset, y+yTileCenterOffset);
		rRenderer.Rotate(m_pPrivData->rotAngle);
		
		RenderQuad(m_pPrivData->texId, -xTileCenterOffset, -yTileCenterOffset, w, h);

		rRenderer.PopMatrix();
	}
	else
	{
		// Render texture onto a quad
		RenderQuad(m_pPrivData->texId, x, y, w, h);
	}

	// Restore color
	rRenderer.SetColor(Color3F::White());

	// Restore alpha blending state
	if ( m_pPrivData->bAlphaBlending )
		rRenderer.SetAlphaBlending(false);
}


///////////////////////////////////////////////////////////////////////////////
// AnimTileLayer implementation

AnimTileLayer::AnimTileLayer(const TextureId& firstTexId, std::auto_ptr<AnimSeq> pAnimSeq)
: m_firstTexId(firstTexId), m_pAnimSeq(pAnimSeq.release())
{
}

void AnimTileLayer::Render(float x, float y, float w, float h)
{
	if ( m_clock.HasElapsedMS(200) )
	{
		m_pAnimSeq->Next();
		m_clock.Reset();
	}
	
	TileLayer::SetTexture( m_firstTexId + m_pAnimSeq->Curr() );
	TileLayer::Render(x, y, w, h);
}



///////////////////////////////////////////////////////////////////////////////
// Tile implementation

// Default number of tile layers - increase this if it's more on average
// in order to improve performance when setting texture layers
const int DEF_MAX_TILE_LAYERS = 3;
/*
// Tile::PIMPL data struct
struct Tile::PIMPL
{
	typedef int IndexType;	
	IndexType lowLayerIdx, highLayerIdx;	// Range of texture layer indices
	
	struct TileLayerData
	{
		TextureId texture;		// Base texture id
		AnimSeq* pAnimSeq;		// Animation sequence
		GameClockClient clock;	// For animation timing

		TileLayerData() : pAnimSeq(NULL) {}
		~TileLayerData() { delete pAnimSeq; }
	};
	typedef std::vector<TileLayerData> TileLayerDataArray;
	TileLayerDataArray tileLayers;	

	PIMPL() 
		: tileLayers(DEF_MAX_TILE_LAYERS),
		lowLayerIdx(std::numeric_limits<IndexType>::max()), // Standard way to get max value for type
		highLayerIdx(0)
	{
	}
};
*/
Tile::Tile()
: m_bCollidable(false), m_tileLayerPtrs(DEF_MAX_TILE_LAYERS),
  m_iLowLayerIdx(std::numeric_limits<size_t>::max()), // Standard way to get max value for type
  m_iHighLayerIdx(0)
{
}

Tile::~Tile()
{
}

TileLayer& Tile::AddLayer(TileLayerIdx layer)
{
	// If not enough space in the vector, resize it. This may become
	// inefficient if done often, so change the default vector size
	// (DEF_MAX_TILE_LAYERS)
	if ( layer >= m_tileLayerPtrs.capacity() )
		m_tileLayerPtrs.resize(layer+1);

	// Add new TileLayer
	m_tileLayerPtrs[layer].reset(new TileLayer);

	// Update current low/high layer index if we have to
	if (layer < m_iLowLayerIdx)
		m_iLowLayerIdx = layer;
	else if (layer > m_iHighLayerIdx)
		m_iHighLayerIdx = layer;

	// Make sure low index is less than high index
	if ( m_iLowLayerIdx > m_iHighLayerIdx )
		m_iHighLayerIdx = m_iLowLayerIdx;

	return Layer(layer);
}

void Tile::AddLayer(TileLayerIdx layer, std::auto_ptr<TileLayer> pTileLayer)
{
	// If not enough space in the vector, resize it. This may become
	// inefficient if done often, so change the default vector size
	// (DEF_MAX_TILE_LAYERS)
	if ( layer >= m_tileLayerPtrs.capacity() )
		m_tileLayerPtrs.resize(layer+1);

	// Add new TileLayer
	m_tileLayerPtrs[layer].reset( pTileLayer.release() );

	// Update current low/high layer index if we have to
	if (layer < m_iLowLayerIdx)
		m_iLowLayerIdx = layer;
	else if (layer > m_iHighLayerIdx)
		m_iHighLayerIdx = layer;

	// Make sure low index is less than high index
	if ( m_iLowLayerIdx > m_iHighLayerIdx )
		m_iHighLayerIdx = m_iLowLayerIdx;
}

void Tile::Render()
{
	// For efficiency, we only render from lowest to highest
	// layer index
	for (size_t i=m_iLowLayerIdx; i<=m_iHighLayerIdx; ++i)
	{
		// Some layers may not have been set in between
		TileLayer* pTileLayer = m_tileLayerPtrs[i].get();

		if ( pTileLayer != NULL )
			pTileLayer->Render(m_pos.x, m_pos.y, m_size.w, m_size.h);
	}
}

/*
void Tile::SetTexture(TileLayerIdx layer, const TextureId& texId)
{
	// If not enough space in the vector, resize it. This may become
	// inefficient if done often, so change the default vector size
	// (DEF_MAX_TILE_LAYERS)
	if ( (size_t)layer >= m_pPrivData->tileLayers.capacity() )
		m_pPrivData->tileLayers.resize(layer+1);

	// Store texture id for input layer
	m_pPrivData->tileLayers[layer].texture = texId;
	m_pPrivData->tileLayers[layer].pAnimSeq = new NoAnimSeq();

	// Update current low/high layer index if we have to
	if (layer < m_pPrivData->lowLayerIdx)
		m_pPrivData->lowLayerIdx = layer;
	else if (layer > m_pPrivData->highLayerIdx)
		m_pPrivData->highLayerIdx = layer;
}

void Tile::SetAnimatedTexture(TileLayerIdx layer,
		const TextureId& firstTexId, int numTextures, double dblAnimIntervalMS)
{
	SetTexture(layer, firstTexId);
	delete m_pPrivData->tileLayers[layer].pAnimSeq;
	m_pPrivData->tileLayers[layer].pAnimSeq = new LoopAnimSeq(numTextures);
}

void Tile::Render()
{
	IRenderer& rRenderer = Renderer::Instance();

	// Aliases for more readable code
	const int& lowLayerIdx = m_pPrivData->lowLayerIdx;
	const int& highLayerIdx = m_pPrivData->highLayerIdx;
	PIMPL::TileLayerDataArray& tileLayers = m_pPrivData->tileLayers;

	SMART_ASSERT(tileLayers[lowLayerIdx].texture.IsFree() == false).msg(
		"At least one texture must be set for this tile");

	// Draw lowest layer texture with no alpha blending

	if ( tileLayers[lowLayerIdx].clock.HasElapsedMS(200) )
	{
		tileLayers[lowLayerIdx].pAnimSeq->Next();
		tileLayers[lowLayerIdx].clock.Reset();
	}
	
	int currTexture = tileLayers[lowLayerIdx].texture + tileLayers[lowLayerIdx].pAnimSeq->Curr();
	RenderQuad(currTexture, m_pos.x, m_pos.y, m_size.w, m_size.h);
	//RenderQuad(m_pPrivData->tileLayers[lowLayerIdx].texture, m_pos.x, m_pos.y, m_size.w, m_size.h);

	// Any more layers to draw?
	if (lowLayerIdx == highLayerIdx)
		return;

	// Now draw the rest of the layers with alpha blending
	rRenderer.SetAlphaBlending(true);

	for (int i=lowLayerIdx+1; i<=highLayerIdx; ++i)
	{
		if ( tileLayers[i].texture.IsFree() == false )
		{
			if ( tileLayers[i].clock.HasElapsedMS(200) )
			{
				tileLayers[i].pAnimSeq->Next();
				tileLayers[i].clock.Reset();
			}

			currTexture = tileLayers[i].texture + tileLayers[i].pAnimSeq->Curr();
			RenderQuad(currTexture, m_pos.x, m_pos.y, m_size.w, m_size.h);

			//RenderQuad(tileLayers[i].texture, m_pos.x, m_pos.y, m_size.w, m_size.h);
		}
	}

	rRenderer.SetAlphaBlending(false);
}
*/