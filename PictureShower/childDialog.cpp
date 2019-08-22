// childDialog.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "PictureShower.h"
#include "PictureShowerDlg.h"
#include "afxdialogex.h"
#include "childDialog.h"

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


// childDialog �Ի���

IMPLEMENT_DYNAMIC(childDialog, CDialogEx)

childDialog::childDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(childDialog::IDD, pParent)
{

}

childDialog::~childDialog()
{
}

void childDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CURRENTPICTURE, m_CurrentPicture);
	DDX_Text(pDX, IDC_MODE, m_TransitionMode);
	DDX_Text(pDX, IDC_EDIT1, m_TransitionDuration);
	DDX_Text(pDX, IDC_EDIT2, m_StretchMode);
}

UINT childDialog::ThreadFunc(LPVOID pParam)
{
	childDialog *dlg = (childDialog*)pParam;
	dlg->process();
	return 0;
}

void childDialog::process()
{
	
}

void childDialog::m_showWindow(){
	CWinThread *pThread = (CWinThread*)AfxBeginThread(ThreadFunc, this);
	ShowWindow(SW_SHOW);
}


BEGIN_MESSAGE_MAP(childDialog, CDialogEx)
END_MESSAGE_MAP()




