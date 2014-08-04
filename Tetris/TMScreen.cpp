// TMScreen.cpp: implementation of the TMScreen class.
//
//////////////////////////////////////////////////////////////////////

#include "TMScreen.h"
#include "Defs.h"
#include "Tetris.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TMScreen* TMScreen::m_pInstance = NULL;

TMScreen* TMScreen::GetInstance()
{
	if (!m_pInstance)
		m_pInstance = new TMScreen;
	return m_pInstance;
}

TMScreen::TMScreen() : m_map(MAP_X, MAP_Y), m_pause(false)
{
}

TMScreen::~TMScreen()
{

}

bool TMScreen::DoIteration()
{
	// If game is paused, just draw and return.
	if ( IsPaused() )
	{
		Draw();
		return true;
	}

	m_map.DoIteration(); // Tell the map to do its work

	Draw(); // Draw the screen

	// Return false if game over
	return ( m_map.IsGameOver() == false );
}


void TMScreen::DoGameOver()
{
	ASSERT(m_map.IsGameOver(), "Only call when game over");
	GetGameEngine()->GetGameMusicStream().Stop();
	GetGameEngine()->GetGameOverSoundStream().Play(true);
}

void TMScreen::NewGame()
{
	GetGameEngine()->GetGameMusicStream().Play();
	m_map.Reset(); // Reset the map to start all over
}

void TMScreen::Draw()
{
	TheTetrisMaster* pGameEngine = GetGameEngine();
	
	pGameEngine->GetBackgroundTexture().RenderQuad();

	if ( IsPaused() )
	{
		TMFont& rFontObj = pGameEngine->GetFontObject();
		rFontObj.WriteCentered(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, "Paused");		
		return; // We've drawn enough for the paused state
	}

	// Draw all windows
	pGameEngine->GetGameInfoWindowTexture().RenderQuad();
	pGameEngine->GetLinesLevelWindowTexture().RenderQuad();
	pGameEngine->GetScoreWindowTexture().RenderQuad();
	pGameEngine->GetTimeWindowTexture().RenderQuad();
	pGameEngine->GetNextWindowTexture().RenderQuad();

	// Draw the game information stuff on top of the game info transparent window
	pGameEngine->GetGameInfoTexture().RenderQuad();

	// Write text on top of windows
	TMFont& rFontObj = pGameEngine->GetFontObject();	
	float x, y;

	pGameEngine->GetLinesLevelWindowTexture().Pos().Get(x, y);
	rFontObj.Write(x+SCALE(1), y+SCALE(1), "Lines");
	rFontObj.Write(x+SCALE(1), y+SCALE(10), str_format("%d", GetMap()->GetTotalLines()));
	rFontObj.Write(x+SCALE(1), y+SCALE(22), "Level");	
	rFontObj.Write(x+SCALE(1), y+SCALE(31), str_format("%d", GetMap()->GetCurrLevel()));

	pGameEngine->GetNextWindowTexture().Pos().Get(x, y);
	rFontObj.Write(x+SCALE(15), y+SCALE(1), "Next");

	pGameEngine->GetScoreWindowTexture().Pos().Get(x, y);
	rFontObj.Write(x+SCALE(1), y+SCALE(1), "Score");
	rFontObj.Write(x, y+SCALE(15), str_format("%d", GetMap()->GetScore()));

	pGameEngine->GetTimeWindowTexture().Pos().Get(x, y);
	rFontObj.Write(x+SCALE(1), y+SCALE(1), "Time");

	static clock_t startTime = clock();
	clock_t elapsedSecs = (clock() - startTime)/CLOCKS_PER_SEC;
	std::string strTimeElapsed = str_format("%02i:%02i:%02i", elapsedSecs/3600, elapsedSecs/60%60, elapsedSecs%60);
	rFontObj.Write(x+SCALE(1), y+SCALE(15), strTimeElapsed);
	
	// Show next block
	TMBlock nextBlock = m_map.GetNextBlock();

	// Block 1 and 7 don't look nice normally, so place them specially...
	int nextXPos = NEXT_WIN_X;
	int nextYPos = NEXT_WIN_Y;
	switch (nextBlock.GetId())
	{
		case 1:
			//nextBlock.Draw(nextXPos+(BLOCK_SQUARE_SIZE/2), nextYPos+BLOCK_SQUARE_SIZE);
			nextBlock.Draw(nextXPos+SCALE(25), nextYPos+SCALE(16));
			break;

		case 7:
			//nextBlock.Draw(nextXPos-(BLOCK_SQUARE_SIZE/2), nextYPos-(BLOCK_SQUARE_SIZE/2));
			nextBlock.Draw(nextXPos+SCALE(15), nextYPos+SCALE(1));
			break;

		default:
			nextBlock.Draw(nextXPos+SCALE(20), nextYPos+SCALE(6));
	}

	// Draw the map itself (if not paused)
	m_map.Draw();
}

void TMScreen::PauseGame(bool pause)
{
	m_pause = pause;

	if (m_pause)
		Draw();
}

TMMap* TMScreen::GetMap()
{
	return &m_map;
}

bool TMScreen::IsPaused()
{
	return m_pause;
}
