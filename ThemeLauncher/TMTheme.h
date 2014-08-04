#ifndef _TMTHEME_H_
#define _TMTHEME_H_

#include <string>
#include <GSGameLib.h>

const int MAX_STRING = 80;
const int MAX_DESCRIPTION_STRING = 255;

class TMTheme
{
public:
	typedef std::string string;

	TMTheme();
	~TMTheme();

	// Loads up the good old theme file
	void LoadTheme(string strThemeName);

	string GetBackgroundImageFileName() const	{ return m_strBackgroundImage; }
	string GetTranpWinImageFileName() const		{ return m_strTranspWinImage; }
	string GetAllBlocksImageFileName() const	{ return m_strAllBlocksImage; }

	string GetGameMusicFileName() const			{ return m_strGameMusic; }
	string GetPlaceBlockSoundFileName() const	{ return m_strPlaceBlockSound; }
	string GetLinesSoundFileName() const		{ return m_strLinesSound; }
	string GetTetrisSoundFileName() const		{ return m_strTetrisSound; }
	string GetChangeLevelSoundFileName() const	{ return m_strChangeLevelSound; }
	string GetGameOverSoundFileName() const		{ return m_strGameOverSound; }

	string GetDescription() const				{ return m_strDescription; }

	float GetTranspLevel() const				{ return m_fTranspLevel; }

	const Color3F& GetFontColor() const			{ return m_fontColor; }
	const Color3F& GetFontShadowColor() const	{ return m_fontShadowColor; }

	float GetMusicVolume() const				{ return m_fMusicVolume; }
	float GetSoundVolume() const				{ return m_fSoundVolume; }

private:
	string m_strBackgroundImage;
	string m_strTranspWinImage;
	string m_strAllBlocksImage;

	string m_strGameMusic;
	string m_strPlaceBlockSound;
	string m_strLinesSound;
	string m_strTetrisSound;
	string m_strChangeLevelSound;
	string m_strGameOverSound;

	string m_strDescription;
    
	float m_fTranspLevel; // [0,1]
	Color3F m_fontColor;
	Color3F m_fontShadowColor;

	float m_fMusicVolume; // [0,1]
	float m_fSoundVolume; // [0,1]
};

#endif // _TMTHEME_H_