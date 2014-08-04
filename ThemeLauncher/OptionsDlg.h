#pragma once


// COptionsDlg dialog

class COptionsDlg : public CDialog
{
	DECLARE_DYNAMIC(COptionsDlg)

public:
	COptionsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~COptionsDlg();

// Dialog Data
	enum { IDD = IDD_OPTIONS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
private:
	int m_iScreenModeRadioButton;
	int m_iWinSizeRadioButton;
public:
	afx_msg void OnBnClickedRadioFullscreen();
	afx_msg void OnBnClickedRadioWindow();
private:
	int m_iCustomWidthEditCtrl;
	int m_iCustomHeightEditCtrl;
	BOOL m_bAudioCheckBox;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnSetfocusEditWinWidth();
	afx_msg void OnEnKillfocusEditWinWidth();
	afx_msg void OnEnSetfocusEditWinHeight();
	afx_msg void OnEnKillfocusEditWinHeight();
};
