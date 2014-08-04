// ThemeLauncherDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CThemeLauncherDlg dialog
class CThemeLauncherDlg : public CDialog
{
// Construction
public:
	CThemeLauncherDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_THEMELAUNCHER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


private:
	void PopulateThemeList();

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonPlay();
	afx_msg void OnBnClickedButtonRandom();
	afx_msg void OnBnClickedOptions();
	afx_msg void OnBnClickedButtonQuit();
private:
	CListBox m_themeListBox;
public:
	afx_msg void OnLbnDblclkListThemes();
	afx_msg void OnBnClickedButtonWeblink();
	afx_msg void OnLbnSelchangeListThemes();
private:
	CString m_strDescriptionEditCtrl;
};
