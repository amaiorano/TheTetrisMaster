#include "Tetris.h"
#include <string>
#include <sstream>
#include <vector>
using namespace std;

// Comment out when not profiling
#define PROFILING

// Comment out when using Windows subsystem, otherwise keep it for Console
/* #define CONSOLE_APP */

vector<string> argList; // Will contain command line arguments

#ifdef CONSOLE_APP
int main(int argc, char* argv[])
{
	for (int i=0; i<argc; ++i)
		argList.push_back(argv[i]);

#else
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmdLine, int nCmdShow)
{
	// Extract command line parameters
	stringstream cmdLineStream(GetCommandLine());
	string strTemp;
	cmdLineStream >> strTemp; 
	argList.push_back(strTemp); // First param: program name
	argList.push_back(cmdLineStream.str()); // Second param: theme path

#endif

	// Ignore input argument in profiling mode, we default to "Guardian Soft" theme
	// Reason: Profiler bug fails to spawn app if arguments specified in project
#ifndef PROFILING
	if ( argList.size() != 2 )
	{
		cerr << "Usage (example): " << argList[0] << " .\\themes\\default\\default.the" << endl;
		return -1;
	}
#endif

	// Set theme path - if none supplied, then use default
	string strThemePath = ( argList.size()>1? argList[1] : "Guardian Soft");

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
