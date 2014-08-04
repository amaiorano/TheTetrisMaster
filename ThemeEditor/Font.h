// font.h

#ifndef FONT_H
#define FONT_H

// Include files
#include <string.h>
#include <dos.h>
#include <stdio.h>
#include "video.h"

// Defines
#define WHITE 65535
#define SCREENWIDTH 320

// External definitions
extern VideoObject Video;

class FontObject
{
	public:

		// Constructor
		FontObject(char filename[], long size, char fontwidth = 9,
							 char fontheight = 11);

		// Destructor
		~FontObject(void);

		// Write facilitators (overloaded)
		void Write(char character, int x, int y); // Writes a character
		void Write(char string[], int x, int y); // Writes a string
		void Write(char string[], int y);	// Writes and centers a string
		void Write(int number, int x, int y); // Write an integer
		void Write(long number, int x, int y); // Writes a long integer
		void Write(float number, int x, int y); // Writes a float

		// Mutators
		void SetSpacing(char s);
		void SetColor(unsigned short c);
		void SetShadow(bool s);
		void SetShadowColor(unsigned short c);
		void SetWriteSlow(bool slow);
		void SetSpeed(char s);
		void SetPrecision(int p);
		void SetTARGETBUFFER(unsigned short *target);

		// Inspectors
		char GetWidth(void);
		char GetHeight(void);
		char GetSpacing(void);
		char GetSpeed(void);

	private:

		// Private facilitators
		void GetFontSprite(char character, int x, int y);
		void LoadFontArray(char filename[], long size);

		// Data members
		unsigned short *FONTBUFFER; // pointer to the font gfx buffer
		unsigned short *FontTable[128]; // array of pointers containing
																		// data for each letter

		unsigned short *TARGETBUFFER; // where to write text to

		// Font attributes
		char width, height, spacing;
		unsigned short color; // color of the font
		bool shadow; // if TRUE, draws a shadow for each character
		unsigned short ShadowColor;
		bool WriteSlow; // if TRUE, text is written slowly
		char speed; // 0 is slowest, 10 is fastest (for WriteSlow)
		int precision; // number of decimal places for float output
};

#endif
