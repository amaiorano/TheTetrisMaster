// ThemeLauncherDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ThemeLauncher.h"
#include "ThemeLauncherDlg.h"
#include ".\themelauncherdlg.h"
#include "OptionsDlg.h"

#include "TMTheme.h"
#include <vector>
#include <map>
#include <process.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Constants
const CString THEME_SEARCH_PATH = ".\\Themes\\*.*";
const CString TETRIS_EXE		= "Tetris.exe";
const CString WEBLINK			= "http://www.firebell.org/guardiansoft";

// Globals
struct ThemeInfo
{
	CString strPath;
	CString strDescrption;
};

typedef std::vector<ThemeInfo> ThemeInfoList;
ThemeInfoList g_themeInfoList;


// CAboutDlg dialog used for App About
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CThemeLauncherDlg dialog
CThemeLauncherDlg::CThemeLauncherDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CThemeLauncherDlg::IDD, pParent)
	, m_strDescriptionEditCtrl(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CThemeLauncherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_THEMES, m_themeListBox);
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, m_strDescriptionEditCtrl);
}

BEGIN_MESSAGE_MAP(CThemeLauncherDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_PLAY, OnBnClickedButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON_RANDOM, OnBnClickedButtonRandom)
	ON_BN_CLICKED(IDC_OPTIONS, OnBnClickedOptions)
	ON_BN_CLICKED(IDC_BUTTON_QUIT, OnBnClickedButtonQuit)
	ON_LBN_DBLCLK(IDC_LIST_THEMES, OnLbnDblclkListThemes)
	ON_BN_CLICKED(IDC_BUTTON_WEBLINK, OnBnClickedButtonWeblink)
	ON_LBN_SELCHANGE(IDC_LIST_THEMES, OnLbnSelchangeListThemes)
END_MESSAGE_MAP()

// My own member functions

void CThemeLauncherDlg::PopulateThemeList()
{	
	CFileFind themePathFinder;
	if ( !themePathFinder.FindFile(THEME_SEARCH_PATH) )
	{
		MessageBox("Could not find the theme root folder: " + THEME_SEARCH_PATH, 0, MB_OK|MB_ICONERROR);
		return;
	}

	// Found theme root folder, so recurse through it and look for themes
	bool bContinue = true;
	while ( bContinue )
	{
		bContinue = themePathFinder.FindNextFile()==TRUE;

		if ( !themePathFinder.IsDirectory() ) // Skip non-directories
			continue;

		// Check if the directory contains a file of the same
		// name with '.the' extension (the theme file)

		CString strThemeName = themePathFinder.GetFileName();
		CString strThemePath = themePathFinder.GetFilePath();
		
		CFileFind themeFileFinder;
		CString strThemeFullPath = strThemePath + "\\" + strThemeName + ".the";
		if ( !themeFileFinder.FindFile(strThemeFullPath) )
			continue; // Does not contain a theme file, so skip to next

		// Found a theme, attempt to load it up
		try
		{
			TMTheme theme;
			theme.LoadTheme((const char*)strThemeName);

			// Found a theme, add it to the list box
			m_themeListBox.AddString(strThemeName);

			// Store theme info loading later
			ThemeInfo themeInfo;
			themeInfo.strPath = strThemePath;
			themeInfo.strDescrption = theme.GetDescription().c_str();
			g_themeInfoList.push_back(themeInfo);
		}
		catch (...)
		{
			continue; // Skip this theme
		}
	}

	// Select first theme
	m_themeListBox.SetCurSel(0);

	// Fake the message that should be sent upon selection
	OnLbnSelchangeListThemes();
}


// CThemeLauncherDlg message handlers

BOOL CThemeLauncherDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	
	// Populate the theme list with all themes
	PopulateThemeList();
	
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CThemeLauncherDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CThemeLauncherDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CThemeLauncherDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CThemeLauncherDlg::OnBnClickedButtonPlay()
{
	// Get current selection in list
	int idx = m_themeListBox.GetCurSel();
	if ( idx == LB_ERR )
		return; // Nothing selected

	// Hide theme launcher
	ShowWindow(SW_HIDE);

	// Launch the game passing in selected theme as argument
	if ( _spawnl(_P_WAIT, TETRIS_EXE, TETRIS_EXE, g_themeInfoList[idx].strPath, NULL) == 0 )
	{
		MessageBox("Error launching theme", 0, MB_OK|MB_ICONERROR);
	}

	// Show launcher again
	ShowWindow(SW_SHOW);
}

void CThemeLauncherDlg::OnBnClickedButtonRandom()
{
	// We select a theme randomly, however, we make sure not to repeat any 
	// other themes that were selected randomly until we've done them all.

	const int iNumThemes = static_cast<int>( g_themeInfoList.size() );
	static int iThemesPlayed = 0; // Used to track number of themes played randomly so far

	if ( iNumThemes <= 0 )
		return; // No themes!

	typedef std::map<int, bool> IdxMap;
	static IdxMap s_idxMap; // Used to track selected themes (by index)

	// If we played all of them, reset statics
	if ( iThemesPlayed == iNumThemes )
	{
		iThemesPlayed = 0;
		s_idxMap.clear();
	}

	int iChooseIdx = -1;
	do // Keep randomly choosing an index until we find one we haven't yet played
	{
		iChooseIdx = rand() % iNumThemes;

	} while ( s_idxMap.find(iChooseIdx) != s_idxMap.end() );

	++iThemesPlayed; // Increment counter
	s_idxMap[iChooseIdx] = true; // Add to map

	// Now select it and play
	m_themeListBox.SetCurSel(iChooseIdx);
	OnBnClickedButtonPlay();
}

void CThemeLauncherDlg::OnBnClickedOptions()
{
	COptionsDlg dlg;
	dlg.DoModal();
}

void CThemeLauncherDlg::OnBnClickedButtonQuit()
{
	EndDialog(0);
}

void CThemeLauncherDlg::OnLbnDblclkListThemes()
{
	OnBnClickedButtonPlay(); // Same as clicking play button
}

void CThemeLauncherDlg::OnBnClickedButtonWeblink()
{
	// Make it open the default browser and go to the website
	::ShellExecute(m_hWnd, "open", WEBLINK, NULL, NULL, SW_SHOW);
}


void CThemeLauncherDlg::OnLbnSelchangeListThemes()
{
	// Set description for selected theme

	// Get current selection in list
	int idx = m_themeListBox.GetCurSel();
	if ( idx == LB_ERR )
		return; // Nothing selected

	m_strDescriptionEditCtrl = g_themeInfoList[idx].strDescrption;
	UpdateData(false);
}
