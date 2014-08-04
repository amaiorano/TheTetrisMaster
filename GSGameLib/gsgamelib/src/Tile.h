#ifndef _TILE_H_
#define _TILE_H_

#include <string>
#include <memory>
#include "GameNode.h"
#include "ImageData.h"
#include "Color.h"
#include "Angle.h"
#include "TextureId.h"
#include "AnimSeq.h"
#include "GameClock.h"

typedef size_t TileLayerIdx;

class TileLayer
{
public:
	TileLayer();
	virtual ~TileLayer();

	// Enables/disables alpha blending and optionally sets the blend
	// factor. If fBlendFactor is < 1, the texture will be blended
	// by that factor with the destination pixels.
	// Default is alpha blending disabled
	void SetAlphaBlending(bool bEnable, float fBlendFactor=1.0f);

	// Sets the rendering color. If RGB is set to pure white, the texture is
	// rendered using original source colors, otherwise it is tinted by the set
	// color.
	// Default color is Color3F::White()
	void SetColorTint(const Color3F& color);

	// Sets amount to rotate texture by before rendering
	// Default is 0 (no rotation)
	void SetRotation(const Angle& rotAngle);

	// Sets texture for this layer
	void SetTexture(const TextureId& texId);

	// Renders the layer image
	virtual void Render(float x, float y, float w, float h);

private:
	struct PIMPL;
	std::auto_ptr<PIMPL> m_pPrivData;
};

class AnimTileLayer : public TileLayer
{
public:
	AnimTileLayer(const TextureId& firstTexId, std::auto_ptr<AnimSeq> pAnimSeq);

	virtual void Render(float x, float y, float w, float h);

private:
	TextureId m_firstTexId;
	std::shared_ptr<AnimSeq> m_pAnimSeq;	
	GameClockClient m_clock;
};


// A Map is made up of a grid of Tiles
class Tile : public GameNode
{
public:
	Tile();
	~Tile();

	// Is tile collidable? (default = false)
	bool& Collidable()				{ return m_bCollidable; }
	const bool& Collidable() const	{ return m_bCollidable; }

	// Creates and adds layer to the tile
	TileLayer& AddLayer(TileLayerIdx layer);

	// Adds input tile layer to the tile.
	// NOTE: Tile takes ownership of newly allocated TileLayer pointer
	// (will delete it in destructor)
	void AddLayer(TileLayerIdx layer, std::auto_ptr<TileLayer> pTileLayer);
	
	// Returns reference to an existing layer
	TileLayer& Layer(TileLayerIdx layer)
	{
		SMART_ASSERT(layer>=0 && layer<m_tileLayerPtrs.size());
		return *(m_tileLayerPtrs[layer]);
	}

	// Returns whether input layer exists (has been added)
	bool LayerExists(TileLayerIdx layer)
	{
		return (m_tileLayerPtrs[layer].get() != NULL);
	}

	void Render();

private:
	bool m_bCollidable;
	
	typedef std::vector<std::shared_ptr<TileLayer> > TileLayerPtrArray;
	TileLayerPtrArray m_tileLayerPtrs;
	size_t m_iLowLayerIdx, m_iHighLayerIdx;
};

#endif // _TILE_H_
