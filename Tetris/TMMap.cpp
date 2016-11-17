// TMMap.cpp: implementation of the TMMap class.
//
//////////////////////////////////////////////////////////////////////

#include "TMMap.h"
#include "TMScreen.h"
#include "Tetris.h"

float CalculateDropDelayMS(float actualLevel)
{
	// From spec: Iteration_Delay = ((10.0 - Actual_Level) / 20.0) seconds
	// So Iteration_Delay = 50.0(10.0 - Actual_Level) milliseconds
	const float fudgeFactor = 1.5;
	return 50.0f * (10.0f - actualLevel) * fudgeFactor;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TMMap::TMMap(int x, int y)
	: m_x(x), m_y(y) // The rest of the data members are set in Reset()
{
	ASSERT(x>=0 && x<SCREEN_WIDTH && y>=0 && y<SCREEN_HEIGHT, "Bad map position on screen");
	Reset();
}

TMMap::~TMMap()
{

}

void TMMap::Reset()
{	
	memset(m_grid, NO_BLOCK, sizeof(m_grid));

	m_gameOver = false;

	m_currBlock = m_blockFactory.GetNextRandomBlock();
	m_nextBlock = m_blockFactory.GetNextRandomBlock();

	m_totalLines = 0;
	m_score = 0;
	m_currLevel = 0;
	m_freeFalls = 0;

	m_iterDropDelayMS = CalculateDropDelayMS(0); // Level 0

	m_dropDelayTimer.StartTimer(); // Starts/Resets the timer
}

bool TMMap::DoIteration()
{	
	// If elapsed time has passed current drop delay, move
	// the block down
	if ( m_dropDelayTimer.GetElapsedTimeMS() >= m_iterDropDelayMS )
	{		
		++m_freeFalls; // Block just fell freely

		MoveCurrBlockDown(); // Move the block down

		// The reset has been moved into MoveCurrBlockDown() - look in the function
		// for an explanation
#if 0
		m_dropDelayTimer.StartTimer(); // Reset timer
#endif
	}

	return true;
}

void TMMap::Draw()
{
	// Draw the map transparent window
	GetGameEngine()->GetMapWindowTexture().RenderQuad();

	// Draw the map using the grid
	UCHAR id;
	for (char row=0; row<MAP_ROWS; ++row)
	{
		for (char col=0; col<MAP_COLS; ++col)
		{
			// If there's a block value in the grid, draw it

			id = m_grid[row][col];
			if (id != NO_BLOCK)
			{
				int x, y;
				GetCoordsFromMapPosition(row, col, x, y);
				m_blockFactory.GetBlockWithId(id)->DrawSquare(x, y);
			}
		}
	}	

	// Draw the current block
	m_currBlock.Draw();	
}

bool TMMap::BlockCanFit(char shape[4][4], UCHAR size, char mapRow, char mapCol)
{
	for (char row=0; row<size; ++row)
	{
		for (char col=0; col<size; ++col)
		{			
			if (shape[row][col])
			{
				// If the block is beyond the boundaries of the map, it obviously
				// does not fit
				if (mapRow+row >= MAP_ROWS)
					return false;

				if (mapCol+col<0 || mapCol+col >= MAP_COLS)
					return false;

				// If there's a block value in the grid, then the block can't fit
				if (m_grid[mapRow+row][mapCol+col] != NO_BLOCK)
					return false;
			}
		}
	}

	return true;
}

void TMMap::PlaceBlock(char shape[4][4], UCHAR size, char mapRow, char mapCol, UCHAR id)
{
	for (char row=0; row<size; ++row)
	{
		for (char col=0; col<size; ++col)
		{			
			if (shape[row][col])
			{
				ASSERT(m_grid[mapRow+row][mapCol+col] == NO_BLOCK, "Grid cell not empty!");

				m_grid[mapRow+row][mapCol+col] = id;
			}				
		}
	}
}

bool TMMap::MoveCurrBlockDown()
{
	// Whenever we move the block down, we reset the timing variables
	// so that:
	// 1) If user presses down, then the block won't drop down automatically
	// 2) When dropping automatically, this resets it for the next automatic drop
	m_dropDelayTimer.StartTimer(); // Reset the timer

	// Move the current block; if it got placed, then get a new random block
	if ( m_currBlock.MoveDown() ) // Returns true if block was placed
	{
		m_currBlock = m_nextBlock;
		m_nextBlock = m_blockFactory.GetNextRandomBlock();
		m_freeFalls = 0; // Reset free fall counter for new block

		// Does this new block overlap any blocks already on the map?
		if ( !m_currBlock.BlockCanFitOnMap() )
		{
			// Game over!!
			m_gameOver = true;
			return true;
		}

// Cannot wait this way because it freezes everything for the elapsed time,
// including the time display, which is not acceptable. Would have to add
// some form of iterative waiting.
#if 0
		// Wait one free fall delay before spawning next piece
		while ( m_dropDelayTimer.GetElapsedTimeMS() <= m_iterDropDelayMS ) {}
		m_dropDelayTimer.StartTimer();
#endif
		return true;
	}

	return false;
}

bool TMMap::CheckForLines(char startRow, UCHAR size)
{
	UCHAR numRows = size; // Number of rows we're going to check
	m_numLinesToRemove = 0; // Reset number of lines

	// Adjust numRows so that row+numRows doesn't go below the map
	if (startRow+numRows >= MAP_ROWS)
		numRows = MAP_ROWS - startRow;

	// Now check the relevant rows
	for (char row=startRow; row<startRow+numRows; ++row)
	{
		// Check if this row is filled with blocks. As soon as
		// we see that it's not, we jump to the next row.
		bool lineInThisRow = true;
		for (int col=0; lineInThisRow && col<MAP_COLS; ++col)
		{
			if (m_grid[row][col] == NO_BLOCK)
				lineInThisRow = false;
		}

		// If this row contains a line of blocks, update m_linesToRemove[]
		if (lineInThisRow)
		{
			m_linesToRemove[m_numLinesToRemove] = row;
			++m_numLinesToRemove;
		}
	}

	return (m_numLinesToRemove > 0);
}

void TMMap::RemoveLines()
{
	RemoveLinesEffect1(); // Perform a nice line removal effect

	// Actually remove the lines from the grid
	for (char i=0; i<m_numLinesToRemove; ++i)
	{
		for (char row=m_linesToRemove[i]; row>0; --row)
			for (char col=0; col<10; ++col)
				m_grid[row][col] = m_grid[row-1][col];
	}

	// Determine if we should go up a level
	if ( IsNextLevel() )
	{
		SetNextLevel();
		GetGameEngine()->GetChangeLevelSoundStream().Play();
	}
	else
	{
		if (m_numLinesToRemove == 4)
			GetGameEngine()->GetTetrisSoundStream().Play();
		else
			GetGameEngine()->Get123LinesSoundStream().Play();
	}
}

void TMMap::UpdateScore()
{
	// Use a variant of standard tetris scoring because it doesn't take
	// lines removed into account (if any). Standard scoring is:
	// Point_Award = ((24 + (3*Actual_Level)) - Free_Fall_Iterations);
	//
	// Our version uses Current_Level rather than Actual_Level, and if lines
	// were removed, we multiply the score by an approximation 
	// of 2.5^(Lines_To_Remove+1) to get an exponential bonus.

	int score = ((24 + (3 * m_currLevel)) - m_freeFalls); // Nearly standard

	if ( m_numLinesToRemove > 0 )
	{
		switch (m_numLinesToRemove)
		{
		case 1: score *= 5; break;
		case 2: score *= 15; break;
		case 3: score *= 40; break;
		case 4: score *= 100; break;
		}
	}

	IncrementScore(score);
}

void TMMap::RemoveLinesEffect1()
{
	// No effect coded yet...
}

bool TMMap::IsNextLevel()
{
	// Incremement the number of lines user has made so far
	m_totalLines += m_numLinesToRemove;

	// If we reached the "next level", make it go faster
	if (m_totalLines/10 != m_currLevel)
		return true;

	return false;
}

void TMMap::SetNextLevel()
{
	++m_currLevel; // Increment the current level

	// Now based on the "actual" level, determine the drop delay

	// Calculate actual level - locks at 9
	float actualLevel = m_currLevel < 9? (float)m_currLevel : 9.0f;

	// Update drop delay
	m_iterDropDelayMS = CalculateDropDelayMS(actualLevel);
}

