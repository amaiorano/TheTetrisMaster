// font.cpp

#include "font.h"

// Public member functions
// Constructor
//**********************************************************************
FontObject::FontObject(char filename[], long size, char fontwidth /* =9 */,
											 char fontheight /* =11 */)
{
	// This constructor must have the filename and size of the font RAW file
	// passed to it. If the height and width of each font character is not
	// the default values, they must be passed as well.

	Video.GetVSCREENAddress(&TARGETBUFFER);
	width = fontwidth;
	height = fontheight;
	spacing = 1;
	color = WHITE;
	WriteSlow = FALSE;
	speed = 5;
	shadow = FALSE;
	ShadowColor = 6944;
	precision = 2;

	LoadFontArray(filename, size);
}

// Destructor
//**********************************************************************
FontObject::~FontObject(void)
{
	for (int i=0; i<128; ++i)
		delete [] FontTable[i];
}

// Facilitators
//**********************************************************************
void FontObject::Write(char character, int x, int y)
{
	// This method draws character at (x,y) on TARGETBUFFER.
	// It does not draw any black (transparent)

	int DestOffset = y*SCREENWIDTH + x;
	int SourceOffset = 0;
	unsigned short pixel;

	for (char i=0; i<height; ++i)
	{
		for (char j=0; j<width; ++j)
		{
			pixel = *(FontTable[character]+SourceOffset+j);
			if (pixel) // if the pixel is not black, draw it...
			{
				TARGETBUFFER[DestOffset+j] = color;
				if (shadow)
				{
					TARGETBUFFER[DestOffset+j+1] = ShadowColor;
					TARGETBUFFER[DestOffset+j+2] = ShadowColor;
				}
			}
		}
		DestOffset += SCREENWIDTH;
		SourceOffset += width;
	}
}

//**********************************************************************
void FontObject::Write(char string[], int x, int y)
{
	// This method writes string[] to TARGETBUFFER. If WriteSlow is TRUE,
	// the string is written out slowly, otherwise it is blasted to
	// TARGETBUFFER

	int StrLength = (int)strlen(string);

	for (int i=0; i<StrLength; ++i)
	{
		Write(string[i], x, y);
		x += (width+spacing);

		if (WriteSlow)
		{
			Video.ShowScreen();
			Sleep((10-speed)*10);	//delay((10-speed)*10);
		}
	}
}

//**********************************************************************
void FontObject::Write(char string[], int y)
{
	// This method is similar to WriteString, except it centers the
	// string at position y on TARGETBUFFER

	int StrLength = (int)strlen(string);
	int x = (SCREENWIDTH - (StrLength*(width+spacing))) >>1;

	for (int i=0; i<StrLength; ++i)
	{
		Write(string[i], x, y);
		x += (width+spacing);

		if (WriteSlow)
		{
			Video.ShowScreen();
			Sleep((10-speed)*10);	//delay((10-speed)*10);
		}
	}
}

//**********************************************************************
void FontObject::Write(int number, int x, int y)
{
	char tempstring[20]; // to hold number converted to string
	sprintf(tempstring, "%d", number); // convert number to string
	Write(tempstring, x, y);
}

//**********************************************************************
void FontObject::Write(float number, int x, int y)
{
	char tempstring[20]; // to hold number converted to string
	sprintf(tempstring, "%.*f", precision, number); // convert float to string
	Write(tempstring, x, y);
}

//**********************************************************************
void FontObject::Write(long number, int x, int y)
{
	char tempstring[20]; // to hold number converted to string
	sprintf(tempstring, "%d", number); // convert number to string
	Write(tempstring, x, y);
}

// Public mutators
//**********************************************************************
void FontObject::SetSpacing(char s)
{
	spacing = s;
}

//**********************************************************************
void FontObject::SetColor(unsigned short c)
{
	color = c;
}

//**********************************************************************
void FontObject::SetShadow(bool s)
{
	shadow = s;
}
//**********************************************************************
void FontObject::SetShadowColor(unsigned short c)
{
	ShadowColor = c;
}

//**********************************************************************
void FontObject::SetWriteSlow(bool slow)
{
	WriteSlow = slow;
}

//**********************************************************************
void FontObject::SetSpeed(char s)
{
	speed = s;
}

//**********************************************************************
void FontObject::SetPrecision(int p)
{
	precision = p;
}

//**********************************************************************
void FontObject::SetTARGETBUFFER(unsigned short *target)
{
	TARGETBUFFER = target;
}

// Public inspectors
//**********************************************************************
char FontObject::GetWidth(void)
{
	return width;
}

//**********************************************************************
char FontObject::GetHeight(void)
{
	return height;
}

//**********************************************************************
char FontObject::GetSpacing(void)
{
	return spacing;
}

//**********************************************************************
char FontObject::GetSpeed(void)
{
	return speed;
}

// Private member functions
//**********************************************************************
void FontObject::GetFontSprite(char character, int x, int y)
{
	// This function copies the character at (x,y) from FONTBUFFER
	// into FontTable[character]
	int DestOffset = 0;
	int SourceOffset = y*SCREENWIDTH + x;

	FontTable[character] = new unsigned short [height*width];	//(unsigned short *)malloc(height*width*2);

	for (char i=0; i<height; i++)
	{
		memcpy(FontTable[character]+DestOffset, FONTBUFFER+SourceOffset, width<<1);
		SourceOffset += SCREENWIDTH;
		DestOffset += width; // pointer moves "width" ints (width*2 bytes)
	}
}

//**********************************************************************
void FontObject::LoadFontArray(char filename[], long size)
{
	// This method loads all the fonts into FontArray[]
	short x, y;
	char Character;

	FONTBUFFER = new unsigned short [size]; //(unsigned short *)malloc(size*2);

	if (Video.LoadRAW(FONTBUFFER, filename, size) == false)
	{
		MessageBox(NULL, "Cannot load font table.\nSoftware is not properly installed.", "Error", MB_OK | MB_ICONHAND);
		delete [] FONTBUFFER;
		return;
	}

	// Load upper case letters
	x = y = 1;
	for (Character = 'A'; Character <= 'Z'; ++Character)
	{
		GetFontSprite(Character, x, y);
		x += (width+1);
	}

	// Load lower case letters
	x = 1;
	y += (height+1);
	for (Character = 'a'; Character <= 'z'; ++Character)
	{
		GetFontSprite(Character, x, y);
		x += (width+1);
	}

	// Load numbers 0 to 9
	x = 1;
	y += (height+1);
	for (Character = '0'; Character <= '9'; ++Character)
	{
		GetFontSprite(Character, x, y);
		x += (width+1);
	}

	x = 101;
	y = 25;
	// Load extra characters
	GetFontSprite(':', x, y);
	GetFontSprite('-', x += (width+1), y);
	GetFontSprite('(', x += (width+1), y);
	GetFontSprite(')', x += (width+1), y);
	GetFontSprite('"', x += (width+1), y);
	GetFontSprite(',', x += (width+1), y);
	GetFontSprite('!', x += (width+1), y);
	GetFontSprite('?', x += (width+1), y);
	GetFontSprite('\'', x += (width+1), y);
	GetFontSprite('.', x += (width+1), y);
	GetFontSprite('&', x += (width+1), y);
	GetFontSprite(' ', x += (width+1), y);

	delete [] FONTBUFFER; //free(FONTBUFFER); // We're done, so destroy FONTBUFFER
}
