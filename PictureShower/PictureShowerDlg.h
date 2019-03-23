
// PictureShowerDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "SpecialEffectD3DShower.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "libavcodec/avcodec.h"
#include "libavutil/imgutils.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#ifdef __cplusplus
} // endof extern "C"
#endif

//extern BOOL initPictureEveryTime;

// CPictureShowerDlg 对话框
class CPictureShowerDlg : public CDialogEx
{
public:
	hvframe * frame = NULL;
	CSpecialEffectD3DShower * m_D3DShower = NULL;
// 构造
public:
	CPictureShowerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PICTURESHOWER_DIALOG };

	protected:
	//MFC中用于变量和控件间关联数据和控制数据的宏
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
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
	afx_msg void OnBnClickedPlay();
	void DeletePicture();
	CString m_TransitionDuration;
	CString m_StretchMode;

	//new
	afx_msg void OnBnClickedCheck1();
	bool initPictureEveryTime;
};
