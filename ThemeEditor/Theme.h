// theme.h

#ifndef THEME_H
#define THEME_H

#include <string.h>
#include <fstream>
#include <ctype.h>
#include <direct.h>

#include "winstandard.h"
#include "video.h"
#include "font.h"
#include "block.h"

using namespace std;

// Font & Font Shadow Color defines
#define BLACK 0
#define WHITE 65535
#define LIGHTGRAY 33808
#define DARKGRAY 16904
#define PURPLE 49176
#define MAGENTA 63519
#define RED 63488
#define DARKRED 49152
#define YELLOW 65504
#define GREEN 2016
#define DARKGREEN 1536
#define LIGHTBLUE 2047
#define BLUE 31
#define DARKBLUE 24
#define LIGHTCYAN 50719
#define CYAN 33823


// Define color struct
struct ColorObject
{
	char Name[20];
	unsigned short Index;
};

extern ColorObject Color[16];


// Global buffers and variables
extern unsigned short *BACKGROUND;
extern unsigned short *MAINWIN;
extern unsigned short *GAMEINFO;

// Global vars to hold Program Path and Theme Path
extern char ProgramPath[512];
extern char ThemePath[512];

// Gloval vars for long filenames
extern char long_filename[80];
extern char long_BackgroundPic[80];
extern char long_TWindow[80];
extern char long_BlockPic[80];
extern char long_GameMusic[80];
extern char long_PlaceBlockSound[80];
extern char long_LinesSound[80];
extern char long_TetrisSound[80];
extern char long_ChangeLevelSound[80];
extern char long_GameOverSound[80];



// Declare block objects to display in the main window
extern BlockObject Block[7];

// // Auxiliary function prototypes
void InitBlocks(void);
void InitColorStruct(void);


// Class ThemeObject declaration

class ThemeObject
{
	public:
		ThemeObject(void);
		~ThemeObject(void);

		bool Save(void);
		void Load(char FileName[]);

		void Create(char FileName[]);
		void Update(void);
		void Reset(void);
		
		void TextOutput(void);
		bool ValidateThemeName(char string[]);

		// Inspectors
		unsigned short GetFontColor(void);
		unsigned short GetFontShadowColor(void);

		// Member functions to change each theme setting
		void LoadBackground(char FileName[]);
		void LoadGameWindow(char FileName[]);
		void LoadBlocks(char FileName[]);

		void LoadGameMusic(char FileName[]);
		void LoadPlaceBlockSound(char FileName[]);
		void LoadLinesSound(char FileName[]);
		void LoadTetrisSound(char FileName[]);
		void LoadChangeLevelSound(char FileName[]);
		void LoadGameOverSound(char FileName[]);

		void SetTLevel(int NewTLevel);
		void SetFontColor(unsigned short Color);
		void SetFontShadowColor(unsigned short Color);
		void SetUseFontShadow(bool Value);
		void SetMusicVolume(int Volume);
		void SetSoundVolume(int Volume);

		void SetDescription(char String[]);
		void GetDescriptionAddress(char **String);
		void GetFileName(char FileName[]);

		int GetMusicVolume(void);
		int GetSoundVolume(void);

	private:
		char filename[80];

		char BackgroundPic[80];
		char TWindow[80];
		char BlockPic[80];

		char GameMusic[80];
		char PlaceBlockSound[80];
		char LinesSound[80];
		char TetrisSound[80];
		char ChangeLevelSound[80];
		char GameOverSound[80];

		unsigned short MusicVolume;
		unsigned short SoundVolume;

		unsigned short TLevel;
		unsigned short FontColor;
		unsigned short FontShadowColor;
		bool UseFontShadow;

		char Description[255]; // Only allow user to enter max 250 chars

};

#endif
