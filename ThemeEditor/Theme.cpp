// theme.cpp

#include "theme.h"
#include "globals.h"


// HACK: Don't bother storing short file names anymore. This makes sure we can actually
// load themes on volumes that don't store the 8.3 format. TODO: remove all this cruft.
#undef GetShortPathName
template <typename SizeT>
void GetShortPathName(char in[], char out[], SizeT len)
{
	strncpy(out, in, len-1);
}

ColorObject Color[16];

// Global buffers and variables
unsigned short *BACKGROUND;
unsigned short *MAINWIN;
unsigned short *GAMEINFO;

// Global vars to hold Program Path and Theme Path
char ProgramPath[512];
char ThemePath[512];

// Gloval vars for long filenames
char long_filename[80];
char long_BackgroundPic[80];
char long_TWindow[80];
char long_BlockPic[80];
char long_GameMusic[80];
char long_PlaceBlockSound[80];
char long_LinesSound[80];
char long_TetrisSound[80];
char long_ChangeLevelSound[80];
char long_GameOverSound[80];

// Declare block objects to display in the main window
BlockObject Block[7];



//**********************************************************************
// Constructor
ThemeObject::ThemeObject(void)
{
	// Allocate memory to global buffers (for Theme Editor)
	BACKGROUND = new unsigned short [320*240];
	MAINWIN = new unsigned short [100*200];
	GAMEINFO = new unsigned short [70*96];

	// Get ProgramPath[]
	_getcwd(ProgramPath, 255);

	// Set ThemePath[] to null
	strcpy(ThemePath, "\0");

	// Initialize blocks
	InitBlocks();

	// Initialize color struct
	InitColorStruct();

	// Reset all theme values
	Reset();

	// Load the Game Info .raw file
	SetCurrentDirectory(ProgramPath);
	Video.LoadRAW(GAMEINFO, ".\\graphics\\gameinfo.raw", 70*96);
}

//**********************************************************************
// Destructor
ThemeObject::~ThemeObject(void)
{
	delete [] BACKGROUND;
	delete [] MAINWIN;
	delete [] GAMEINFO;
}


//**********************************************************************
bool ThemeObject::Save(void)
{
	char string[255];

	if (strlen(long_filename))
	{
		int i = MessageBox(NULL, "Are you sure you want to save?", "Save Theme", MB_YESNO);
		if (i == IDYES)
		{
			// Get the right path to save to
			strcpy(string, ThemePath);

			// Go into the directory so conversion is possible
			if (!SetCurrentDirectory(string))
				ErrorMessage("Can't set current directory. Make sure theme name and directory match.");

			// Append to string the theme name
			strcat(string, long_filename);

			// Convert all filenames to 8.3 DOS format

			// In order to convert the filename, it must first exist. To make sure, create
			// an empty file with long_filename
			ofstream fout(long_filename);
			fout << " ";
			fout.close();

			GetShortPathName(long_filename, filename, 80);

			if (strlen(long_BackgroundPic))
				GetShortPathName(long_BackgroundPic, BackgroundPic, 80);

			if (strlen(long_TWindow))
				GetShortPathName(long_TWindow, TWindow, 80);

			if (strlen(long_BlockPic))
				GetShortPathName(long_BlockPic, BlockPic, 80);

			if (strlen(long_GameMusic))
				GetShortPathName(long_GameMusic, GameMusic, 80);

			if (strlen(long_PlaceBlockSound))
				GetShortPathName(long_PlaceBlockSound, PlaceBlockSound, 80);

			if (strlen(long_LinesSound))
				GetShortPathName(long_LinesSound, LinesSound, 80);

			if (strlen(long_TetrisSound))
				GetShortPathName(long_TetrisSound, TetrisSound, 80);

			if (strlen(long_ChangeLevelSound))
				GetShortPathName(long_ChangeLevelSound, ChangeLevelSound, 80);

			if (strlen(long_GameOverSound))
				GetShortPathName(long_GameOverSound, GameOverSound, 80);

			fout.open(string, ios::binary);
			fout.write((char *)this, sizeof(*this));
			fout.close();
			return true;
		}
		else
			return false;
	}
	else
	{
		MessageBox(NULL, "You must create a new theme before saving.", "Error", MB_OK | MB_ICONHAND);
		return false;
	}
}

//**********************************************************************
void ThemeObject::Load(char FileName[])
{
	WIN32_FIND_DATA FileData;

	ifstream fin(FileName, ios::binary);

	if (fin.fail())
	{
		fin.close();
		MessageBox(NULL, "Error: Cannot open file!", "Error Message", MB_OK);	
		return;
	}
	else
	{
		// Reset all theme values
		Reset();

		fin.read((char *)this, sizeof(*this));
		fin.close();

		// HACK: overwrite whatever we loaded from the file with the actual name of the file itself. This is to support
		// copying themes and renaming them. Not sure why I even saved the theme name in the file in the first place!
		strcpy(filename, FileName);

		// Convert all filenames to long filename equivalents and save in global vars (long_*)
		if (strlen(filename))
		{
			FindFirstFile(filename, &FileData);
			strcpy(long_filename, FileData.cFileName);
		}

		if (strlen(BackgroundPic))
		{
			FindFirstFile(BackgroundPic, &FileData);
			strcpy(long_BackgroundPic, FileData.cFileName);
		}

		if (strlen(TWindow))
		{
			FindFirstFile(TWindow, &FileData);
			strcpy(long_TWindow, FileData.cFileName);
		}

		if (strlen(BlockPic))
		{
			FindFirstFile(BlockPic, &FileData);
			strcpy(long_BlockPic, FileData.cFileName);
		}

		if (strlen(GameMusic))
		{
			FindFirstFile(GameMusic, &FileData);
			strcpy(long_GameMusic, FileData.cFileName);
		}

		if (strlen(PlaceBlockSound))
		{
			FindFirstFile(PlaceBlockSound, &FileData);
			strcpy(long_PlaceBlockSound, FileData.cFileName);
		}

		if (strlen(LinesSound))
		{
			FindFirstFile(LinesSound, &FileData);
			strcpy(long_LinesSound, FileData.cFileName);
		}

		if (strlen(TetrisSound))
		{
			FindFirstFile(TetrisSound, &FileData);
			strcpy(long_TetrisSound, FileData.cFileName);
		}

		if (strlen(ChangeLevelSound))
		{
			FindFirstFile(ChangeLevelSound, &FileData);
			strcpy(long_ChangeLevelSound, FileData.cFileName);
		}

		if (strlen(GameOverSound))
		{
			FindFirstFile(GameOverSound, &FileData);
			strcpy(long_GameOverSound, FileData.cFileName);
		}

		// Make global string ThemePath[]
		strcpy(ThemePath, ProgramPath);
		strcat(ThemePath, "\\themes\\");
		strcat(ThemePath, long_filename);
		ThemePath[strlen(ThemePath)-4] = '\0';
		strcat(ThemePath, "\\");
	
		// Now load data into neccesary buffers (if they exist)
		if (strlen(long_BackgroundPic))
			Video.LoadRAW(BACKGROUND, long_BackgroundPic, 320*240); // Load Background			
	
		if (strlen(long_TWindow))
			Video.LoadRAW(MAINWIN, long_TWindow, 100*200); // Load Map graphic
		
		if (strlen(long_BlockPic))
			LoadBlocks(long_BlockPic);

		// Set up Font object
		Font.SetColor(FontColor);
		Font.SetShadowColor(FontShadowColor);
		Font.SetShadow(UseFontShadow);

		// Clear the video buffer
		Video.Clear(0);

	}
}

//**********************************************************************
void ThemeObject::Create(char FileName[])
{
	// Create the directory in the .\themes directory

	// Make copy of current ThemePath in case of failure
	char temp[512];
	strcpy(temp, ThemePath);

	// Make global string ThemePath[]
	strcpy(ThemePath, ProgramPath);
	strcat(ThemePath, "\\themes\\");
	strcat(ThemePath, FileName);
	strcat(ThemePath, "\\");

	// Check if theme already exists
	char temp2[255];
	strcpy(temp2, ThemePath);
	strcat(temp2, FileName);
	strcat(temp2, ".the");
	ifstream fin(temp2, ios::in);
	if (!fin.fail()) // If it does NOT fail, file exists
	{		
		sprintf(temp2, "Theme \"%s.the\" already exists.\nDo you want to overwrite current theme?", FileName);
		int i = MessageBox(NULL, temp2, "New Theme", MB_YESNO | MB_ICONHAND);
		if (i == IDNO)
		{
			strcpy(ThemePath, temp); // Restore original path
			fin.close();
			return;
		}
	}
	fin.close();

	// Create the directory
	_mkdir(ThemePath);
		
	Reset(); // Reset all the data members to empty strings

	// Copy the new theme name to long_filename
	strcpy(long_filename, FileName);
	strcat(long_filename, ".the");

	Update(); // Update and redraw the theme

	sprintf(temp, "Theme \"%s\" will be saved in directory \"%s\".\n\nMake sure to copy all your theme files into this directory before loading them. If you don't, your theme will not work.", long_filename, ThemePath);
	MessageBox(NULL, temp, "New Theme", MB_OK);
}

//**********************************************************************
void ThemeObject::Update(void)
{
	// Updates and redraws current theme info to the screen/window
	// And updates and redraws the text output
	
	// Only draw the images that have been loaded by the user

	if (strlen(long_BackgroundPic))
		Video.Copy(BACKGROUND, 0, 0, 0, 0, 320, 240); // Copy the background

	if (strlen(long_TWindow))
	{
		Video.CopyTransparent(MAINWIN, 0, 0, 110, 20, 100, 200, TLevel); // Copy the Main Window
		Video.CopyTransparent(MAINWIN, 0, 0, 229, 178, 70, 41, TLevel, 100); // Copy the Line Window
		Video.CopyTransparent(MAINWIN, 0, 0, 229, 126, 70, 42, TLevel, 100); // Copy the Next Window
		Video.CopyTransparent(MAINWIN, 0, 0, 8, 150, 82, 30, TLevel, 100); // Copy the Score Window
		Video.CopyTransparent(MAINWIN, 0, 0, 8, 190, 82, 30, TLevel, 100); // Copy the Time Window
		Video.CopyTransparent(MAINWIN, 0, 0, 229, 20, 70, 96, TLevel, 100); // Copy Game Info Window


		Video.CopySprite(GAMEINFO, 0, 0, 229, 20, 70, 96);
	}

	if (strlen(long_BlockPic))
	{
		Block[0].Draw(120, 120);
		Block[1].Draw(160, 90);
		Block[2].Draw(150, 140);
		Block[3].Draw(120, 60);
		Block[4].Draw(170, 190);
		Block[5].Draw(120, 170);
		Block[6].Draw(150, 20);

		Block[3].Draw(249, 132);
	}

	// Set up Font object
	Font.SetColor(FontColor);
	Font.SetShadowColor(FontShadowColor);
	Font.SetShadow(UseFontShadow);
	
	if (strlen(long_TWindow)) // Only display the font if transparent window exists
	{	
		// Lines Window
		Font.Write("Lines", 230, 179);
		Font.Write(21, 230, 188);
		Font.Write("Level", 230, 200);
		Font.Write(2, 230, 209);

		// Next Window
		Font.Write("Next", 244, 127);

		// Score Window
		Font.Write("Score", 9, 151);
		Font.Write(36500, 8, 165);

		// Time Window
		Font.Write("Time", 9, 191);
		Font.Write("00:01:34", 9, 205);
	}

	Video.ShowScreen();
	TextOutput();
}



// Member functions to change each theme setting
//**********************************************************************
void ThemeObject::Reset()
{
	// Reset all theme values
	strcpy(filename, "");	strcpy(long_filename, "");

	strcpy(BackgroundPic, ""); strcpy(long_BackgroundPic, "");
	strcpy(TWindow, ""); strcpy(long_TWindow, "");
	strcpy(BlockPic, ""); strcpy(long_BlockPic, "");

	strcpy(GameMusic, ""); strcpy(long_GameMusic, "");

	strcpy(PlaceBlockSound, ""); strcpy(long_PlaceBlockSound, "");
	strcpy(LinesSound, ""); strcpy(long_LinesSound, "");
	strcpy(TetrisSound, ""); strcpy(long_TetrisSound, "");
	strcpy(ChangeLevelSound, "" ); strcpy(long_ChangeLevelSound, "");
	strcpy(GameOverSound, ""); strcpy(long_GameOverSound, "");
	
	strcpy(Description, "");

	TLevel = 128;
	FontColor = YELLOW;
	FontShadowColor = BLACK;
	UseFontShadow = TRUE;

	MusicVolume = 10;
	SoundVolume = 64;

	// Set all buffers to black
	memset(BACKGROUND, 0, 320*240*2);
	memset(MAINWIN, 0, 100*200*2);

	Video.Clear(0);
}


//**********************************************************************
void ThemeObject::TextOutput(void)
{
	char string[255];

	// Blast white to the text output portion
	HDC hdc = GetDC(hwnd); // Get DC
	BitBlt(hdc, 325, 0, 315, 240, NULL, 325, 0, WHITENESS); 
	BitBlt(hdc, 5, 240, 635, 70, NULL, 325, 0, WHITENESS);
	ReleaseDC(hwnd, hdc);

	// Output text to window
	sprintf(string, "Theme: %s", long_filename);
	Text.Write(string, 325, 0);

	sprintf(string, "Background: %s", long_BackgroundPic);
	Text.Write(string, 325, 20);

	sprintf(string, "Blocks: %s", long_BlockPic);
	Text.Write(string, 325, 40);

	sprintf(string, "Game Window: %s", long_TWindow);
	Text.Write(string, 325, 60);

	sprintf(string, "Music: %s", long_GameMusic);
	Text.Write(string, 325, 100);

	Text.Write("Sounds...", 325, 120);

	sprintf(string, "Place Block: %s", long_PlaceBlockSound);
	Text.Write(string, 335, 140);

	sprintf(string, "1,2,3 Lines: %s", long_LinesSound);
	Text.Write(string, 335, 160);

	sprintf(string, "Tetris (4 Lines): %s", long_TetrisSound);
	Text.Write(string, 335, 180);

	sprintf(string, "Change Level: %s", long_ChangeLevelSound);
	Text.Write(string, 335, 200);

	sprintf(string, "Game Over: %s", long_GameOverSound);
	Text.Write(string, 335, 220);

	sprintf(string, "Window Transparency Level: %i", TLevel);
	Text.Write(string, 325, 250);

	sprintf(string, "Font Color: %i", FontColor);
	Text.Write(string, 5, 250);

	sprintf(string, "Font Shadow Color: %i", FontShadowColor);
	Text.Write(string, 5, 270);

	if (UseFontShadow)
		Text.Write("Use Font Shadow: YES", 5, 290);
	else
		Text.Write("Use Font Shadow: NO", 5, 290);

	sprintf(string, "Music Volume: %i", MusicVolume);
	Text.Write(string, 325, 270);

	sprintf(string, "Sound Volume: %i", SoundVolume);
	Text.Write(string, 325, 290);
}

//**********************************************************************
// Tests if string is a valid theme name
bool ThemeObject::ValidateThemeName(char string[])
{
	int i, length;

	length = (int)strlen(string); // Get length of string

	// Did the user enter anything?
	if (!length)
	{
		ErrorMessage("No theme name was entered.");
		return false;
	}

	// Is it too long?
	if (length > 80)
	{
		ErrorMessage("Theme name is too long (Maximum 80 characters).");
		return false;
	}

	// Any illegal characters (spaces included)?
	if (isdigit(string[0]))
	{
		ErrorMessage("Theme name must begin with a letter.");
		return false;
	}

	for (i=1; i<length; ++i)
	{		
		if ((isalnum(string[i]) || string[i] == ' ' || string[i] == '-' || string[i] == '_')  == false)
		{
			MessageBox(NULL, "Invalid characters in Theme name.\nValid characters include: Letters, numbers, spaces, underscores, and hyphens.", "Error", MB_OK | MB_ICONHAND);
			return false;
		}
	}

	return true;

}


// Inspectors
//**********************************************************************
unsigned short ThemeObject::GetFontColor(void)
{
	return FontColor;
}

//**********************************************************************
unsigned short ThemeObject::GetFontShadowColor(void)
{
	return FontShadowColor;
}


//**********************************************************************
void ThemeObject::LoadBackground(char FileName[])
{
	if (strlen(FileName) > 80)
		ErrorMessage("File name too long (Maximum 80 characters).");
	else
	{
		if (Video.LoadRAW(BACKGROUND, FileName, 320*240))
			strcpy(long_BackgroundPic, FileName);
	}
}

//**********************************************************************
void ThemeObject::LoadGameWindow(char FileName[])
{
	if (strlen(FileName) > 80)
		ErrorMessage("File name too long (Maximum 80 characters).");
	else
	{
		if (Video.LoadRAW(MAINWIN, FileName, 100*200))
			strcpy(long_TWindow, FileName);
	}
}

//**********************************************************************
void ThemeObject::LoadBlocks(char FileName[])
{
	if (strlen(FileName) > 80)
		ErrorMessage("File name too long (Maximum 80 characters).");
	else
	{
		for (int i=0; i<7; ++i)
		{
			if (!Block[i].LoadBlock(FileName, (i*10), 0))
				return;
		}

		// If we made it this far, we successfully loaded all blocks
		strcpy(long_BlockPic, FileName);
	}
}

//**********************************************************************
void ThemeObject::LoadGameMusic(char FileName[])
{
	if (strlen(FileName) > 80)
		ErrorMessage("File name too long (Maximum 80 characters).");
	else
	{
		strcpy(long_GameMusic, FileName);
	}
}

//**********************************************************************
void ThemeObject::LoadPlaceBlockSound(char FileName[])
{
	if (strlen(FileName) > 80)
		ErrorMessage("File name too long (Maximum 80 characters).");
	else
	{
		strcpy(long_PlaceBlockSound, FileName);
	}
}

//**********************************************************************
void ThemeObject::LoadLinesSound(char FileName[])
{
	if (strlen(FileName) > 80)
		ErrorMessage("File name too long (Maximum 80 characters).");
	else
	{
		strcpy(long_LinesSound, FileName);
	}
}

//**********************************************************************
void ThemeObject::LoadTetrisSound(char FileName[])
{
	if (strlen(FileName) > 80)
		ErrorMessage("File name too long (Maximum 80 characters).");
	else
	{
		strcpy(long_TetrisSound, FileName);
	}
}


//**********************************************************************
void ThemeObject::LoadChangeLevelSound(char FileName[])
{
	if (strlen(FileName) > 80)
		ErrorMessage("File name too long (Maximum 80 characters).");
	else
	{
		strcpy(long_ChangeLevelSound, FileName);
	}
}

//**********************************************************************
void ThemeObject::LoadGameOverSound(char FileName[])
{
	if (strlen(FileName) > 80)
		ErrorMessage("File name too long (Maximum 80 characters).");
	else
	{
		strcpy(long_GameOverSound, FileName);
	}
}

//**********************************************************************
void ThemeObject::SetTLevel(int NewTLevel)
{
	TLevel = NewTLevel;
}

//**********************************************************************
void ThemeObject::SetFontColor(unsigned short Color)
{
	FontColor = Color;
}

//**********************************************************************
void ThemeObject::SetFontShadowColor(unsigned short Color)
{
	FontShadowColor = Color;
}

//**********************************************************************
void ThemeObject::SetUseFontShadow(bool Value)
{
	UseFontShadow = Value;
}

//**********************************************************************
void ThemeObject::SetDescription(char String[])
{
	strcpy(Description, String);
}

//**********************************************************************
void ThemeObject::SetMusicVolume(int Volume)
{
	MusicVolume = Volume;
}

//**********************************************************************
void ThemeObject::SetSoundVolume(int Volume)
{
	SoundVolume = Volume;
}

//**********************************************************************
void ThemeObject::GetDescriptionAddress(char **String)
{
	*String = Description;
}

//**********************************************************************
void ThemeObject::GetFileName(char FileName[])
{
	strcpy(FileName, long_filename);
}

//**********************************************************************
int ThemeObject::GetMusicVolume(void)
{
	return MusicVolume;
}

//**********************************************************************
int ThemeObject::GetSoundVolume(void)
{
	return SoundVolume;
}



// Auxiliary function definitions
//**********************************************************************
void InitBlocks(void)
{

	// Now define all 7 blocks

	Block[0].SetSize(2);
	Block[0].SetShape("11",
										"11");

	Block[1].SetSize(3);
	Block[1].SetShape("000",
										"111",
										"100");

	Block[2].SetSize(3);
	Block[2].SetShape("000",
										"111",
										"001");

	Block[3].SetSize(3);
	Block[3].SetShape("000",
										"011",
										"110");

	Block[4].SetSize(3);
	Block[4].SetShape("000",
										"110",
										"011");
	 
	Block[5].SetSize(3);
	Block[5].SetShape("000",
										"111",
										"010");
	 
	Block[6].SetSize(4);
	Block[6].SetShape("0000",
										"0000",
										"1111");

}

//**********************************************************************
void InitColorStruct(void)
{
	strcpy(Color[0].Name, "Black");
	strcpy(Color[1].Name, "White");
	strcpy(Color[2].Name, "Light Gray");
	strcpy(Color[3].Name, "Dark Gray");
	strcpy(Color[4].Name, "Purple");
	strcpy(Color[5].Name, "Magenta");
	strcpy(Color[6].Name, "Red");
	strcpy(Color[7].Name, "Dark Red");
	strcpy(Color[8].Name, "Yellow");
	strcpy(Color[9].Name, "Green");
	strcpy(Color[10].Name, "Dark Green");
	strcpy(Color[11].Name, "Light Blue");
	strcpy(Color[12].Name, "Blue");
	strcpy(Color[13].Name, "Dark Blue");
	strcpy(Color[14].Name, "Light Cyan");
	strcpy(Color[15].Name, "Cyan");

	Color[0].Index = BLACK;
	Color[1].Index = WHITE;
	Color[2].Index = LIGHTGRAY;
	Color[3].Index = DARKGRAY;
	Color[4].Index = PURPLE;
	Color[5].Index = MAGENTA;
	Color[6].Index = RED;
	Color[7].Index = DARKRED;
	Color[8].Index = YELLOW;
	Color[9].Index = GREEN;
	Color[10].Index = DARKGREEN;
	Color[11].Index = LIGHTBLUE;
	Color[12].Index = BLUE;
	Color[13].Index = DARKBLUE;
	Color[14].Index = LIGHTCYAN;
	Color[15].Index = CYAN;
}



/*
//**********************************************************************
// Makes sure that FileName (8.3 format) is not doubled (or tripled, etc.)
void VerifyShortPathName(char FileName[], bool Reset)
{
	HANDLE Handle;
	WIN32_FIND_DATA DirData;
	WIN32_FIND_DATA FileData;

	static Number;

	if (Reset)
		Number = 2;

	int StringLength = strlen(FileName);

	Handle = FindFirstFile(FileName, &DirData); // Get first file

	if (Handle == INVALID_HANDLE_VALUE)
		return;

	// Now check for more files of the same name





}

*/
