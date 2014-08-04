// Defs.h

// Contains global standard definitions and macros

#ifndef _DEFS_H_
#define _DEFS_H_

// Game library include (links with lib)
#include <GSGameLib.h>

// Standard includes
#include <stdio.h>		// sprintf()
#include <windows.h>	// Header file for Windows
#include <time.h>		// time()

// Returns the game engine instance
class TheTetrisMaster;
TheTetrisMaster* GetGameEngine();

// Performs x^y (x to the power of y)
int Power(int x, int y);

// Redefine ASSERT to use SMART_ASSERT
#define ASSERT(AssertCondition, Reason) SMART_ASSERT(AssertCondition).msg(Reason)

// General defines and constants
#define UCHAR					unsigned char

// Use this define to control the screen mode
#define MODE_320x240

#ifdef MODE_320x240
	#define RAW_SZ(x)			(x)
	#define SCALE(x)			(x)
#else // if MODE_640x480
	#define RAW_SZ(x)			(x/2)
	#define SCALE(x)			(x*2)
#endif

#define SCALE_FACTOR			SCALE(1)

// Game-specific
#define GAME_TITLE				"The Tetris Master"
#define SCREEN_WIDTH 			SCALE(320)
#define SCREEN_HEIGHT 			SCALE(240)
#define BITS_PER_PIXEL			32

#define NUM_BLOCKS 				7
#define BLOCK_SQUARE_SIZE 		SCALE(10)
#define NO_BLOCK 				0

#define MAP_WIDTH				SCALE(100)
#define MAP_HEIGHT				SCALE(200)
#define MAP_HIDDEN_ROWS 		2
#define MAP_ROWS 				20+MAP_HIDDEN_ROWS /* Add 2 for the two hidden rows above the screen */
#define MAP_COLS 				10
#define MAP_X 					(SCREEN_WIDTH-MAP_WIDTH)/2
#define MAP_Y					(SCREEN_HEIGHT-MAP_HEIGHT)/2

#define GAMEINFO_WIN_X			SCALE(229)
#define GAMEINFO_WIN_Y 			SCALE(20)
#define GAMEINFO_WIN_WIDTH 		SCALE(70)
#define GAMEINFO_WIN_HEIGHT 	SCALE(96)

#define NEXT_WIN_X				SCALE(229)
#define NEXT_WIN_Y 				SCALE(126)
#define NEXT_WIN_WIDTH 			SCALE(70)
#define NEXT_WIN_HEIGHT 		SCALE(42)

#define LINES_LEVEL_WIN_X		SCALE(229)
#define LINES_LEVEL_WIN_Y 		SCALE(178)
#define LINES_LEVEL_WIN_WIDTH 	SCALE(70)
#define LINES_LEVEL_WIN_HEIGHT 	SCALE(41)

#define SCORE_WIN_X				SCALE(8)
#define SCORE_WIN_Y 			SCALE(150)
#define SCORE_WIN_WIDTH 		SCALE(82)
#define SCORE_WIN_HEIGHT 		SCALE(30)

#define TIME_WIN_X				SCALE(8)
#define TIME_WIN_Y 				SCALE(190)
#define TIME_WIN_WIDTH 			SCALE(82)
#define TIME_WIN_HEIGHT 		SCALE(30)

#define FONT_CHAR_WIDTH			SCALE(9)
#define FONT_CHAR_HEIGHT		SCALE(11)
#define FONT_INTER_CHAR_SPACE	SCALE(1)
#define FONT_SHADOW_OFFSET		SCALE(2)

#endif _DEFS_H_