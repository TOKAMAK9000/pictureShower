#pragma once
#include "afxwin.h"
#include "SpecialEffectD3DShower.h"
#include "PictureShower.h"
#include "childDialog.h"




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

//extern BOOL initPictureEveryTime;

// CPictureShowerDlg 
class CPictureShowerDlg : public CDialogEx
{
public:
	//childDialog * cdlg = NULL;//为什么不能放在这个文件中
	hvframe * frame = NULL;
	CSpecialEffectD3DShower * m_D3DShower = NULL;

public:
	CPictureShowerDlg(CWnd* pParent = NULL);
	~CPictureShowerDlg();

	enum { IDD = IDD_PICTURESHOWER_DIALOG };

	protected:
	//MFC�����ڱ����Ϳؼ���������ݺͿ������ݵĺ�
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	CRect mainWindowRect;

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnCHDlg(WPARAM wParam, LPARAM iParam);
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_Picture;
	CString m_CurrentPicture;
	CString m_TransitionMode;
	afx_msg void OnBnClickedReadpicturefile();
	int InitWindow();
	bool DecodePicture(CString FileName);
	afx_msg void OnDestroy();
	void DrawPicture();
	void DeletePicture();
	CString m_TransitionDuration;
	CString m_StretchMode;

	//new
	bool initPictureEveryTime;
	WINDOWPLACEMENT m_struOldWndpl;
	WINDOWPLACEMENT m_struOldWndpPic;
	CRect rectFullScreen;
	BOOL bFullScreen;

	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedPlay();
	afx_msg void OnClose();
	afx_msg void ONCloseDlg();
	void OnSize(UINT nType, int cx, int cy);
	void ChangeSize(CWnd * pWnd, int cx, int cy);
};
