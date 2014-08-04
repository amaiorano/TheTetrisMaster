// OptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ThemeLauncher.h"
#include "OptionsDlg.h"

#include <GSGameLib.h> // For Config class
#include ".\optionsdlg.h"
Config g_gameConfig;

// Useful function that builds a list of CWnd pointers from an array
// of control IDs (terminated by NULL)
typedef std::vector<CWnd*> CWndPtrList;
CWndPtrList MakeCWndPtrList(CWnd* pParent, int wndIds[])
{
	CWndPtrList list;
	for (int i=0; wndIds[i]!=NULL; ++i)
		list.push_back( pParent->GetDlgItem(wndIds[i]) );
	return list;
}

// Control ids for controls in the Window Size group
int WinSizeGroup[] =
{
	IDC_RADIO_WIN_SIZE, IDC_RADIO_WIN_SIZE2, IDC_RADIO_WIN_SIZE3,
	IDC_RADIO_WIN_SIZE4, IDC_RADIO_WIN_SIZE5, IDC_RADIO_WIN_SIZE6,
	IDC_EDIT_WIN_WIDTH, IDC_EDIT_WIN_HEIGHT,

	NULL
};

enum eScreenMode
{
	SM_FULLSCREEN,
	SM_WINDOWED
};

enum eWindowSize
{
	WS_640x480,
	WS_800x600,
	WS_1024x768,
	WS_1152x864,
	WS_1280x1024,
	WS_CUSTOM
};


// COptionsDlg dialog

IMPLEMENT_DYNAMIC(COptionsDlg, CDialog)
COptionsDlg::COptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionsDlg::IDD, pParent)
	, m_iScreenModeRadioButton(0)
	, m_iWinSizeRadioButton(0)
	, m_iCustomWidthEditCtrl(0)
	, m_iCustomHeightEditCtrl(0)
	, m_bAudioCheckBox(FALSE)
{
}

COptionsDlg::~COptionsDlg()
{
}

void COptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_FULLSCREEN, m_iScreenModeRadioButton);
	DDX_Radio(pDX, IDC_RADIO_WIN_SIZE, m_iWinSizeRadioButton);
	DDX_Text(pDX, IDC_EDIT_WIN_WIDTH, m_iCustomWidthEditCtrl);
	DDX_Text(pDX, IDC_EDIT_WIN_HEIGHT, m_iCustomHeightEditCtrl);
	DDX_Check(pDX, IDC_CHECK_AUDIO, m_bAudioCheckBox);
}

BOOL COptionsDlg::OnInitDialog()
{
	// Load up configuration data
	g_gameConfig.AddVar("fullscreen", "true");
	g_gameConfig.AddVar("win_width", 640);
	g_gameConfig.AddVar("win_height", 480);
	g_gameConfig.AddVar("audio", "true");
	g_gameConfig.LoadConfigFile("TTM.cfg");


	// Set fullscreen/windowed
	bool bFullScreen = (g_gameConfig["fullscreen"] == "true");
	m_iScreenModeRadioButton = bFullScreen? SM_FULLSCREEN : SM_WINDOWED;

	// Set window size
	int winW = g_gameConfig["win_width"].AsInt();
	int winH = g_gameConfig["win_height"].AsInt();

	if (winW==640 && winH==480)
		m_iWinSizeRadioButton = WS_640x480;
	else if (winW==800 && winH==600)
		m_iWinSizeRadioButton = WS_800x600;
	else if (winW==1024 && winH==768)
		m_iWinSizeRadioButton = WS_1024x768;
	else if (winW==1152 && winH==864)
		m_iWinSizeRadioButton = WS_1152x864;
	else if (winW==1280 && winH==1024)
		m_iWinSizeRadioButton = WS_1280x1024;
	else
	{
		m_iWinSizeRadioButton = WS_CUSTOM;

		// Set the custom width & height in the edit boxes
		m_iCustomWidthEditCtrl = winW;
		m_iCustomHeightEditCtrl = winH;
	}

	// Set audio on/off
	m_bAudioCheckBox = (g_gameConfig["audio"] == "true");

	// Data Members -> Controls
	UpdateData(false);

	// Finally, simulate selection of fullscreen/windowed mode
	if ( bFullScreen )
		OnBnClickedRadioFullscreen();
	else
		OnBnClickedRadioWindow();

	return TRUE;
}

BEGIN_MESSAGE_MAP(COptionsDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO_FULLSCREEN, OnBnClickedRadioFullscreen)
	ON_BN_CLICKED(IDC_RADIO_WINDOW, OnBnClickedRadioWindow)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_EN_SETFOCUS(IDC_EDIT_WIN_WIDTH, OnEnSetfocusEditWinWidth)
	ON_EN_KILLFOCUS(IDC_EDIT_WIN_WIDTH, OnEnKillfocusEditWinWidth)
	ON_EN_SETFOCUS(IDC_EDIT_WIN_HEIGHT, OnEnSetfocusEditWinHeight)
	ON_EN_KILLFOCUS(IDC_EDIT_WIN_HEIGHT, OnEnKillfocusEditWinHeight)
END_MESSAGE_MAP()



// COptionsDlg message handlers

void COptionsDlg::OnBnClickedRadioFullscreen()
{
	// Full screen radio button clicked, disable the Window Size group    
	CWndPtrList list = MakeCWndPtrList(this, WinSizeGroup);
	CWndPtrList::iterator it;
	for (it = list.begin(); it != list.end(); ++it)
		(*it)->EnableWindow(false);
}

void COptionsDlg::OnBnClickedRadioWindow()
{
	// Window radio button clicked, enable the Window Size group
	CWndPtrList list = MakeCWndPtrList(this, WinSizeGroup);
	CWndPtrList::iterator it;
	for (it = list.begin(); it != list.end(); ++it)
		(*it)->EnableWindow(true);
}

void COptionsDlg::OnBnClickedOk()
{
	// Controls -> Data Members
	UpdateData(true); 

	// Set screen mode
	g_gameConfig["fullscreen"] = m_iScreenModeRadioButton==0? "true" : "false";

	// Set window size
	int winW, winH;
	switch (m_iWinSizeRadioButton)
	{
	case WS_640x480:	winW=640;	winH=480; break;
	case WS_800x600:	winW=800;	winH=600; break;
	case WS_1024x768:	winW=1024;	winH=768; break;
	case WS_1152x864:	winW=1152;	winH=864; break;
	case WS_1280x1024:	winW=1280;	winH=1024; break;
	case WS_CUSTOM:
		winW = m_iCustomWidthEditCtrl;
		winH = m_iCustomHeightEditCtrl;
		break;
	}
	g_gameConfig["win_width"] = winW;
	g_gameConfig["win_height"] = winH;

	// Set audio on/off
	g_gameConfig["audio"] = m_bAudioCheckBox? "true" : "false";

	// Save to config file
	g_gameConfig.SaveConfigFile("TTM.cfg");
	
	OnOK();
}

void COptionsDlg::OnEnSetfocusEditWinWidth()
{
	m_iWinSizeRadioButton = WS_CUSTOM;
	UpdateData(false);	
}

void COptionsDlg::OnEnKillfocusEditWinWidth()
{
	UpdateData(true);
}

void COptionsDlg::OnEnSetfocusEditWinHeight()
{
	OnEnSetfocusEditWinWidth();
}

void COptionsDlg::OnEnKillfocusEditWinHeight()
{
	OnEnKillfocusEditWinWidth();
}
