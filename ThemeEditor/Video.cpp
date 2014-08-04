// video.cpp

#include "video.h"
#include "winstandard.h"

//**********************************************************************
// Constructor
VideoObject::VideoObject(int Width, int Height)
{
			
	// Set data members
	VWidth = Width;
	VHeight = Height;

	// Allocate memory to the VSCREEN
	VSCREEN = new unsigned short [Width*Height];
	memset(VSCREEN, 0, Width*Height*2);

	// Create a bitmap into which VSCREEN will copy its contents
	hdc = GetWindowDC(NULL); // Get Device Context

	hBitmap = CreateCompatibleBitmap(hdc, Width, Height); // Create the bitmap
	hBitmapDC = CreateCompatibleDC(hdc); // Create compatible DC for bitmap
	
	SelectObject(hBitmapDC, hBitmap); // Place the bitmap into the DC

	ReleaseDC(NULL, hdc); // Release the Device Context
	DeleteObject(hBitmap); // Delete the hBitmap
}


//**********************************************************************
// Destructor
VideoObject::~VideoObject(void)
{
	DeleteDC(hBitmapDC); // Delete the DC
}

//**********************************************************************
// Mutator to set hwnd member variable of VideoObject class
void VideoObject::SetWinHandle(HWND hwnd)
{
	hWindow = hwnd;
}

//**********************************************************************
// This function will paint the data contained in VSCREEN to the screen
// Must be called only within WM_PAINT case in WindowProc
void VideoObject::Paint(void)
{
	hdc = BeginPaint(hWindow, &ps); // Get DC

	// Store clipping values
	left = ps.rcPaint.left;
	right = ps.rcPaint.right;
	top = ps.rcPaint.top;
	bottom = ps.rcPaint.bottom;

	// Blit the bitmap to the screen (with clipping)
	BitBlt(hdc, left, top, right-left, bottom-top, hBitmapDC, 
				 left, top, SRCCOPY); 

	EndPaint(hWindow, &ps); // Release DC
}

//**********************************************************************
// This function can be called anywhere in the program to display the current contents
// of VSCREEN
void VideoObject::ShowScreen(void)
{
	int i, j, k;
	unsigned short temp;
	unsigned char red, green, blue;
	
	// First copy VSCREEN to hBitmapDC
	k=0;
	for (j=0; j<240; j++)
	{
		for (i=0; i<320; i++)
		{
			temp = VSCREEN[k];
			red = (unsigned char)(temp>>8);
			green = (unsigned char)(temp>>3);
			blue = (unsigned char)(temp<<3);

			SetPixel(hBitmapDC, i, j, RGB(red, green, blue));
			k++;
		}
	}

	// Now blit the bitmap to the window
	hdc = GetDC(hWindow);

	BitBlt(hdc, 0, 0, VWidth, VHeight, hBitmapDC, 0, 0, SRCCOPY);

	ReleaseDC(hWindow, hdc);
}


//**********************************************************************
bool VideoObject::LoadRAW(unsigned short *DEST, char filename[], long size)
{
	// This function loads a 16-bit color .raw file (with LOTS of error checking!)

	FILE *fp;

	unsigned short *TEMP = new unsigned short [size];
	long x = 0;
	unsigned char red=0,blue=0,green=0;
	int temp;
	
	fp = fopen(filename, "rb");

	if (fp == NULL)
	{
		delete [] TEMP;
		ErrorMessageV("Error: Cannot open file %s", filename);
		return false;
	}


	while (x<size)
	{
		// Make sure not at end of file
		if ((temp = fgetc(fp)) == EOF)
		{
			fclose(fp);
			delete [] TEMP;
			ErrorMessage("Error: Invalid file size");
			return false;
		}
		else
			ungetc(temp, fp);
		
		red = (unsigned char)fgetc(fp);
		green = (unsigned char)fgetc(fp);
		blue = (unsigned char)fgetc(fp);

		TEMP[x] = SetRGB(red>>3, green>>2, blue>>3); // least-significant bits not important
		x++;
	}

	// Make sure at end of file now
	if ((temp = fgetc(fp)) != EOF)
	{
		fclose(fp);
		delete [] TEMP;
		ErrorMessage("Error: Invalid file size");
		return false;
	}

	// If all is well, then copy the data to DEST
	memcpy(DEST, TEMP, size<<1);
	fclose(fp);
	delete [] TEMP;
	return true;
} // End of LoadRAW();

/*
//**********************************************************************
bool VideoObject::LoadBMP(unsigned short *DEST, char FileName[], int Width, int Height)
{
	FILE *fp;
	unsigned char *tempData = new unsigned char [Width*Height*3]; // For holding the data
	unsigned short *tempData2 = new unsigned short [Width*Height]; // For holding the data
	bmpInfoHeader bmpInfo;
	bmpHeader bmpH;

	fp = fopen(FileName, "rb");

	if (fp == NULL)
	{
		ErrorMessage("Error: Cannot open file");
		return FALSE; // File doesn't exist!
	}

	fread(&bmpH, sizeof(bmpHeader), 1, fp); // Load In the Header

	if (bmpH.type != 0x4D42) // This is not a BMP
	{
		ErrorMessage("Error: Not a valid BMP file");
		fclose(fp);
		return FALSE; 
	} 

	fread(&bmpInfo, sizeof(bmpInfo), 1, fp); // Load in the BMP info

	if (bmpInfo.bpp == 8) // This is 8 bit colour
	{ 
		ErrorMessage("Error: This is an 8-bit BMP file. Must be 24-bit");
		fclose(fp); 
		return FALSE; 
	} 

	if (bmpInfo.width != Width || bmpInfo.height != Height) // Wrong size
	{
		ErrorMessage("Error: Wrong size.");
		fclose(fp);
		return FALSE;
	}

	fread(tempData, bmpInfo.imageSize, 1, fp); // Read in the image

	fclose(fp);


ofstream fout("tony.txt");
fout << "Image size = " << bmpInfo.imageSize << endl;
fout.close();

	if (bmpInfo.bpp == 24) // Convert to 16 bit color:
	{
		char r,g,b;

		for (int z=0; z<bmpInfo.imageSize; z+=3)
		{
			r = tempData[z+2];
			g = tempData[z+1];
			b = tempData[z];
			tempData2[z/3] = (unsigned short)SetRGB(r>>3, g>>2, b>>3);     
		}
	}

	// Flip the BMP so it's right side up:	
	int DestOffset = 0;
	int SourceOffset = bmpInfo.width * (bmpInfo.height-1); // Last Line for the Source offset

	for (int y=0; y<bmpInfo.height; y++)
	{
		memcpy(DEST+DestOffset, tempData2+SourceOffset, bmpInfo.width*2);
		DestOffset += bmpInfo.width;
		SourceOffset -= bmpInfo.width;
	}

	delete [] tempData;
	delete [] tempData2;
	return TRUE;
}
*/

//**********************************************************************
// Sets POINTER to VSCREEN
void VideoObject::GetVSCREENAddress(unsigned short **POINTER)
{
	// Set the value of pointer to the address contained in VSCREEN
	*POINTER = VSCREEN;
}

//**********************************************************************
// Function that will copy SOURCE to the VSCREEN
// If the SOURCE width (SWidth) is different from width to copy (Width), specify it
void VideoObject::Copy(unsigned short *SOURCE, int Xs, int Ys, int Xd, int Yd, int Width, int Height, int SWidth/* = 0 */)
{
	if (SWidth == 0)
		SWidth = Width;

	int DestOffset = Xd + (Yd*VWidth);
	int SourceOffset = Xs + (Ys*SWidth);
	
	for (int i=0; i<Height; ++i)
	{
		memcpy(VSCREEN+DestOffset, SOURCE+SourceOffset, Width<<1);
		DestOffset += VWidth;
		SourceOffset += SWidth;
	}
}

//**********************************************************************
// Function that will copy SOURCE to the VSCREEN with Transparency (TLevel)
// If the SOURCE width (SWidth) is different from width to copy (Width), specify it
void VideoObject::CopyTransparent(unsigned short *SOURCE, int Xs, int Ys, int Xd, int Yd, int Width, int Height, int TLevel, int SWidth/* = 0 */)
{
	if (SWidth == 0)
		SWidth = Width;
	
	int DestOffset = (Yd*VWidth)+Xd;
	int SourceOffset = (Ys*SWidth)+Xs;
	
	unsigned short SColor,DColor; // Source and Destination colors.
	unsigned char SR,SG,SB,DR,DG,DB,FR,FG,FB; // Color elements

	for (int i=0; i<Height; ++i)
	{
		for (int j=0; j<Width; ++j)
		{
			SColor = SOURCE[SourceOffset+j];
			SR = Red(SColor);
			SG = Green(SColor);
			SB = Blue(SColor);

			DColor = VSCREEN[DestOffset+j];
			DR = Red(DColor);
			DG = Green(DColor);
			DB = Blue(DColor);

			FR = ((SR*TLevel) + (DR*(256-TLevel))) >> 8;
			FG = ((SG*TLevel) + (DG*(256-TLevel))) >> 8;
			FB = ((SB*TLevel) + (DB*(256-TLevel))) >> 8;

			VSCREEN[DestOffset+j] = SetRGB(FR, FG, FB); //FColor;
		}
		DestOffset += VWidth; // width of the screen
		SourceOffset += SWidth; // width of the Source bitmap
	}

}

//**********************************************************************
void VideoObject::CopySprite(unsigned short *SOURCE, int Xs, int Ys, int Xd, int Yd, int Width, int Height, int SWidth/* = 0 */)
{
	if (SWidth == 0)
		SWidth = Width;

	int DestOffset = Xd + (Yd*VWidth);
	int SourceOffset = Xs + (Ys*SWidth);
	unsigned short pixel;
	
	for (int i=0; i<Height; ++i)
	{
		for (int j=0; j<SWidth; ++j)
		{
			if (pixel = SOURCE[SourceOffset+j])
				VSCREEN[DestOffset+j] = pixel;
		}
		DestOffset += VWidth;
		SourceOffset += SWidth;
	}
}

//**********************************************************************
void VideoObject::Clear(unsigned short color)
{
	memset(VSCREEN, color, VWidth*VHeight*2);
}

//**********************************************************************
void VideoObject::FadeOut(void)
{
	// 16-bit Color Mode Fade Out

	long x, y;
	unsigned short color;
	unsigned char r,g,b;
	char speed=2;
	
	for(x=0; x<16; x++)
	{
		for(y=0; y<(VWidth*VHeight) ; y++)
		{
			color = VSCREEN[y];
			r = Red(color);
			g = Green(color);
			b = Blue(color);
			if (r>1)
				r -= speed;
			else
				r = 0;

			if (g>3)
				g -= (speed<<1); // (speed*2)
			else
				g = 0;

			if (b>1)
				b -= speed;
			else
				b = 0;

			VSCREEN[y] = SetRGB(r,g,b);
		}
		ShowScreen();
	}
}

//**********************************************************************
void VideoObject::ScreenShot(char filename[])
{
	// This function saves a screenshot of VSCREEN to filename.raw (16-bit)

	FILE *fp;
	unsigned char red,blue,green;
	
	if (fp = fopen(filename, "wb"))
	{
		long x = 0;
		while (x<(320*240))
		{
			red = Red(VSCREEN[x]);
			green = Green(VSCREEN[x]);
			blue = Blue(VSCREEN[x]);
			fputc(red<<3, fp);
			fputc(green<<2, fp);
			fputc(blue<<3, fp);
			x++;
		}
		fclose(fp);
	}
}


// Private member functions

//**********************************************************************
unsigned short VideoObject::SetRGB(unsigned char r, unsigned char g, unsigned char b)
{
	// Returns the color value of rgb
	return (b+(g<<5)+(r<<11));
}

//**********************************************************************
unsigned char VideoObject::Red(unsigned short color)
{
	// Returns the r value of color
	return (color>>11)&0x1F;
}

//**********************************************************************
unsigned char VideoObject::Green(unsigned short color)
{
	// Returns the g value of color
	return (color>>5)&0x3F;
}

//**********************************************************************
unsigned char VideoObject::Blue(unsigned short color)
{
	// Returns the b value of color
	return (color)&0x1F;
}
