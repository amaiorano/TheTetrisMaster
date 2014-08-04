// controls.cpp

#include "controls.h"

#pragma comment(lib, "comctl32.lib")


// OpenSaveFileObject definition

//**********************************************************************
// Constructor
OpenSaveFileObject::OpenSaveFileObject(void)
{
	// Initialize private variables
	memset(CompletePath, 0, sizeof(CompletePath));

	// Initialize OPENFILENAME fname struct

	memset(&fname, 0, sizeof(OPENFILENAME));
	fname.lStructSize = sizeof(OPENFILENAME); // Size of the structure
	
	fname.nFilterIndex = 1; // By default, display 1st file type defined in fileFilter

	fname.lpstrFile = CompletePath; // Where the complete path to the file will be stored
	fname.nMaxFile = sizeof(CompletePath); // Size of lpstrFile array

	fname.lpstrFileTitle = FileName; // Where only the file name will be stored
	fname.nMaxFileTitle = sizeof(FileName)-1;

	fname.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_LONGNAMES; // Set flags		
}

//**********************************************************************
// Mutator to set the hwndOwner field in the OPENFILENAME struct
void OpenSaveFileObject::SetWinHandle(HWND hwnd)
{
	fname.hwndOwner = hwnd; // Handle to parent window
}

//**********************************************************************
// Member function to Display the "Open" window. Returns false if "Cancel" is pressed.
bool OpenSaveFileObject::DisplayOpenWindow(LPCSTR FileFilter, LPCSTR InitialDir)
{
	fname.lpstrInitialDir = InitialDir; // Initial directory to open into  (NULL by default)
	fname.lpstrFilter = FileFilter; // File filter (e.g. "C++ file\0*.CPP\0\0\0");
	
	if (GetOpenFileName(&fname))
	{
		// Store the file extension in Extension[] array
		strncpy(Extension, CompletePath + fname.nFileExtension, 3);
		return true;
	}

	return false;
}

//**********************************************************************
// Member function to Display the "Save As" window. Returns false if "Cancel" is pressed.
bool OpenSaveFileObject::DisplaySaveAsWindow(LPCSTR FileFilter, LPCSTR InitialDir)
{
	fname.lpstrFilter = FileFilter; // File filter (e.g. "C++ file\0*.CPP\0\0\0");
	fname.lpstrInitialDir = InitialDir; // Initial directory to open into (NULL by default)

	// Set lpstrDefExt to point to a location where the default extension is specified
	// (This will be used when the user does not specify an index when saving)
	fname.lpstrDefExt = CompletePath + fname.nFileExtension;
	
	if (GetSaveFileName(&fname))
	{
		// Store the file extension in Extension[] array
		strncpy(Extension, CompletePath + fname.nFileExtension, 3);
		return true;
	}
	else
		return false;
}



// StatusBarObject defintion


//**********************************************************************
// Constructor
StatusBarObject::StatusBarObject(void)
{
	// Does nothing. Initialize() member funciton is expected to be called
	// within WinMain()
}

//**********************************************************************
// Second Constructor
StatusBarObject::StatusBarObject(HWND hwnd, int NumParts)
{
	// Set data members
	hWindow = hwnd;

	parts = new unsigned int [NumParts];

	RECT WinDimensions;
	GetClientRect(hwnd, &WinDimensions); // Get the window's dimensions
	int SinglePartWidth = WinDimensions.right / NumParts; // Get width of one part

	// Fill up the parts array
	for (int i=0; i<NumParts; ++i)
		parts[i] = SinglePartWidth * (i+1);

	// Create the status bar
	hStatusWindow = CreateStatusWindow(WS_CHILD | WS_VISIBLE, NULL, hWindow, 0);

	// Set the number of parts
	SendMessage(hStatusWindow, SB_SETPARTS, (WPARAM) NumParts, (LPARAM) parts);
}

//**********************************************************************
// Destructor
StatusBarObject::~StatusBarObject(void)
{
	delete [] parts;
}

//**********************************************************************
// Call in WinMain() is using a global Status Bar
void StatusBarObject::Initialize(HWND hwnd, int NumParts)
{
	// Set data members
	hWindow = hwnd;

	parts = new unsigned int [NumParts];

	RECT WinDimensions;
	GetClientRect(hwnd, &WinDimensions); // Get the window's dimensions
	int SinglePartWidth = WinDimensions.right / NumParts; // Get width of one part

	// Fill up the parts array
	for (int i=0; i<NumParts; ++i)
		parts[i] = SinglePartWidth * (i+1);

	// Create the status bar
	hStatusWindow = CreateStatusWindow(WS_CHILD | WS_VISIBLE, NULL, hWindow, 0);

	// Set the number of parts
	SendMessage(hStatusWindow, SB_SETPARTS, (WPARAM) NumParts, (LPARAM) parts);
}

//**********************************************************************
// Set a string in StatusBar part
void StatusBarObject::SetText(int Part, char String[])
{
	SendMessage(hStatusWindow, SB_SETTEXT, (WPARAM)Part, (LPARAM)String);
}
