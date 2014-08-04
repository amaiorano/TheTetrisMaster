// TMBlock.cpp: implementation of the TMBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "Defs.h"
#include "TMBlock.h"
#include "TMScreen.h"

#include "Tetris.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TMMap* TMBlock::m_pMap = TMScreen::GetInstance()->GetMap();

TMBlock::TMBlock() : m_specialRotate(false), m_specialRotLeft(true)
{
	SetMapPosition(0, 0);
	SetSize(2);
	SetShape();
}

TMBlock::~TMBlock()
{

}

void TMBlock::SetShape(	char r1[4] /*= "0000"*/,
						char r2[4] /*= "0000"*/,
						char r3[4] /*= "0000"*/,
						char r4[4] /*= "0000"*/ )
{
	for (UCHAR i=0; i<m_size; ++i)
	{
		m_shape[0][i] = (UCHAR)(r1[i]-'0');
		m_shape[1][i] = (UCHAR)(r2[i]-'0');
		m_shape[2][i] = (UCHAR)(r3[i]-'0');
		m_shape[3][i] = (UCHAR)(r4[i]-'0');
	}
}

void TMBlock::Draw(int x/*=ON_MAP*/, int y/*=ON_MAP*/)
{
	int startX, startY;

	if (x==ON_MAP || y==ON_MAP)
	{
		// Get the upper-left (x,y) position of this block on the screen from the Map
		m_pMap->GetCoordsFromMapPosition(m_mapRow, m_mapCol, startX, startY);
	}
	else
	{
		startX = x;
		startY = y;
	}

	// Now draw the block there
	int yPos = startY;
	for (char row=0; row<m_size; ++row, yPos+=BLOCK_SQUARE_SIZE)
	{
		// Don't draw blocks that are currently on a hidden row
		if ( m_mapRow+row < MAP_HIDDEN_ROWS )
			continue;

		int xPos = startX;
		for (char col=0; col<m_size; ++col)
		{
			if (m_shape[row][col]) // Do we draw this square?
				DrawSquare(xPos, yPos);

			xPos+=BLOCK_SQUARE_SIZE;
		}
	}

}

void TMBlock::DrawSquare(int x, int y)
{	
	Texture& rBlockTex = GetGameEngine()->GetBlockTexture(GetId()-1);

	rBlockTex.Pos().Set((float)x, (float)y);
	rBlockTex.RenderQuad();
}

bool TMBlock::MoveDown()
{
	// Ask the map if this block can move down
	if ( m_pMap->BlockCanFit(m_shape, m_size, m_mapRow+1, m_mapCol) )
	{
		++m_mapRow; // Increment the block's column position
	}
	else // It doesn't fit, so place it there
	{
		m_pMap->PlaceBlock(m_shape, m_size, m_mapRow, m_mapCol, m_id);

		Draw();

		// If placing the block created lines, remove them
		if ( m_pMap->CheckForLines(m_mapRow, m_size) )
		{
			// NOTE: This function plays sounds for 123 or 4 (tetris) lines
			m_pMap->RemoveLines();
		}
		else
		{
			// No line, but block was placed, so play the sound
			GetGameEngine()->GetPlaceBlockSoundStream().Play();
		}

		// Placed a block, so update the score
		m_pMap->UpdateScore();

		return true;
	}

	return false; // Not placed by default
}

void TMBlock::MoveLeft()
{
	// Ask the map if this block can move left
	if ( m_pMap->BlockCanFit(m_shape, m_size, m_mapRow, m_mapCol-1) )
		--m_mapCol;
}

void TMBlock::MoveRight()
{
	// Ask the map if this block can move right
	if ( m_pMap->BlockCanFit(m_shape, m_size, m_mapRow, m_mapCol+1) )
		++m_mapCol;
}

void TMBlock::RotateLeft()
{
	if (m_specialRotate)
		RotateSpecial();
	else
		DoRotateLeft();
}

void TMBlock::RotateRight()
{
	if (m_specialRotate)
		RotateSpecial();
	else
		DoRotateRight();
}

bool TMBlock::BlockCanFitOnMap()
{
	return m_pMap->BlockCanFit(m_shape, m_size, m_mapRow, m_mapCol);
}

void TMBlock::RotateSpecial()
{
	ASSERT(m_specialRotate, "Not special block!");
	
	if (m_specialRotLeft)
		DoRotateLeft();
	else
		DoRotateRight();

	m_specialRotLeft = !m_specialRotLeft; // Toggle
}

void TMBlock::DoRotateLeft()
{
	char col1, col2, row1, row2;

	memset(m_rotShape, 0, sizeof(m_rotShape));

	row2 = col2 = 0;
	for (col1=(m_size-1); col1>=0; --col1)
	{
		for (row1=0; row1<m_size; ++row1)
		{
			m_rotShape[row2][col2] = m_shape[row1][col1];
			++col2;
		}
		col2=0;
		++row2;
	}

	if ( m_pMap->BlockCanFit(m_rotShape, m_size, m_mapRow, m_mapCol) )
		memcpy(m_shape, m_rotShape, sizeof(m_shape));
}

void TMBlock::DoRotateRight()
{
	int col1, col2, row1, row2;

	memset(m_rotShape, 0, sizeof(m_rotShape));

	row2 = col2 = 0;
	for (col1=0; col1<m_size; ++col1)
	{
		for (row1=(m_size-1); row1>=0; --row1)
		{
			m_rotShape[row2][col2] = m_shape[row1][col1];
			++col2;
		}
		col2=0;
		++row2;
	}

	if ( m_pMap->BlockCanFit(m_rotShape, m_size, m_mapRow, m_mapCol) )
		memcpy(m_shape, m_rotShape, sizeof(m_shape));
}
