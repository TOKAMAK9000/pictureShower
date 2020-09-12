#pragma once
#include "afxwin.h"
#include "SpecialEffectD3DShower.h"
#include "PictureShowerDlg.h"
#include "PictureShower.h"
#include "voice.h"
#include "public.h"


#ifdef __cplusplus
extern "C" {
#endif
#include "libavcodec/avcodec.h"
#include "libavutil/imgutils.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#ifdef __cplusplus
}// endof extern "C" 
#endif


class childDialog : public CDialogEx
{
	DECLARE_DYNAMIC(childDialog)

public:
	hvframe * frame = NULL;
	CSpecialEffectD3DShower * m_D3DShower = NULL;

public:
	childDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~childDialog();

	enum { IDD = IDD_DIALOG1 };// �Ի�������

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString CHCurrentPicture;
	CString CHTransitionMode;
	CString CHTransitionDuration;
	CString CHStretchMode;
	CString CHtempText;
	HANDLE CHhThread;
	DWORD CHThreadID;
	static UINT CHThreadFunc(LPVOID pParam);
	void  CHprocess();

private:

	CWnd *mParentWnd;
public:
	CEdit CHDuration;
	afx_msg void OnBnClickedChs();
	afx_msg void OnBnClickedChclear();
	afx_msg void OnEnChangeChduration();
	afx_msg void OnEnChangeChmode();
	afx_msg void OnBnClickedChreadpic();
	afx_msg void OnBnClickedChquit();
	afx_msg void OnBnClickedChplay();

	BOOL OnInitDialog();

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnStnClickedChstatic5();
	CListCtrl mList;
	afx_msg void OnBnClickedChgetvoice();
};