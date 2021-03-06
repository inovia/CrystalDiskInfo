﻿/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : https://crystalmark.info/
//      License : The MIT License
/*---------------------------------------------------------------------------*/

#include "stdafx.h"
#include "DiskInfo.h"
#include "DiskInfoDlg.h"
#include "SoundSettingDlg.h"

IMPLEMENT_DYNCREATE(CSoundSettingDlg, CDialog)

static CDiskInfoDlg* p;

CSoundSettingDlg::CSoundSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogFx(CDiskInfoDlg::IDD, pParent)
{
	p = (CDiskInfoDlg*)pParent;

	m_ZoomType = p->GetZoomType();
	m_FontScale = p->GetFontScale();
	m_FontRatio = p->GetFontRatio();
	m_FontFace = p->GetFontFace();
	m_CurrentLangPath = p->GetCurrentLangPath();
	m_DefaultLangPath = p->GetDefaultLangPath();
	m_ThemeDir = p->GetThemeDir();
	m_CurrentTheme = p->GetCurrentTheme();
	m_DefaultTheme = p->GetDefaultTheme();
	m_Ini = p->GetIniPath();
}

CSoundSettingDlg::~CSoundSettingDlg()
{
}

void CSoundSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogFx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_FILE_PATH, m_FilePath);

	DDX_Control(pDX, IDC_FILE_PATH, m_CtrlFilePath);
	DDX_Control(pDX, IDC_BUTTON_SELECT_FILE, m_CtrlSelectFile);
	DDX_Control(pDX, IDC_BUTTON_PLAY, m_CtrlPlay);
	DDX_Control(pDX, IDC_BUTTON_DEFAULT, m_CtrlDefault);
	DDX_Control(pDX, IDC_BUTTON_OK, m_CtrlOk);
}

BOOL CSoundSettingDlg::OnInitDialog()
{
	CDialogFx::OnInitDialog();

	SetWindowText(i18n(_T("WindowTitle"), _T("SOUND_SETTINGS")));

	m_bShowWindow = TRUE;

	TCHAR str[256];
	GetPrivateProfileString(_T("Setting"), _T("AlertSoundPath"), _T(""), str, 256, m_Ini);
	m_FilePath = str;
	UpdateData(FALSE);

	m_CtrlDefault.SetWindowTextW(i18n(_T("HealthStatus"), _T("DEFAULT")));

	UpdateDialogSize();

	CenterWindow();
	ShowWindow(SW_SHOW);

	return TRUE;
}

BEGIN_MESSAGE_MAP(CSoundSettingDlg, CDialogFx)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_FILE, &CSoundSettingDlg::OnSelectFile)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, &CSoundSettingDlg::OnPlay)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, &CSoundSettingDlg::OnDefault)
	ON_BN_CLICKED(IDC_BUTTON_OK, &CSoundSettingDlg::OnOk)
END_MESSAGE_MAP()

void CSoundSettingDlg::UpdateDialogSize()
{
	CDialogFx::UpdateDialogSize();

	ChangeZoomType(m_ZoomType);
	SetClientSize(SIZE_X, SIZE_Y, m_ZoomRatio);
	UpdateBackground(FALSE, m_bDarkMode);

	m_CtrlFilePath.SetFontEx(m_FontFace, 12, 12, m_ZoomRatio, m_FontRatio);
	m_CtrlSelectFile.SetFontEx(m_FontFace, 12, 12, m_ZoomRatio, m_FontRatio);
	m_CtrlPlay.SetFontEx(m_FontFace, 12, 12, m_ZoomRatio, m_FontRatio);
	m_CtrlDefault.SetFontEx(m_FontFace, 12, 12, m_ZoomRatio, m_FontRatio);
	m_CtrlOk.SetFontEx(m_FontFace, 12, 12, m_ZoomRatio, m_FontRatio);

	m_CtrlFilePath.InitControl(8, 8, 416, 24, m_ZoomRatio, &m_BkDC, NULL, 0, SS_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode);
	m_CtrlSelectFile.InitControl(428, 8, 24, 24, m_ZoomRatio, &m_BkDC, IP(L"selectSound"), 2, BS_CENTER, OwnerDrawImage, m_bHighContrast, m_bDarkMode);
	m_CtrlSelectFile.SetHandCursor();
	m_CtrlPlay.InitControl(456, 8, 24, 24, m_ZoomRatio, &m_BkDC, IP(L"playSound"), 2, BS_CENTER, OwnerDrawImage, m_bHighContrast, m_bDarkMode);
	m_CtrlPlay.SetHandCursor();
	m_CtrlDefault.InitControl(40, 40, 160, 24, m_ZoomRatio, &m_BkDC, NULL, 0, BS_CENTER, SystemDraw, m_bHighContrast, m_bDarkMode);
	m_CtrlOk.InitControl(280, 40, 160, 24, m_ZoomRatio, &m_BkDC, NULL, 0, BS_CENTER, SystemDraw, m_bHighContrast, m_bDarkMode);

	m_CtrlFilePath.SetDrawFrame(TRUE);

	SetDarkModeControl(m_CtrlOk.GetSafeHwnd(), m_bDarkMode);
	SetDarkModeControl(m_CtrlDefault.GetSafeHwnd(), m_bDarkMode);

	Invalidate();
}

void CSoundSettingDlg::OnSelectFile()
{
	CFileDialog dlg(TRUE, _T(""), _T(""), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,
		i18n(_T("Customize"), _T("SUPPORTED_SOUND_FORMAT"))
		+ _T("|*.wav;*.mp3;*.opus;|WAVE (*.wav)|*.wav|MP3 (*.mp3)|*.mp3|Opus (*.opus)|*.opus|")
		+ i18n(_T("Customize"), _T("ALL_FILES"))
		+ _T(" (*.*)|*.*||"));

	if(dlg.DoModal() == IDOK)
	{
		m_FilePath = dlg.GetPathName();
		WritePrivateProfileString(_T("Setting"), _T("AlertSoundPath"), _T("\"") + m_FilePath + _T("\""), m_Ini);
		m_CtrlFilePath.SetToolTipText(m_FilePath);
		UpdateData(FALSE);
	}
}

void CSoundSettingDlg::OnPlay()
{
	::PostMessage(m_ParentWnd->GetSafeHwnd(), MY_PLAY_ALERT_SOUND, NULL, NULL);
}


void CSoundSettingDlg::OnDefault()
{
	m_FilePath = _T("");
	WritePrivateProfileString(_T("Setting"), _T("AlertSoundPath"), m_FilePath, m_Ini);
	m_CtrlFilePath.SetToolTipText(m_FilePath);
	UpdateData(FALSE);
}

void CSoundSettingDlg::OnOk()
{
	CDialogFx::OnCancel();
}
