#include "TMTheme.h"

#include <fstream>
using namespace std;


TMTheme::TMTheme()
{
}

TMTheme::~TMTheme()
{
}

void TMTheme::LoadTheme(std::string strThemeName)
{
	// Because I was silly back in the day and simply shot out the theme class
	// directly to a file, I now have to carefully load it back. I was using
	// the Watcom compiler, which by default, would NOT introduce padding between
	// data members, so here we force the compiler to do the same.
#ifdef WIN32
	#pragma pack(1)
#else
	#error Make sure to set compiler option to pack this struct to 1 (no padding)
#endif
	struct OldFileValues // Strict old file formatting
	{
		char szFileName[MAX_STRING];

		char szBackgroundPic[MAX_STRING];
		char szTWindow[MAX_STRING];
		char szBlockPic[MAX_STRING];

		char szGameMusic[MAX_STRING];
		char szPlaceBlockSound[MAX_STRING];
		char szLinesSound[MAX_STRING];
		char szTetrisSound[MAX_STRING];
		char szChangeLevelSound[MAX_STRING];
		char szGameOverSound[MAX_STRING];

		unsigned short iMusicVolume;		// [0,64]
		unsigned short iSoundVolume;		// [0,64]

		unsigned short iTranspLevel;		// [0,255]
		unsigned short iFontColor;			// [0,65535]
		unsigned short iFontShadowColor;	// [0,65535]
		bool bUseFontShadow;

		char szDescription[MAX_DESCRIPTION_STRING];
	} oldFileValues;

	// Try loading up the theme file
	const string strThemeFolder = "./Themes/";
	string strThemePath = strThemeFolder + strThemeName + "/";
	string strThemeFileName = strThemeName + ".the";

	ifstream fin( (strThemePath+strThemeFileName).c_str(), ios::binary);
	if ( !fin )
		throw std::invalid_argument("Invalid theme: " + strThemeName);

	// Just read in all the data directly into the struct
	fin.read((char*)&oldFileValues, sizeof(oldFileValues));

	// Now store/convert the data to our own data members
	m_strBackgroundImage = strThemePath + oldFileValues.szBackgroundPic;
	m_strTranspWinImage = strThemePath + oldFileValues.szTWindow;
	m_strAllBlocksImage = strThemePath + oldFileValues.szBlockPic;
	m_strGameMusic = strThemePath + oldFileValues.szGameMusic;
	m_strPlaceBlockSound = strThemePath + oldFileValues.szPlaceBlockSound;
	m_strLinesSound = strThemePath + oldFileValues.szLinesSound;
	m_strTetrisSound = strThemePath + oldFileValues.szTetrisSound;
	m_strChangeLevelSound = strThemePath + oldFileValues.szChangeLevelSound;
	m_strGameOverSound = strThemePath + oldFileValues.szGameOverSound;

	m_strDescription = oldFileValues.szDescription;

	// Convert transparancy level from [0,255] range to [0,1]
	m_fTranspLevel = static_cast<float>(oldFileValues.iTranspLevel / 255.0f);

	// Convert 16-bit font colors to RGB values in range [0,1]
	m_fontColor = ColorFuncs::ExtractRGB565(oldFileValues.iFontColor);
	m_fontShadowColor = ColorFuncs::ExtractRGB565(oldFileValues.iFontShadowColor);

	// Convert Music/Sound volume from range [0,64] to [0,1]
	m_fMusicVolume = oldFileValues.iMusicVolume / 64.0f;
	m_fSoundVolume = oldFileValues.iSoundVolume / 64.0f;
}
