#ifndef _THE_TETRIS_MASTER_H_
#define _THE_TETRIS_MASTER_H_

#include <string>
#include "Defs.h"
#include "TMFont.h"
#include "TextureComposite.h"

// The Game Engine
class TheTetrisMaster : public IGameEngine
{
public:
	void SetThemeName(const std::string& strThemeName) { m_strThemeName = strThemeName; }
	
	// Note: If any of the shared pointers got destroyed, we'd get exceptions
	// trying to dereference them as we do. But we know these won't
	// be destroyed until the the game is closed, so it's ok.

	TextureComposite& GetBackgroundTexture()	{ return m_background; }
	TextureComposite& GetMapWindowTexture()		{ return m_mapWindow; }
	Texture& GetGameInfoWindowTexture()			{ return *m_gameInfoWindow; }
	Texture& GetLinesLevelWindowTexture()		{ return *m_linesLevelWindow; }
	Texture& GetScoreWindowTexture()			{ return *m_scoreWindow; }
	Texture& GetTimeWindowTexture()				{ return *m_timeWindow; }
	Texture& GetNextWindowTexture()				{ return *m_nextWindow; }
	Texture& GetBlockTexture(int index)			{ return *m_allBlocks[index]; }
	Texture& GetGameInfoTexture()				{ return *m_gameInfo; }

	AudioStream& GetGameMusicStream()			{ return *m_pGameMusic; }
	AudioStream& GetPlaceBlockSoundStream()		{ return *m_pPlaceBlockSound; }
	AudioStream& Get123LinesSoundStream()		{ return *m_p123LinesSound; }
	AudioStream& GetTetrisSoundStream()			{ return *m_pTetrisSound; }
	AudioStream& GetChangeLevelSoundStream()	{ return *m_pChangeLevelSound; }
	AudioStream& GetGameOverSoundStream()		{ return *m_pGameOverSound; }

	TMFont& GetFontObject()						{ return m_font; }

protected:
	virtual bool Initialize();
	virtual void PreCreateWindow(std::string& strTitle, int& iWidth, int& iHeight, int& iBitsPerPixel, bool& bFullScreen);	
	virtual void InitializeAudioStreamMgr(AudioStreamMgr& rAudioStreamMgr);
	virtual bool LoadGameData();
	virtual void ProcessInput(const KeyboardMgr& rKeyboardMgr);
	virtual bool RenderFrame(IRenderer& renderer);
	virtual void PostFrameLogic();

private:
	std::string m_strThemeName;
	Config m_gameConfig;

	// Use TextureComposites for background and map window to slice up each
	// texture so that they are smaller or equal to 256x256 which is the max
	// on Voodoo and perhaps other old graphics cards.
	TextureComposite m_background;
	TextureComposite m_mapWindow;
	TextureShPtr m_gameInfoWindow;
	TextureShPtr m_linesLevelWindow;
	TextureShPtr m_scoreWindow;
	TextureShPtr m_timeWindow;
	TextureShPtr m_nextWindow;
	TextureShPtr m_allBlocks[NUM_BLOCKS];
	TextureShPtr m_gameInfo;

	AudioStreamShPtr m_pGameMusic;
	AudioStreamShPtr m_pPlaceBlockSound;
	AudioStreamShPtr m_p123LinesSound;
	AudioStreamShPtr m_pTetrisSound;
	AudioStreamShPtr m_pChangeLevelSound;
	AudioStreamShPtr m_pGameOverSound;

	TMFont m_font;

	bool m_bNewGamePending;
	bool m_bGameOver;
};

#endif _THE_TETRIS_MASTER_H_