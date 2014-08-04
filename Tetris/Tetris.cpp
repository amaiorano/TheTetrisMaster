#include "Tetris.h"

#include <iostream>
#include "TMTheme.h"
#include "TMScreen.h"

const char* VERSION = "ver 2.0";

bool TheTetrisMaster::Initialize()
{
	m_bNewGamePending = true;
	m_bGameOver = false;
	m_bBlockWasPlaced = false;

	// Load up game config
	m_gameConfig.AddVar("fullscreen", "true");
	m_gameConfig.AddVar("win_width", 640);
	m_gameConfig.AddVar("win_height", 480);
	m_gameConfig.AddVar("audio", "true");
	m_gameConfig.LoadConfigFile("TTM.cfg");
	return true;
}

void TheTetrisMaster::PreCreateWindow(std::string& strTitle, int& iWidth, int& iHeight, int& iBitsPerPixel, bool& bFullScreen)
{
	strTitle = GAME_TITLE;
	iWidth = SCREEN_WIDTH;
	iHeight = SCREEN_HEIGHT;
	iBitsPerPixel = BITS_PER_PIXEL;
	bFullScreen = m_gameConfig["fullscreen"] == "true";
}

void TheTetrisMaster::InitializeAudioStreamMgr(AudioStreamMgr& rAudioStreamMgr)
{
	if ( m_gameConfig["audio"] == "true" )
		rAudioStreamMgr.Initialize();
}

// Yes, macros are evil, but this makes the code cleaner and is only used
// in TheTetrisMaster::LoadGameData() below
#define LOAD_TRANSP_WIN(var, name) \
	if (true) { \
		ImageData subImgData = imgData.GetSubImageData(0, 0, name##_WIDTH, name##_HEIGHT); \
		var = Texture::Load(subImgData); \
		var->SetAlphaBlending(true, fTranspLevel); \
		var->Pos().Set(name##_X, name##_Y); \
	} else

bool TheTetrisMaster::LoadGameData()
{
	// Resize window in windowed mode
	IWindow& rWindow = GameApp::Instance().GetWindow();
	if ( !rWindow.IsFullScreen() )
	{
		rWindow.Resize(m_gameConfig["win_width"].AsInt(), m_gameConfig["win_height"].AsInt());
	}

	// Load theme
	TMTheme theme;
	theme.LoadTheme(m_strThemeName);

	// Grab transparency level
	float fTranspLevel = theme.GetTranspLevel();

	// Load up the background (cut up into smaller textures for older video card compatibility)
	ImageData imgData = ImageData::LoadRAW(theme.GetBackgroundImageFileName(), RAW_SZ(SCREEN_WIDTH), RAW_SZ(SCREEN_HEIGHT), 3);
	imgData.Scale(SCALE_FACTOR, SCALE_FACTOR);
	m_background.TileImage(imgData, SCREEN_WIDTH/4, SCREEN_HEIGHT/2);

	// Load up the map window
	imgData = ImageData::LoadRAW(theme.GetTranpWinImageFileName(), RAW_SZ(MAP_WIDTH), RAW_SZ(MAP_HEIGHT), 3);
	imgData.Scale(SCALE_FACTOR, SCALE_FACTOR);
	m_mapWindow.TileImage(imgData, MAP_WIDTH, MAP_HEIGHT/2, MAP_X, MAP_Y);
	m_mapWindow.SetAlphaBlending(true, fTranspLevel);

	// From the map window, load up the other windows
	LOAD_TRANSP_WIN(m_gameInfoWindow, GAMEINFO_WIN);
	LOAD_TRANSP_WIN(m_linesLevelWindow, LINES_LEVEL_WIN);
	LOAD_TRANSP_WIN(m_scoreWindow, SCORE_WIN);
	LOAD_TRANSP_WIN(m_timeWindow, TIME_WIN);
	LOAD_TRANSP_WIN(m_nextWindow, NEXT_WIN);

	// Load up the blocks
	imgData = ImageData::LoadRAW(theme.GetAllBlocksImageFileName(), 70, 10, 3);
	for (int i=0; i<NUM_BLOCKS; ++i)
	{
		m_allBlocks[i] = Texture::Load( imgData.GetSubImageData(i*10, 0, 10, 10).Scale(SCALE_FACTOR, SCALE_FACTOR) );
		m_allBlocks[i]->SetAlphaBlending(true);
	}

	// Load the game info screen
	imgData = ImageData::LoadRAW("Graphics/gameinfo.raw", RAW_SZ(GAMEINFO_WIN_WIDTH), RAW_SZ(GAMEINFO_WIN_HEIGHT));
	imgData.AddAlphaChannel(Color3UB::Black()); // Add alpha using black as the transparent color
	m_gameInfo = Texture::Load(imgData.Scale(SCALE_FACTOR, SCALE_FACTOR));
	m_gameInfo->Pos().Set(GAMEINFO_WIN_X, GAMEINFO_WIN_Y);

	// Load game sounds and music
	AudioStreamMgr& rAudioStreamMgr = GameApp::Instance().GetAudioStreamMgr();

	m_pGameMusic = rAudioStreamMgr.LoadMusicFile( theme.GetGameMusicFileName() );
	m_pPlaceBlockSound = rAudioStreamMgr.LoadSoundFile( theme.GetPlaceBlockSoundFileName(), 5 );
	m_p123LinesSound = rAudioStreamMgr.LoadSoundFile( theme.GetLinesSoundFileName(), 3 );
	m_pTetrisSound = rAudioStreamMgr.LoadSoundFile( theme.GetTetrisSoundFileName(), 3 );
	m_pChangeLevelSound = rAudioStreamMgr.LoadSoundFile( theme.GetChangeLevelSoundFileName(), 1 );
	m_pGameOverSound = rAudioStreamMgr.LoadSoundFile( theme.GetGameOverSoundFileName(), 1 );

	m_pGameMusic->SetRepeat(true); // Make sure music loops

#if 0 // Don't set volumes, the current audio library handles it very well.
	m_pGameMusic->SetVolume( theme.GetMusicVolume() );

	m_pPlaceBlockSound->SetVolume( theme.GetSoundVolume() );
	m_p123LinesSound->SetVolume( theme.GetSoundVolume() );
	m_pTetrisSound->SetVolume( theme.GetSoundVolume() );
	m_pChangeLevelSound->SetVolume( theme.GetSoundVolume() );
	m_pGameOverSound->SetVolume( theme.GetSoundVolume() );
#endif

	// Load up the font
	m_font.Initialize(TMFont::SHADOWS_ON, theme.GetFontColor(), theme.GetFontShadowColor());

	return true;
}

void TheTetrisMaster::ProcessInput(const KeyboardMgr& rKeyboardMgr)
{
	static TMScreen* pScreen = TMScreen::GetInstance();
	static TMMap* pMap = pScreen->GetMap();	


	// If new game is pending, we're waiting for user to hit space to start
	if ( m_bNewGamePending )
	{
		if ( rKeyboardMgr[VK_SPACE].Pressed() )
		{
			m_bNewGamePending = false;
			TMScreen::GetInstance()->NewGame();
		}

		return; // Don't continue until Space is pressed
	}

	static bool bPaused = pScreen->IsPaused();
	if ( rKeyboardMgr['P'].Pressed() )
	{
		bPaused = !bPaused;
		pScreen->PauseGame(bPaused);

		// Pause/Resume all musica and sound
		if ( bPaused )
			GameApp::Instance().GetAudioStreamMgr().PauseAll();
		else
			GameApp::Instance().GetAudioStreamMgr().ResumeAll();
	}

	if ( bPaused )
		return; // Don't continue in paused state

	// Get amount of MS elapsed since last iteration
	float fDeltaTimeMS = GameApp::Instance().GetDeltaTimeMS();

	// Timing variables
	const float fRotTimeMS = 250;
	const float fMoveHorzMS = 80;
	
	// Drop time must be at least as fast as maximum drop speed.
	// According to tetris spec, at max level 9, it drops at 0.05 second
	// intervals (=50 ms)
	const float fMoveDownMS = 50;
	const float fMoveDownAfterBlockPlacementsMS = fMoveDownMS * 10;

	bool bDownTimeElapsed; // Used to track if a key has been down a given amount of time

	if ( rKeyboardMgr['A'].Down(fRotTimeMS, fDeltaTimeMS, bDownTimeElapsed) )
	{
		if ( bDownTimeElapsed )
			pMap->RotateCurrBlockRight();
	}
	else if ( rKeyboardMgr['S'].Down(fRotTimeMS, fDeltaTimeMS, bDownTimeElapsed) )
	{
		if ( bDownTimeElapsed )
			pMap->RotateCurrBlockLeft();
	}

	if ( rKeyboardMgr[VK_LEFT].Down(fMoveHorzMS, fDeltaTimeMS, bDownTimeElapsed) )
	{
		if ( bDownTimeElapsed )
			pMap->MoveCurrBlockLeft();
	}
	else if ( rKeyboardMgr[VK_RIGHT].Down(fMoveHorzMS, fDeltaTimeMS, bDownTimeElapsed) )
	{
		if ( bDownTimeElapsed )
			pMap->MoveCurrBlockRight();
	}

	// For moving blocks down, we want the delay to be longer when a block is just placed so that
	// the next piece doesn't start dropping at full speed as soon as it's spawned, giving the player
	// a chance to optionally let go of the down button and line up the piece.
	float fActualMoveDownMS = m_bBlockWasPlaced? fMoveDownAfterBlockPlacementsMS : fMoveDownMS;
	if ( rKeyboardMgr[VK_DOWN].Down(fActualMoveDownMS, fDeltaTimeMS, bDownTimeElapsed) && bDownTimeElapsed )
	{
		m_bBlockWasPlaced = pMap->MoveCurrBlockDown();
	}
}

bool TheTetrisMaster::RenderFrame(IRenderer& renderer)
{
	// While new game is pending, just render the background and display message
	if ( m_bNewGamePending )
	{
		GetBackgroundTexture().RenderQuad();
		m_font.WriteCentered(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, "Press SPACE to start");
	}
	else
	{
		// Just tell the screen instance to do its work. It returns false
		// on Game Over
		m_bGameOver = TMScreen::GetInstance()->DoIteration() == false;
	}

	// Write game version
	m_font.Write(SCREEN_WIDTH-SCALE(80), SCREEN_HEIGHT-SCALE(15), VERSION);

	return true;
}

void TheTetrisMaster::PostFrameLogic()
{
	if ( !m_bNewGamePending && m_bGameOver )
	{
		// Game over just happened
		TMScreen::GetInstance()->DoGameOver();
		m_bNewGamePending = true; // Start waiting for a new game
	}
}
