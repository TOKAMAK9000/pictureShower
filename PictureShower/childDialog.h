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
}// endof extern "C"
#endif


// childDialog �Ի���

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
	CString m_CurrentPicture;
	CString m_TransitionMode;
	CString m_TransitionDuration;
	CString m_StretchMode;




	HANDLE hThread;
	DWORD ThreadID;
	static UINT ThreadFunc(LPVOID pParam); //�̺߳������������ڶ�����ʹ��static�������Σ�����static�������������ⶨ�塣
	void  process(); //��ĳ�Ա������process()��������Ҫ�õ����������Ա����
	void m_showWindow();
};