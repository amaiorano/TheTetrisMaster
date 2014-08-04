// video.h

#ifndef VIDEO_H
#define VIDEO_H

#include <windows.h>
#include <stdio.h>
#include <time.h>

#include <fstream>
using namespace std;

/*
// Structs used by LoadBMP()
#pragma pack(1) // For compatibility with Watcom C/C++

typedef struct
{
	short type;
	long size;
	short reserved;
	short reserved2;
	long offset;
} bmpHeader;

typedef struct
{
	long size;
	long width;
	long height;
	short planes;
	short bpp;
	long compression;
	long imageSize;
	long xRes;
	long yRes;
	long colorsUsed;
	long importantColors;
} bmpInfoHeader;
*/


class VideoObject
{
	public:

		VideoObject(int Width, int Height); // Constructor
		~VideoObject(void); // Destructor

		void SetWinHandle(HWND hwnd);
		
		void Paint(void); // Paints bitmap to the screen (call only in WM_PAINT)
		void ShowScreen(void); // Displays bitmap to screen (call anywhere)
				
		bool LoadRAW(unsigned short *DEST, char filename[], long size); // Loads .raw files
		//bool LoadBMP(unsigned short *DEST, char FileName[], int Width, int Height); // Loads .bmp 

		void GetVSCREENAddress(unsigned short **POINTER); // Sets POINTER to VSCREEN

		// Use these two functions if width to copy is different from width of SOURCE buffer
		void Copy(unsigned short *SOURCE, int Xs, int Ys, int Xd, int Yd, int Width, int Height, int SWidth=0);
		void CopyTransparent(unsigned short *SOURCE, int Xs, int Ys, int Xd, int Yd, int Width, int Height, int TLevel, int SWidth=0);
		void CopySprite(unsigned short *SOURCE, int Xs, int Ys, int Xd, int Yd, int Width, int Height, int SWidth=0);

		void Clear(unsigned short color);

		void FadeOut(void); // Fades out the VSCREEN
		void ScreenShot(char filename[]); // Saves a screenshot of VSCREEN to filename.raw

unsigned short *VSCREEN; // Virtual buffer to blit to screen		

	private:

		// Private member functions
		
		unsigned short SetRGB(unsigned char r, unsigned char g, unsigned char b); // Combines rgb values
		unsigned char Red(unsigned short color); // Returns the r value of color
		unsigned char Green(unsigned short color); // Returns the g value of color
		unsigned char Blue(unsigned short color); // Returns the b value of color
		
		// Private data members



		HWND hWindow; // Handle to the window

		HDC hdc; // Handle to device context (Screen)
		PAINTSTRUCT ps; // Used in WM_PAINT

		HDC hBitmapDC; // Handle to the Bitmap Device Context
		HBITMAP hBitmap; // Handle to the VSCREEN bitmap
			
		int VWidth, VHeight; // Width and height of VSCREEN

		int left, right, top, bottom; // Defines boundaries to redraw in Paint()
		RECT rect; // Rectangle structure used to redraw in Display()
};

#endif
