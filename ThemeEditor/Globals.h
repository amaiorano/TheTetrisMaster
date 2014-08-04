// globals.h

// Define all global variables and window functions

#ifndef GLOBALS_H
#define GLOBALS_H

#include <windows.h>
#include <string.h>
#include <process.h>
#include "resources.h"
#include "controls.h"
#include "video.h"
#include "font.h"
#include "theme.h"


// Global variables

extern HWND hwnd; // Handle to the main window
extern HINSTANCE hinst; // Handle to the current instance
extern HMENU hMenu; // Handle to the main menu

extern bool ThemeSaved;

// Declare personal library objects
extern VideoObject Video; // Declare video object
extern FontObject Font;
extern TextObject Text; // Declare text object to output simple text
extern OpenSaveFileObject OpenSaveFile; // OpenSaveFile object used to open and save files
extern ThemeObject Theme;
//StatusBarObject StatusBar; // Declare the status bar

// Global function prototypes

// Use this function to enable/disable the menu items
void SetupMenuItems(bool Enable);
void SetThemeSaved(bool saved);

// Windows message-handling callback function prototype
LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK NewThemeDialogProc(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK SelectTLevelDialogProc(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK FontOptionsDialogProc(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DescriptionDialogProc(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK SetVolumeDialogProc(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam);

#endif
