// block.cpp

#include "block.h"

// Constructor
//**********************************************************************
BlockObject::BlockObject(void)
{
	size = 0;
	memset(BlockArray, 0, sizeof(BlockArray));
	PixelSize = 10;
	TargetWidth = 320;
	BLOCKPIC = new unsigned short [PixelSize*PixelSize];
}

// Destructor
//**********************************************************************
BlockObject::~BlockObject(void)
{
	delete [] BLOCKPIC;
}

// Facilitators

//**********************************************************************
void BlockObject::Draw(int X, int Y)
{
	// Displays the block to the screen
	unsigned short DestOffset = (TargetWidth*Y) + X;


	for (char i=0; i<size; ++i)
		for (char j=0; j<size; ++j)
			if (BlockArray[i][j])
				Video.Copy(BLOCKPIC, 0, 0, X+(j*10), Y+(i*10), 10, 10);
}

//**********************************************************************
bool BlockObject::LoadBlock(char Filename[], int X, int Y)
{
	unsigned short *TEMPBUFFER = new unsigned short [70*PixelSize]; // (unsigned short *)malloc(70*PixelSize<<1);

	if (Video.LoadRAW(TEMPBUFFER, Filename, 70*PixelSize))
	{
		int DestOffset = 0;
		int SourceOffset = Y*70 + X;
		for (char i=0; i<PixelSize; ++i)
		{
			memcpy(BLOCKPIC+DestOffset, TEMPBUFFER + SourceOffset, 20);
			DestOffset += PixelSize;
			SourceOffset += 70;
		}
		delete [] TEMPBUFFER; //free(TEMPBUFFER);
		return true;
	}
	else
	{
		delete [] TEMPBUFFER; //free(TEMPBUFFER);
		return false;
	}
	return true;
}

// Mutators

//**********************************************************************
void BlockObject::SetSize(char s)
{
	size = s;
}

//**********************************************************************
void BlockObject::SetShape(char a[5], char b[5], char c[5], char d[5], char e[5])
{
	for (char i=0; i<size; ++i)
	{
		BlockArray[0][i] = (int)(a[i]-'0');
		BlockArray[1][i] = (int)(b[i]-'0');
		BlockArray[2][i] = (int)(c[i]-'0');
		BlockArray[3][i] = (int)(d[i]-'0');
		BlockArray[4][i] = (int)(e[i]-'0');
	}
}
