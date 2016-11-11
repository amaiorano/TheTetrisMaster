// Global Function definitions

#include "globals.h"

// Global variables
HWND hwnd; // Handle to the main window
HINSTANCE hinst; // Handle to the current instance
HMENU hMenu; // Handle to the main menu

bool ThemeSaved = true;

// Declare personal library objects
VideoObject Video(320, 240); // Declare video object
FontObject Font(".\\graphics\\font.raw", 320*50);
TextObject Text; // Declare text object to output simple text
OpenSaveFileObject OpenSaveFile; // OpenSaveFile object used to open and save files
ThemeObject Theme;
//StatusBarObject StatusBar; // Declare the status bar


//**********************************************************************
// Use this function to enable/disable the menu items
void SetupMenuItems(bool Enable)
{
	EnableMenuItem(hMenu, IDM_PLAYGAME, Enable ? MF_ENABLED : MF_GRAYED);
	EnableMenuItem(hMenu, IDM_SAVETHEME, Enable ? MF_ENABLED : MF_GRAYED);
	EnableMenuItem(hMenu, IDM_FULLSCREEN, Enable ? MF_ENABLED : MF_GRAYED);
	EnableMenuItem(hMenu, IDM_LOADBACKGROUND, Enable ? MF_ENABLED : MF_GRAYED);
	EnableMenuItem(hMenu, IDM_LOADBLOCKS, Enable ? MF_ENABLED : MF_GRAYED);
	EnableMenuItem(hMenu, IDM_LOADGAMEWINDOW, Enable ? MF_ENABLED : MF_GRAYED);
	EnableMenuItem(hMenu, IDM_LOADMUSIC, Enable ? MF_ENABLED : MF_GRAYED);
	EnableMenuItem(hMenu, IDM_LOADPLACEBLOCKSOUND, Enable ? MF_ENABLED : MF_GRAYED);
	EnableMenuItem(hMenu, IDM_LOAD123LINESSOUND, Enable ? MF_ENABLED : MF_GRAYED);
	EnableMenuItem(hMenu, IDM_LOADTETRISSOUND, Enable ? MF_ENABLED : MF_GRAYED);
	EnableMenuItem(hMenu, IDM_LOADCHANGELEVELSOUND, Enable ? MF_ENABLED : MF_GRAYED);
	EnableMenuItem(hMenu, IDM_LOADGAMEOVERSOUND, Enable ? MF_ENABLED : MF_GRAYED);
	EnableMenuItem(hMenu, IDM_SETVOLUME, Enable ? MF_ENABLED : MF_GRAYED);
	EnableMenuItem(hMenu, IDM_SELECTTLEVEL, Enable ? MF_ENABLED : MF_GRAYED);
	EnableMenuItem(hMenu, IDM_FONTOPTIONS, Enable ? MF_ENABLED : MF_GRAYED);
	EnableMenuItem(hMenu, IDM_DESCRIPTION, Enable ? MF_ENABLED : MF_GRAYED);
}

//**********************************************************************
void SetThemeSaved(bool saved)
{
	ThemeSaved = saved;
	EnableMenuItem(hMenu, IDM_SAVETHEME, saved? MF_GRAYED : MF_ENABLED);
}


//**********************************************************************
// Windows message-handling callback function prototype

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int response; // Used by WM_COMMAND, case IDM_EXIT
	int integer;
	char string[255];
	char string2[255];

	switch (message)
	{
		case WM_CREATE: // Called only once when window is created			
			break;

		case WM_PAINT: // Paint the RAW file
			Video.Paint(); // Paint bitmap to screen
			Theme.TextOutput();		
			break;
		

		case WM_COMMAND: // A menu selection was made or accelerator key was pressed
			switch (LOWORD(wParam))
			{
				// POPUP "&Theme" selections
				case IDM_NEWTHEME:
					Theme.GetFileName(string);
					if (strlen(string) && !ThemeSaved)
					{
						integer = MessageBox(NULL, "The current theme has not been saved.\nAre you sure you want to create a new theme?", "New Theme", MB_YESNO | MB_ICONHAND);
						if (integer == IDNO)
							break;
					}

					// Display a dialogue box asking the user to enter the name of his/her theme
					DialogBox(hinst, "NEWTHEME", hwnd, (DLGPROC)NewThemeDialogProc);					
					break;
			
				case IDM_OPENTHEME: 
					Theme.GetFileName(string);
					if (strlen(string) && !ThemeSaved)
					{
						integer = MessageBox(NULL, "The current theme has not been saved.\nAre you sure you want to open a new theme?", "Open Theme", MB_YESNO | MB_ICONHAND);
						if (integer == IDNO)
							break;
					}

					if (OpenSaveFile.DisplayOpenWindow("Theme Files (*.the)\0*.the\0\0\0", ".\\themes"))
					{						
						Theme.Load(OpenSaveFile.FileName);
						Theme.Update();						
						SetupMenuItems(true);
						SetThemeSaved(true);
					}					
					break;

				case IDM_SAVETHEME:
				case IDM_FORCESAVETHEME:
					if (Theme.Save())
						SetThemeSaved(true);
					break;

				case IDM_PLAYGAME:

					if (!ThemeSaved)
					{
						ErrorMessage("You must save your theme before playing.");
					}
					else
					{
						// Spawn to game
						strcpy(string, ThemePath);
						strcat(string, "\\");

						Theme.GetFileName(string2);
						strcat(string, string2);

						GetShortPathName(string, string, (DWORD)strlen(string));						

						// Set current directory to where tetris.exe is
						SetCurrentDirectory(ProgramPath);

						integer = (int)_spawnl(_P_WAIT, "tetris.exe", "tetris.exe", string, NULL);
						if (integer == 0)
						{
							ErrorMessage("Unable to load theme.");
						}
					}
					break;
	
				case IDM_EXIT:
					SendMessage(hwnd, WM_CLOSE, 0, 0);
					break;

				// POPUP "&View"
				case IDM_FULLSCREEN:
					strcpy(string, ProgramPath);
					strcat(string, "\\EditFiles");
					SetCurrentDirectory(string);

					Video.ScreenShot("temp.raw");

					strcat(string, "\\fullscrn.exe");
					
					_spawnl(_P_WAIT, string, string, "temp.raw", NULL); // Run "fullscrn.exe temp.raw"

					DeleteFile("temp.raw");
					break;


				// POPUP "&Graphics" selections
				case IDM_LOADBACKGROUND:
					if (OpenSaveFile.DisplayOpenWindow("Raw File Format (*.raw)\0*.raw\0\0\0"))
					{
						SetThemeSaved(false);
						Theme.LoadBackground(OpenSaveFile.FileName);
						Theme.Update();
					}
					break;

				case IDM_LOADBLOCKS:
					if (OpenSaveFile.DisplayOpenWindow("Raw File Format (*.raw)\0*.raw\0\0\0"))
					{
						SetThemeSaved(false);
						Theme.LoadBlocks(OpenSaveFile.FileName);
						Theme.Update();
					}
					break;

				case IDM_LOADGAMEWINDOW:
					if (OpenSaveFile.DisplayOpenWindow("Raw File Format (*.raw)\0*.raw\0\0\0"))
					{
						SetThemeSaved(false);
						Theme.LoadGameWindow(OpenSaveFile.FileName);
						Theme.Update();
					}
					break;

				
				// POPUP "&Music and Sound" selections
				case IDM_LOADMUSIC:
					if (OpenSaveFile.DisplayOpenWindow("Music Module File (*.mod;*.s3m;*.mtm;*.xm)\0*.mod;*.s3m;*.mtm;*.xm\0\0\0"))
					{
						SetThemeSaved(false);
						Theme.LoadGameMusic(OpenSaveFile.FileName);
						Theme.Update();
					}
					break;

				case IDM_LOADPLACEBLOCKSOUND:
					if (OpenSaveFile.DisplayOpenWindow("Wave File (*.wav)\0*.wav\0\0\0"))
					{
						SetThemeSaved(false);
						Theme.LoadPlaceBlockSound(OpenSaveFile.FileName);
						Theme.Update();
					}
					break;

				case IDM_LOAD123LINESSOUND:
					if (OpenSaveFile.DisplayOpenWindow("Wave File (*.wav)\0*.wav\0\0\0"))
					{
						SetThemeSaved(false);
						Theme.LoadLinesSound(OpenSaveFile.FileName);
						Theme.Update();
					}
					break;

				case IDM_LOADTETRISSOUND:
					if (OpenSaveFile.DisplayOpenWindow("Wave File (*.wav)\0*.wav\0\0\0"))
					{
						SetThemeSaved(false);
						Theme.LoadTetrisSound(OpenSaveFile.FileName);
						Theme.Update();
					}
					break;

				case IDM_LOADCHANGELEVELSOUND:
					if (OpenSaveFile.DisplayOpenWindow("Wave File (*.wav)\0*.wav\0\0\0"))
					{
						SetThemeSaved(false);
						Theme.LoadChangeLevelSound(OpenSaveFile.FileName);
						Theme.Update();
					}
					break;

				case IDM_LOADGAMEOVERSOUND:
					if (OpenSaveFile.DisplayOpenWindow("Wave File (*.wav)\0*.wav\0\0\0"))
					{
						SetThemeSaved(false);
						Theme.LoadGameOverSound(OpenSaveFile.FileName);
						Theme.Update();
					}
					break;

				case IDM_SETVOLUME:
					DialogBox(hinst, "SETVOLUME", hwnd, (DLGPROC)SetVolumeDialogProc);
					break;

				// POPUP "&Other Options" selections

				case IDM_SELECTTLEVEL:
					// Display a dialogue box asking the user to enter new Transparency Level
					DialogBox(hinst, "SELECTTLEVEL", hwnd, (DLGPROC)SelectTLevelDialogProc);
					break;

				case IDM_FONTOPTIONS:
					DialogBox(hinst, "FONTOPTIONS", hwnd, (DLGPROC)FontOptionsDialogProc);
					break;

				case IDM_DESCRIPTION:
					DialogBox(hinst, "DESCRIPTION", hwnd, (DLGPROC)DescriptionDialogProc);
					break;

					// POPUP "&Help" selections
				case IDM_HELPTOPICS:
					strcpy(string, ProgramPath);					
					strcat(string, "\\EditFiles\\edithelp.hlp");
					WinHelp(hwnd, string, HELP_FINDER, 0);
					break;

				case IDM_ABOUT:
					MessageBox(NULL, 
						"The Tetris Master Theme Editor v. 2.00\n"
						"\n"
						"Programmed by: Antonio Maiorano\n"
						"E-Mail: daroou@videotron.ca\n"
						"\n"
						"Visit our homepage at\n"
						"http://www.firebell.org/guardiansoft\n"
						"\n"
						"All comments are welcome!", 

						"About TTM Theme Editor", MB_OK);
					break;
			}
			break; // End of case WM_COMMAND

		case WM_CLOSE: // User wants to quit the program
			if (ThemeSaved)
				DestroyWindow(hwnd);
			else
			{
				response = MessageBox(hwnd, "Your theme has not been saved.\nQuit the program?", "Exit", MB_YESNO);
				if (response == IDYES)
				{
					DestroyWindow(hwnd);
				}
			}
			break;

		case WM_DESTROY:
			PostQuitMessage(1);
			break;

		default: // Let windows handle the rest
			return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

//**********************************************************************
// Windows call-back procedure to handle "New Theme" Dialog Box

BOOL CALLBACK NewThemeDialogProc(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	char string[85];
	//int integer;

	switch (message)
	{
		case WM_INITDIALOG:
			// Set the focus on the edit box
			SetFocus(GetDlgItem(hdwnd, IDC_NEWTHEMEEDIT));
			break;
		
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case IDOK:
					GetDlgItemText(hdwnd, IDC_NEWTHEMEEDIT, string, 80);
					if (Theme.ValidateThemeName(string))
					{						
						Theme.Create(string);
						SetupMenuItems(true);
						SetThemeSaved(false);
						EndDialog(hdwnd, true);
					}						

					// Highlight the user's text
					SetFocus(GetDlgItem(hdwnd, IDC_NEWTHEMEEDIT));

					break;

				case IDCANCEL:
					EndDialog(hdwnd, false);
					break;
			}
	}
	return false;
}

//**********************************************************************
// Windows call-back procedure to handle "Window Transparency Level" Dialog Box

BOOL CALLBACK SelectTLevelDialogProc(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int integer;

	switch (message)
	{
		case WM_INITDIALOG:
			// Set the focus on the edit box
			SetFocus(GetDlgItem(hdwnd, IDC_SELECTTLEVEL));
			break;

		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case ID_TLEVELAPPLY:
					integer = GetDlgItemInt(hdwnd, IDC_SELECTTLEVEL, NULL, FALSE);
					if (integer < 0 || integer > 255)
					{
						MessageBox(hwnd, "Value must be an integer between 0 and 255", "Error", MB_OK | MB_ICONHAND);
					}						
					else
					{
						SetThemeSaved(false);
						Theme.SetTLevel(integer);
						Theme.Update();						
					}

					// Highlight the user's text
					SetFocus(GetDlgItem(hdwnd, IDC_SELECTTLEVEL));
					break;

				case IDOK:
					EndDialog(hdwnd, false);
					break;
			}
	}
	return false;
}

//**********************************************************************
// Windows call-back procedure to handle "Select Color" Dialog Box

BOOL CALLBACK FontOptionsDialogProc(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int i;
	unsigned long color;
	static bool UserInput = false;	

	switch (message)
	{
		case WM_INITDIALOG:
			// Initialize List Box
			for (i=0; i<16; ++i)
				SendDlgItemMessage(hdwnd, IDC_COLORLIST, LB_ADDSTRING, 0, (LPARAM)Color[i].Name);

			// Set the edit boxes to their respective current selections
			SetDlgItemInt(hdwnd, IDC_FONTEDIT1, Theme.GetFontColor(), false);
			SetDlgItemInt(hdwnd, IDC_FONTEDIT2, Theme.GetFontShadowColor(), false);
			break;

		case WM_COMMAND:
			switch (LOWORD(wParam))
			{			

				case IDC_COLORLIST: // Item selected in List Box
					UserInput = false;
					break;

				case IDC_FONTEDIT1: // User wants to enter his/her own value
				case IDC_FONTEDIT2:
					UserInput = true;
					break;


				case ID_FONTAPPLY1:
					if (UserInput)
					{
						color = GetDlgItemInt(hdwnd, IDC_FONTEDIT1, NULL, false);						
					}
					else
					{
						i = (int)SendDlgItemMessage(hdwnd, IDC_COLORLIST, LB_GETCURSEL, 0, 0);
						color = Color[i].Index;
						SetDlgItemInt(hdwnd, IDC_FONTEDIT1, color, false);
						UserInput = false; // I have to do this because the previous statement sends an IDC_FONEDIT1
					}
					
					if (color > 65535)
					{
						MessageBox(NULL, "Invalid color index. Must be between 0 and 65535.", "Error", MB_OK | MB_ICONHAND);
						break;
					}

					Theme.SetFontColor((unsigned short)color);
					Theme.Update();
					SetThemeSaved(false);
					break;

				case ID_FONTAPPLY2:
					if (UserInput)
					{
						color = GetDlgItemInt(hdwnd, IDC_FONTEDIT2, NULL, false);						
					}
					else
					{
						i = (int)SendDlgItemMessage(hdwnd, IDC_COLORLIST, LB_GETCURSEL, 0, 0);
						color = Color[i].Index;
						SetDlgItemInt(hdwnd, IDC_FONTEDIT2, color, false);
						UserInput = false; // I have to do this because the previous statement sends an IDC_FONEDIT2
					}					

					if (color > 65535)
					{
						MessageBox(NULL, "Invalid color index. Must be between 0 and 65535.", "Error", MB_OK | MB_ICONHAND);
						break;
					}
					
					Theme.SetFontShadowColor((unsigned short)color);
					Theme.Update();
					SetThemeSaved(false);
					break;

				case ID_USEFONTYES:
					Theme.SetUseFontShadow(true);
					Theme.Update();
					SetThemeSaved(false);
					break;

				case ID_USEFONTNO:
					Theme.SetUseFontShadow(false);
					Theme.Update();
					SetThemeSaved(false);
					break;

				case IDOK:
					EndDialog(hdwnd, true);
					break;

				case IDCANCEL:
					EndDialog(hdwnd, false);
					break;
			}
	}

	return false;
}


//**********************************************************************
// Windows call-back procedure to handle "Description" Dialog Box

BOOL CALLBACK DescriptionDialogProc(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	char String[255];
	char *Description;

	switch (message)
	{
		case WM_INITDIALOG:
			Theme.GetDescriptionAddress(&Description);

			SetDlgItemText(hdwnd, IDC_EDIT_DESCRIPTION, Description);
			
			// Set the focus on the edit box
			SetFocus(GetDlgItem(hdwnd, IDC_EDIT_DESCRIPTION));
			break;
		
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case IDOK:
					GetDlgItemText(hdwnd, IDC_EDIT_DESCRIPTION, String, 255);

					if (strlen(String) > 250)
					{
						ErrorMessage("Your description cannot be longer than 250 characters.");
						SetFocus(GetDlgItem(hdwnd, IDC_EDIT_DESCRIPTION));
					}
					else
					{
						SetThemeSaved(false);
						Theme.SetDescription(String);
						EndDialog(hdwnd, true);
					}
					break;

				case IDCANCEL:
					EndDialog(hdwnd, false);
					break;
			}
	}
	return false;
}

//**********************************************************************
// Windows call-back procedure to handle "Description" Dialog Box

BOOL CALLBACK SetVolumeDialogProc(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int i, j;

	switch (message)
	{
		case WM_INITDIALOG:
			// Set the current volume levels in the edit boxes
			SetDlgItemInt(hdwnd, IDC_EDIT_MUSIC, Theme.GetMusicVolume(), false);
			SetDlgItemInt(hdwnd, IDC_EDIT_SOUND, Theme.GetSoundVolume(), false);
			break;
		
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case IDOK:
					i = GetDlgItemInt(hdwnd, IDC_EDIT_MUSIC, NULL, false);
					j = GetDlgItemInt(hdwnd, IDC_EDIT_SOUND, NULL, false);

					if (i > 64 || j > 64)
						ErrorMessage("Volume level must be between 0 and 64");
					else
					{
						Theme.SetMusicVolume(GetDlgItemInt(hdwnd, IDC_EDIT_MUSIC, NULL, false));
						Theme.SetSoundVolume(GetDlgItemInt(hdwnd, IDC_EDIT_SOUND, NULL, false));
						Theme.Update();
						SetThemeSaved(false);
						EndDialog(hdwnd, false);
					}
					break;

				case IDCANCEL:
					EndDialog(hdwnd, false);
					break;
			}
	}
	return false;
}