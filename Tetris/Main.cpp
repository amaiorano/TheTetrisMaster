#include "Tetris.h"
#include <string>
#include <sstream>
#include <vector>
using namespace std;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmdLine, int nCmdShow)
{
	// Set theme path - if none supplied, then use default
	string strThemePath = lpCmdLine;
	if (strThemePath.empty())
		strThemePath = "Guardian Soft\\Guardian Soft.the";

	// Extract theme name from path
	string::size_type extPos =			strThemePath.rfind(".");
	string::size_type backSlashPos =	strThemePath.rfind("\\");
	string strThemeName = strThemePath.substr(backSlashPos+1, extPos-backSlashPos-1);

	// Create the game engine and set the theme
	TheTetrisMaster gameEngine;
	gameEngine.SetThemeName(strThemeName);

	// Run the game
	int iRetVal = GameApp::Instance().Run(gameEngine);
	return (iRetVal==0? 1 : iRetVal); // Theme launcher expects 1 for success
}
