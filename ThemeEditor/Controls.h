// controls.h

#ifndef CONTROLS_H
#define CONTROLS_H

#include <windows.h>
#include <commctrl.h> // ***NOTE: Include "comctl32.lib" in the project libraries
#include <string.h>


// OpenSaveFileObject class definition

class OpenSaveFileObject
{
	public:
		// Constructor
		OpenSaveFileObject(void);

		// Mutator to set the hwndOwner field in the OPENFILENAME struct
		void SetWinHandle(HWND hwnd);

		// Member functions that display Open/Save Window. Return false if cancel is pressed.
		bool DisplayOpenWindow(LPCSTR FileFilter, LPCSTR InitialDir = NULL);
		bool DisplaySaveAsWindow(LPCSTR FileFilter, LPCSTR InitialDir = NULL);

		// Public data members (easier and faster than returning pointers to arrays)
		char CompletePath[512];
		char FileName[256];
		char Extension[4];
			
	private:
		OPENFILENAME fname;
		

};


// StatusBarObject class definition

class StatusBarObject
{
	public:
		// Constructors
		StatusBarObject(void); // Use this to create a global Status Bar. Member function Initialize()
													 // must be called within WinMain()

		StatusBarObject(HWND hwnd, int NumParts); // Use to create local Status Bar within WinMain()

		// Destructor
		~StatusBarObject(void);

		// Member functions
		void Initialize(HWND hwnd, int NumParts); // Call in WinMain() is using a global Status Bar
		void SetText(int Part, char String[]); // Set a string in StatusBar part


	private:
		HWND hWindow;
		HWND hStatusWindow;
		unsigned int *parts;

};

#endif
