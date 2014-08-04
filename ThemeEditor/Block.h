// block.h

#ifndef BLOCK_H
#define BLOCK_H

#include "video.h"

extern VideoObject Video;

class BlockObject
{
	public:
		
		// Constructor
		BlockObject(void);

		// Destructor
		~BlockObject(void);

		// Facilitators
		void Draw(int X, int Y);

		bool LoadBlock(char Filename[], int X, int Y);

		// Mutators
		void SetSize(char s);
		void SetShape(char a[5] = "00000", char b[5] = "00000",
									char c[5] = "00000", char d[5] = "00000",
									char e[5] = "00000");
		private:

		// Data members
		char size; // 3 = 3x3, 5 = 5x5
		short BlockArray[5][5]; // hold the shape of the block
		unsigned short *BLOCKPIC;
		char PixelSize; // width or height in pixels (width=height)
		int TargetWidth; // width of TARGETBUFFER
};

#endif
