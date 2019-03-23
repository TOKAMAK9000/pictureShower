
// PictureShowerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PictureShower.h"
#include "PictureShowerDlg.h"
#include "afxdialogex.h"

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


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPictureShowerDlg 对话框

CPictureShowerDlg::CPictureShowerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPictureShowerDlg::IDD, pParent)
	, m_CurrentPicture(_T(""))
	, m_TransitionMode(_T("12"))
	, m_TransitionDuration(_T("3"))
	, m_StretchMode(_T("1"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	frame = new hvframe();
	frame->pAVFrame = NULL;
}

void CPictureShowerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURE, m_Picture);
	DDX_Text(pDX, IDC_CURRENTPICTURE, m_CurrentPicture);
	DDX_Text(pDX, IDC_MODE, m_TransitionMode);
	DDX_Text(pDX, IDC_EDIT1, m_TransitionDuration);
	DDX_Text(pDX, IDC_EDIT2, m_StretchMode);
}

BEGIN_MESSAGE_MAP(CPictureShowerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_READPICTUREFILE, &CPictureShowerDlg::OnBnClickedReadpicturefile)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_PLAY, &CPictureShowerDlg::OnBnClickedPlay)
	ON_BN_CLICKED(CHECK1, &CPictureShowerDlg::OnBnClickedCheck1)
END_MESSAGE_MAP()


// CPictureShowerDlg 消息处理程序

BOOL CPictureShowerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	InitWindow();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPictureShowerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPictureShowerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPictureShowerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPictureShowerDlg::OnBnClickedReadpicturefile()
{
	// TODO:  在此添加控件通知处理程序代码
	CFileDialog fileDlg(true, _T(""), _T("*.*"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("All File (*.*)|*.*||"), NULL); 
	if (fileDlg.DoModal() == IDOK)    //弹出对话框
	{    
		m_CurrentPicture = fileDlg.GetPathName();//得到完整的文件名和目录名拓展名   
		UpdateData(false);
	}
}


int CPictureShowerDlg::InitWindow()
{
	avdevice_register_all();
	//	avformat_network_init();

	m_D3DShower = new CSpecialEffectD3DShower();
	if (!m_D3DShower) return -1;

	if (m_D3DShower->InitD3D(m_Picture.GetSafeHwnd())<0)
		return -1;
	return 0;
}


bool CPictureShowerDlg::DecodePicture(CString FileName)
{
	AVPacket * pkt;
	AVFrame * pFrame;
	AVFrame * pFrameYUV;
	AVFormatContext * pFormatCtx = NULL;
	AVCodecContext *pCodecCtx;
	SwsContext * img_convert_ctx = NULL;
	AVCodec *pCodec;
	uint8_t * out_buffer;
	int ret;

	//加载图片  
	char szFileName[1024];
	int i;
	for (i = 0; i < FileName.GetLength(); i++)
	{
		szFileName[i] = (int)FileName.GetAt(i);
	}
	szFileName[i] = 0;

	// Open input video file 
	if (avformat_open_input(&pFormatCtx, szFileName, NULL, NULL) != 0)
		return false;

	// Retrieve stream information 
	if (avformat_find_stream_info(pFormatCtx, NULL) < 0)
	{
		avformat_close_input(&pFormatCtx);
		return false;
	}
	//  Find the first video stream 
	int videoStream = -1;
	for (int i = 0; i < (int)pFormatCtx->nb_streams; i++)
	{
		if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			videoStream = i;
			break;
		}
	}

	if (videoStream == -1)
	{
		avformat_close_input(&pFormatCtx);
		return false;
	}
	// Didn't find a video stream
	//  Get a pointer to the codec context for the video stream 

	pCodecCtx = avcodec_alloc_context3(NULL);
	if (!pCodecCtx)
	{
		avformat_close_input(&pFormatCtx);
		return false;
	}

	ret = avcodec_parameters_to_context(pCodecCtx, pFormatCtx->streams[videoStream]->codecpar);
	if (ret < 0 || pCodecCtx->width == 0 || pCodecCtx->height == 0)
	{
		avcodec_free_context(&pCodecCtx);
		avformat_close_input(&pFormatCtx);
		return false;
	}

	pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
	if (pCodec == NULL)
	{
		avcodec_free_context(&pCodecCtx);
		avformat_close_input(&pFormatCtx);
		return false;
	}

	//  Open codec 
	if (avcodec_open2(pCodecCtx, pCodec, NULL)<0)
	{
		avcodec_free_context(&pCodecCtx);
		avformat_close_input(&pFormatCtx);
		return false; // Could not open codec
	}

	pFrame = av_frame_alloc();
	pFrameYUV = av_frame_alloc();

	out_buffer = (unsigned char *)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_RGB32, m_D3DShower->GetWidth(), m_D3DShower->GetHight(), 1));
	av_image_fill_arrays(pFrameYUV->data, pFrameYUV->linesize, out_buffer, AV_PIX_FMT_RGB32, m_D3DShower->GetWidth(), m_D3DShower->GetHight(), 1);
	img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
		m_D3DShower->GetWidth(), m_D3DShower->GetHight(), AV_PIX_FMT_RGB32, SWS_BICUBIC, NULL, NULL, NULL);

	pkt = av_packet_alloc();

	while (true)
	{
		ret = av_read_frame(pFormatCtx, pkt);
		if (ret < 0)
		{
			av_packet_unref(pkt);
			break;
		}
		//  Is this a packet from the video stream? 
		if (pkt->stream_index == videoStream)
		{
			ret = avcodec_send_packet(pCodecCtx, pkt);
			if (ret != 0)
			{
				av_packet_unref(pkt);
				break;
			}

			while (true)
			{
				av_frame_unref(pFrame);
				ret = avcodec_receive_frame(pCodecCtx, pFrame);
				if (ret == 0)
					break;
				av_frame_unref(pFrame);
			}

			img_convert_ctx = sws_getCachedContext(img_convert_ctx,
				pFrame->width, pFrame->height, (AVPixelFormat)(pFrame->format),
				m_D3DShower->GetWidth(), m_D3DShower->GetHight(),
				AV_PIX_FMT_RGB32, SWS_BICUBIC, NULL, NULL, NULL);

			if (img_convert_ctx == NULL)
			{
				av_frame_unref(pFrame);
				av_frame_free(&pFrame);
				avcodec_free_context(&pCodecCtx);
				avformat_close_input(&pFormatCtx);
				av_packet_free(&pkt);
				return false;
			}

			sws_scale(img_convert_ctx, pFrame->data, pFrame->linesize,
				0, pFrame->height, pFrameYUV->data, pFrameYUV->linesize);

			frame->mScreen_Height = m_D3DShower->GetHight();
			frame->mScreen_Width = m_D3DShower->GetWidth();
			frame->mSrcData = pFrameYUV->data[0];
			frame->mLineSize = pFrameYUV->linesize[0];
			frame->pAVFrame = pFrameYUV;

			if (!frame->mImg_Width) frame->mImg_Width = pFrame->width;
			if (!frame->mImg_Height) frame->mImg_Height = pFrame->height;
			if (!frame->mImg_Width) frame->mImg_Width = pCodecCtx->width;
			if (!frame->mImg_Height) frame->mImg_Height = pCodecCtx->height;
		}
		av_packet_unref(pkt);
	}

	avcodec_flush_buffers(pCodecCtx);

	av_packet_unref(pkt);
	av_packet_free(&pkt);

	av_frame_unref(pFrame);
	av_frame_free(&pFrame);

	sws_freeContext(img_convert_ctx);

	avcodec_free_context(&pCodecCtx);
	/* Close the video file */
	avformat_close_input(&pFormatCtx);

	return true;
}


void CPictureShowerDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO:  在此处添加消息处理程序代码
	DeletePicture();
}


void CPictureShowerDlg::DrawPicture()
{
	m_D3DShower->Render(frame);
}


void CPictureShowerDlg::OnBnClickedPlay()
{
	// TODO:  在此添加控件通知处理程序代码

	UpdateData(true);

	DeletePicture();

	if (m_CurrentPicture != "")
	{
		DecodePicture(m_CurrentPicture);
		m_D3DShower->m_DisplayMode = _ttoi(m_StretchMode);
		m_D3DShower->m_TransitionMode = _ttoi(m_TransitionMode);
		m_D3DShower->m_TransitionDuration = _ttoi(m_TransitionDuration);
		DrawPicture();
	}
}


void CPictureShowerDlg::DeletePicture()
{
	AVFrame * pAV = NULL;

	if (frame)
	{
		pAV = frame->pAVFrame;
		if (pAV)
		{
			av_frame_unref(pAV);
			av_frame_free(&pAV);
		}
		delete pAV;
		pAV = NULL;
	}
}

void CPictureShowerDlg::OnBnClickedCheck1()
{
	if (BST_CHECKED == IsDlgButtonChecked(CHECK1))
	{
		m_D3DShower->initPictureEveryTime = true;
	}
	else
	{
		m_D3DShower->initPictureEveryTime = false;
	}
}



