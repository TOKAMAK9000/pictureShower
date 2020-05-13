// childDialog.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "PictureShower.h"
#include "afxdialogex.h"
#include "childDialog.h"
#include "public.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef __cplusplus
extern "C" {
#endif
#include "libswscale/swscale.h"
#include "libavdevice/avdevice.h"
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavutil/frame.h"

#ifdef __cplusplus
} // endof extern "C"
#endif

option mOption;


// childDialog �Ի���

IMPLEMENT_DYNAMIC(childDialog, CDialogEx)

childDialog::childDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(childDialog::IDD, pParent)
	, CHCurrentPicture(_T(""))
	, CHTransitionMode(_T("12"))
	, CHTransitionDuration(_T("3"))
{
	//LONG style = GetWindowLong(m_hWnd, GWL_STYLE); //得到窗口类型
	//style |= WS_CAPTION; //修改窗口类型，显示标题栏
	//SetWindowLong(m_hWnd, GWL_STYLE, style);
}

childDialog::~childDialog()
{
}

void childDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Text(pDX, IDC_CURRENTPICTURE, m_CurrentPicture);
	DDX_Text(pDX, IDC_CHDURATION, CHTransitionDuration);
	DDX_Text(pDX, IDC_CHTRANSMODE1, CHTransitionMode);
	DDX_Text(pDX, IDC_CHCURRENTPICTURE, mOption.PfilePath);
}

UINT childDialog::CHThreadFunc(LPVOID pParam)
{
	childDialog *chdlg = (childDialog*)pParam;
	chdlg->CHprocess();
	return 0;
}

void childDialog::CHprocess()
{
	
}

//void childDialog::m_showWindow(){
//	CWinThread *pThread = (CWinThread*)AfxBeginThread(ThreadFunc, this);
//	ShowWindow(SW_SHOW);
//}


BEGIN_MESSAGE_MAP(childDialog, CDialogEx)
	ON_BN_CLICKED(IDC_CHS, &childDialog::OnBnClickedChs)
	ON_BN_CLICKED(IDC_CHCLEAR, &childDialog::OnBnClickedChclear)
	ON_EN_CHANGE(IDC_CHDURATION, &childDialog::OnEnChangeChduration)
	ON_EN_CHANGE(IDC_CHTRANSMODE1, &childDialog::OnEnChangeChmode)
	ON_BN_CLICKED(IDC_CHREADPIC, &childDialog::OnBnClickedChreadpic)
	ON_BN_CLICKED(IDC_CHQUIT, &childDialog::OnBnClickedChquit)
	ON_BN_CLICKED(IDC_CHPLAY, &childDialog::OnBnClickedChplay)
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()





void childDialog::OnBnClickedChs()
{
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHS))
	{
		mOption.PStretch = _T("1");
	}
	else
	{
		mOption.PStretch = _T("0");
	}
}


void childDialog::OnBnClickedChclear()
{
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHCLEAR))
	{
		mOption.PclearPicture = true;
	}
	else
	{
		mOption.PclearPicture = false;
	}
}


void childDialog::OnEnChangeChduration()
{
	UpdateData(true);
	mOption.PDuration = CHTransitionDuration;
}


void childDialog::OnEnChangeChmode()
{
	UpdateData(true);
	mOption.PTransitionMode = CHTransitionMode;
}


void childDialog::OnBnClickedChreadpic()
{
	CFileDialog fileDlg(true, _T(""), _T("*.*"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("All File (*.*)|*.*||"), NULL);
	if (fileDlg.DoModal() == IDOK)
	{
		mOption.PfilePath = fileDlg.GetPathName();
		UpdateData(false);
	}
}


void childDialog::OnBnClickedChquit()
{
	mParentWnd = GetParent();
	::SendMessage(mParentWnd->m_hWnd, WM_CLOSE, 0, 0);
	CDialogEx::OnCancel();
}


void childDialog::OnBnClickedChplay()
{
	HWND hWnd = AfxGetApp()->GetMainWnd()->GetSafeHwnd();
	::SendMessage(hWnd, WM_COMMAND, MAKELONG(IDC_PLAY, BN_CLICKED),(LPARAM)(GetDlgItem(IDC_PLAY)->GetSafeHwnd()));

}



BOOL childDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//this->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void childDialog::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID) == IDC_AFXBARRES_MINIMIZE)
	{
		this->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_HIDEWINDOW);
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}
