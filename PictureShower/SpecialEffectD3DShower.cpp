#include "StdAfx.h"
#include "SpecialEffectD3DShower.h"
#include "atltime.h"

CSpecialEffectD3DShower::CSpecialEffectD3DShower(void)
{
}


CSpecialEffectD3DShower::~CSpecialEffectD3DShower(void)
{
}


bool CSpecialEffectD3DShower::Render(hvframe * frame)
{
	if (!ConstructSurface(frame))
		return false;

	//	m_TransitionMode = 3;

	switch (this->m_TransitionMode)
	{
	default:
	case 0:
		ShowSurfaceNormal(frame);
		break;
	case 1:
		ShowSurfaceScan(0, frame);
		break;
	case 2:
		ShowSurfaceScan(1, frame);
		break;
	case 3:
		ShowSurfaceScan(2, frame);
		break;
	case 4:
		ShowSurfaceScan(3, frame);
		break;
	case 5:
		ShowSurfaceSlide(0, frame);
		break;
	case 6:
		ShowSurfaceSlide(1, frame);
		break;
	case 7:
		ShowSurfaceSlide(2, frame);
		break;
	case 8:
		ShowSurfaceSlide(3, frame);
		break;
	case 9:
		ShowSurfaceButt(0, frame);
		break;
	case 10:
		ShowSurfaceButt(1, frame);
		break;
	case 11:
		ShowSurfaceCenterFull(frame);
		break;
	case 12:
		ShowSurfaceEnlarge(frame);
		break;
	case 13:
		ShowSurfaceDiagonalFull(0, frame);
		break;
	case 14:
		ShowSurfaceDiagonalFull(1, frame);
		break;
	case 15:
		ShowSurfaceDiagonalFull(2, frame);
		break;
	case 16:
		ShowSurfaceDiagonalFull(3, frame);
		break;
	case 17:
		ShowSurfaceDiagonalEnlarge(0, frame);
		break;
	case 18:
		ShowSurfaceDiagonalEnlarge(1, frame);
		break;
	case 19:
		ShowSurfaceDiagonalEnlarge(2, frame);
		break;
	case 20:
		ShowSurfaceDiagonalEnlarge(3, frame);
		break;
	case 21:
		ShowSurfaceRandBlock(frame);
		break;
	case 22:
		ShowSurfacePullCurtain(frame);
		break;
	case 23:
		ShowSurfaceVerticalBlind(0, frame);
		break;
	case 24:
		ShowSurfaceVerticalBlind(1, frame);
		break;
	case 25:
		testTextureShower(0,frame);
		break;
	case 26:
		testTextureShower(1, frame);
		break;
	case 27:
		testTextureShower(2, frame);
		break;
	}

	RECT rcCurrentClient;
	::GetClientRect(d3dpp.hDeviceWindow, &rcCurrentClient);
	if ((UINT)rcCurrentClient.right != d3dpp.BackBufferWidth ||
		(UINT)rcCurrentClient.bottom != d3dpp.BackBufferHeight)
	{
		d3dpp.BackBufferWidth = rcCurrentClient.right;
		d3dpp.BackBufferHeight = rcCurrentClient.bottom;
		InitD3D(mHand);
	}

	if (m_pDirect3DDevice && m_pDirect3DDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
	{
		if (FAILED(m_pDirect3DDevice->Reset(&d3dpp)))
		{
			InitD3D(mHand);
		}
	}

	return true;
}


bool CSpecialEffectD3DShower::ConstructSurface(hvframe * frame)
{
	//HRESULT��A 32-bit value that is used to describe an error or warning.
	HRESULT lRet;
	if (m_pDirect3DSurfaceRender == NULL)
		return false;

	//D3DLOCKED_RECT���ýṹ�����һ��ָ����ε�ָ���һ����ʾ���г��ȵ�intֵPitch
	D3DLOCKED_RECT d3d_rect;
	lRet = m_pDirect3DSurfaceRender->LockRect(&d3d_rect, NULL, D3DLOCK_DONOTWAIT);
	if (FAILED(lRet))
		return false;

	byte * pSrc = frame->mSrcData;
	byte * pDest = (byte *)d3d_rect.pBits;
	int lPitch = d3d_rect.Pitch;
	int i = 0;

	for (i = 0; i < frame->mScreen_Height; i++)
	{
		memcpy(pDest, pSrc, frame->mLineSize);
		pDest += lPitch;
		pSrc += frame->mLineSize;
	}

	lRet = m_pDirect3DSurfaceRender->UnlockRect();
	if (FAILED(lRet))
		return false;

	if (m_pDirect3DDevice == NULL)
		return false;

	return true;
}


//��ʾͼ��Ļ�������
void CSpecialEffectD3DShower::ShowSurfaceNormal(hvframe * frame)
{
	CRect m_rt;
	//GetClientRect(mHand, &m_rtViewport);

	//�߳�
	EnterCriticalSection(&m_critial);

	if (this->initPictureEveryTime) {
		testInit();
		if (m_DisplayMode)
			calculate_display_rect(&m_rt, frame->mImg_Width, frame->mImg_Height, m_rtViewport.Width(), m_rtViewport.Height());
		else
			m_rt = m_rtViewport;
	}

	m_pDirect3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	m_pDirect3DDevice->BeginScene();

	IDirect3DSurface9 * pBackBuffer = NULL;



	//��������������������������������������������������������
	//��m_rt��ֵ
	if (m_DisplayMode)
		calculate_display_rect(&m_rt, frame->mImg_Width, frame->mImg_Height, m_rtViewport.Width(), m_rtViewport.Height());
	else
		m_rt = m_rtViewport;

	m_pDirect3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);

	//��Դ���ε����ݸ��Ƶ�Ŀ����Ρ� Դ���ο���ͨ��������������͹��ˡ� 
	//�˹���ͨ�����ڸ�����Ƶ���Ŀ�߱ȡ�
	//������Դsurface��Դ���Σ�Ŀ��surface�;��Σ����������͡�
	m_pDirect3DDevice->StretchRect(m_pDirect3DSurfaceRender, NULL, pBackBuffer, &m_rt, D3DTEXF_LINEAR);
	m_pDirect3DDevice->EndScene();
	m_pDirect3DDevice->Present(NULL, NULL, NULL, NULL);

	LeaveCriticalSection(&m_critial);
}


void CSpecialEffectD3DShower::testInit()
{
	m_pDirect3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 0, 0);
	m_pDirect3DDevice->BeginScene();
	m_pDirect3DDevice->EndScene();
	m_pDirect3DDevice->Present(NULL, NULL, NULL, NULL);
}


//ͼ��ƽ�Ƶ�ʵ�ֺ���
//0: left to right; 1: right to left; 2:top to bottom; 3:bottom to top
// to be checked
//���ܴ��ڵ����⣿
//�ں����ж���ı���hδʹ�ã��ʶԳ��������΢����΢��֮ǰ�İ汾���·�ע���С�
//update��ע����ɾ��

void CSpecialEffectD3DShower::ShowSurfaceScan(int direction, hvframe * frame)
{
	int length_w,length_h;
	int steps, i;
	double w, h;
	int trantime;
	CRect pr;
	CRect m_rt;

	if (m_DisplayMode)
		calculate_display_rect(&m_rt, frame->mImg_Width, frame->mImg_Height, m_rtViewport.Width(), m_rtViewport.Height());
	else
		m_rt = m_rtViewport;

	pr = m_rt;


	steps = m_TransitionDuration *  mPicturesPerSecond;
	w = (double)(pr.Width()) / steps;
	h = (double)(pr.Height()) / steps;

	trantime = 1000 / mPicturesPerSecond;

	EnterCriticalSection(&m_critial);

	if (this->initPictureEveryTime) {
		testInit();
		if (m_DisplayMode)
			calculate_display_rect(&m_rt, frame->mImg_Width, frame->mImg_Height, m_rtViewport.Width(), m_rtViewport.Height());
		else
			m_rt = m_rtViewport;
	}

	m_pDirect3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	m_pDirect3DDevice->BeginScene();
	IDirect3DSurface9 * pBackBuffer = NULL;
	m_pDirect3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
	m_pDirect3DDevice->StretchRect(m_pDirect3DSurfaceRender, NULL, pBackBuffer, &m_rt, D3DTEXF_LINEAR);
	m_pDirect3DDevice->EndScene();

	for (i = 1; i < steps; i++)
	{
		length_w = (int)(w*i);
		length_h = (int)(h*i);
		switch (direction)
		{
		default:
		case 0:
			pr.right = pr.left + length_w;
			break;
		case 1:
			pr.left = pr.right - length_w;
			break;
		case 2:
			pr.bottom = pr.top + length_h;
			break;
		case 3:
			pr.top = pr.bottom - length_h;
			break;
		}
		if ((length_w>0)|(length_h>0))
			m_pDirect3DDevice->Present(&pr, &pr, NULL, NULL);

		Sleep(trantime);
	}
	m_pDirect3DDevice->Present(&m_rt, &m_rt, NULL, NULL);
	LeaveCriticalSection(&m_critial);
}

//0: left to right; 1: right to left; 2:top to bottom; 3:bottom to top
// to be checked
void CSpecialEffectD3DShower::ShowSurfaceSlide(int direction, hvframe * frame)
{
	int length;
	int steps, i;
	double w, h;
	CRect pr, sr;
	int trantime;

	CRect m_rt;

	if (m_DisplayMode)
		calculate_display_rect(&m_rt, frame->mImg_Width, frame->mImg_Height, m_rtViewport.Width(), m_rtViewport.Height());
	else
		m_rt = m_rtViewport;

	steps = m_TransitionDuration *  mPicturesPerSecond;
	w = (double)(m_rt.Width()) / steps;
	h = (double)(m_rt.Height()) / steps;

	trantime = 1000 / mPicturesPerSecond;

	EnterCriticalSection(&m_critial);

	
	if (this->initPictureEveryTime){
		testInit();
		if (m_DisplayMode)
			calculate_display_rect(&m_rt, frame->mImg_Width, frame->mImg_Height, m_rtViewport.Width(), m_rtViewport.Height());
		else
			m_rt = m_rtViewport;
	}

	switch (direction)
	{
	default:
	case 0:
		pr.right = m_rt.right;
		pr.top = m_rt.top;
		pr.bottom = m_rt.bottom;

		sr.top = m_rt.top;
		sr.bottom = m_rt.bottom;
		sr.left = m_rt.left;

		for (i = 0; i<steps; i++)
		{
			length = (int)(i*w);
			if (i == (steps - 1))
			{
				pr.left = m_rt.left;
				sr.right = m_rt.right;
			}
			else
			{
				pr.left = m_rt.right - length;
				sr.right = m_rt.left + length;
			}

			if (length > 0)
			{
				m_pDirect3DDevice->BeginScene();
				IDirect3DSurface9 * pBackBuffer = NULL;
				m_pDirect3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
				m_pDirect3DDevice->StretchRect(m_pDirect3DSurfaceRender, &pr, pBackBuffer, &sr, D3DTEXF_LINEAR);
				m_pDirect3DDevice->EndScene();
				m_pDirect3DDevice->Present(&sr, &sr, NULL, NULL);
			}

			Sleep(trantime);
		}
		break;
	case 1:
		pr.left = m_rt.left;
		pr.top = m_rt.top;
		pr.bottom = m_rt.bottom;

		sr.top = m_rt.top;
		sr.bottom = m_rt.bottom;
		sr.right = m_rt.right;

		for (i = 0; i<steps; i++)
		{
			length = (int)(i*w);
			if (i == (steps - 1))
			{
				pr.right = m_rt.right;
				sr.left = m_rt.left;
			}
			else
			{
				pr.right = m_rt.left + length;
				sr.left = m_rt.right - length;
			}

			if (length > 0)
			{
				m_pDirect3DDevice->BeginScene();
				IDirect3DSurface9 * pBackBuffer = NULL;
				m_pDirect3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
				m_pDirect3DDevice->StretchRect(m_pDirect3DSurfaceRender, &pr, pBackBuffer, &sr, D3DTEXF_LINEAR);
				m_pDirect3DDevice->EndScene();
				m_pDirect3DDevice->Present(&sr, &sr, NULL, NULL);
			}

			Sleep(trantime);
		}
		break;
	case 2:
		pr.bottom = m_rt.bottom;
		pr.left = m_rt.left;
		pr.right = m_rt.right;

		sr.left = m_rt.left;
		sr.right = m_rt.right;
		sr.top = m_rt.top;

		for (i = 0; i<steps; i++)
		{
			length = (int)(i*h);
			if (i == (steps - 1))
			{
				pr.top = m_rt.top;
				sr.bottom = m_rt.bottom;
			}
			else
			{
				pr.top = m_rt.bottom - length;
				sr.bottom = m_rt.top + length;
			}

			if (length > 0)
			{
				m_pDirect3DDevice->BeginScene();
				IDirect3DSurface9 * pBackBuffer = NULL;
				m_pDirect3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
				m_pDirect3DDevice->StretchRect(m_pDirect3DSurfaceRender, &pr, pBackBuffer, &sr, D3DTEXF_LINEAR);
				m_pDirect3DDevice->EndScene();
				m_pDirect3DDevice->Present(&sr, &sr, NULL, NULL);
			}
			Sleep(trantime);
		}
		break;
	case 3:
		pr.top = m_rt.top;
		pr.left = m_rt.left;
		pr.right = m_rt.right;

		sr.left = m_rt.left;
		sr.right = m_rt.right;
		sr.bottom = m_rt.bottom;

		for (i = 0; i<steps; i++)
		{
			length = (int)(i*h);
			if (i == (steps - 1))
			{
				pr.bottom = m_rt.bottom;
				sr.top = m_rt.top;
			}
			else
			{
				pr.bottom = m_rt.top + length;
				sr.top = m_rt.bottom - length;
			}

			if (length > 0)
			{
				m_pDirect3DDevice->BeginScene();
				IDirect3DSurface9 * pBackBuffer = NULL;
				m_pDirect3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
				m_pDirect3DDevice->StretchRect(m_pDirect3DSurfaceRender, &pr, pBackBuffer, &sr, D3DTEXF_LINEAR);
				m_pDirect3DDevice->EndScene();
				m_pDirect3DDevice->Present(&sr, &sr, NULL, NULL);
			}
			Sleep(trantime);
		}
		break;
	}
	m_pDirect3DDevice->Present(NULL, NULL, NULL, NULL);
	LeaveCriticalSection(&m_critial);
}


// 0: for vertical; 1: for horical
// to be checked
//ʵ��ͼ�������/��������ʾ��Ч��
//ʵ�ַ���������ShowSurfaceSlide
void CSpecialEffectD3DShower::ShowSurfaceButt(int direction, hvframe * frame)
{
	int length;
	int i, steps;
	double w, h;
	CRect pr, sr;
	int trantime;

	CRect m_rt;

	if (m_DisplayMode)
		calculate_display_rect(&m_rt, frame->mImg_Width, frame->mImg_Height, m_rtViewport.Width(), m_rtViewport.Height());
	else
		m_rt = m_rtViewport;

	pr = m_rt;
	sr = m_rt;

	steps = m_TransitionDuration *  mPicturesPerSecond;
	w = (double)(m_rt.Width()) / steps;
	h = (double)(m_rt.Height()) / steps;

	trantime = 1000 / mPicturesPerSecond;

	EnterCriticalSection(&m_critial);

	if (this->initPictureEveryTime) {
		testInit();
		if (m_DisplayMode)
			calculate_display_rect(&m_rt, frame->mImg_Width, frame->mImg_Height, m_rtViewport.Width(), m_rtViewport.Height());
		else
			m_rt = m_rtViewport;
	}

	m_pDirect3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	m_pDirect3DDevice->BeginScene();
	IDirect3DSurface9 * pBackBuffer = NULL;
	m_pDirect3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
	m_pDirect3DDevice->StretchRect(m_pDirect3DSurfaceRender, NULL, pBackBuffer, &m_rt, D3DTEXF_LINEAR);
	m_pDirect3DDevice->EndScene();

	for (i = 0; i<steps; i++)
	{
		length = (int)(i*w / 2);
		switch (direction)
		{
		default:
		case 0:
			pr.right = pr.left + length;
			sr.left = sr.right - length;
			break;
		case 1:
			pr.bottom = pr.top + length;
			sr.top = sr.bottom - length;
			break;
		}

		if (length>0)
		{
			m_pDirect3DDevice->Present(&pr, &pr, NULL, NULL);
			m_pDirect3DDevice->Present(&sr, &sr, NULL, NULL);
		}
		Sleep(trantime);
	}
	m_pDirect3DDevice->Present(&m_rt, &m_rt, NULL, NULL);
	LeaveCriticalSection(&m_critial);
}


//ͼ���������������չ��
void CSpecialEffectD3DShower::ShowSurfaceCenterFull(hvframe * frame)
{
	int steps;
	double ws, hs;
	int x, y, w, h;
	int offx, offy;
	CRect pr;
	int i;
	int trantime;

	CRect m_rt;

	if (m_DisplayMode)
		calculate_display_rect(&m_rt, frame->mImg_Width, frame->mImg_Height, m_rtViewport.Width(), m_rtViewport.Height());
	else
		m_rt = m_rtViewport;

	w = m_rt.Width() / 2;
	h = m_rt.Height() / 2;

	x = m_rt.left + w;
	y = m_rt.top + h;

	steps = m_TransitionDuration *  mPicturesPerSecond;
	ws = (double)w / steps;
	hs = (double)h / steps;

	trantime = 1000 / mPicturesPerSecond;

	EnterCriticalSection(&m_critial);

	if (this->initPictureEveryTime) {
		testInit();
		if (m_DisplayMode)
			calculate_display_rect(&m_rt, frame->mImg_Width, frame->mImg_Height, m_rtViewport.Width(), m_rtViewport.Height());
		else
			m_rt = m_rtViewport;
	}

	m_pDirect3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	m_pDirect3DDevice->BeginScene();
	IDirect3DSurface9 * pBackBuffer = NULL;
	m_pDirect3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
	m_pDirect3DDevice->StretchRect(m_pDirect3DSurfaceRender, NULL, pBackBuffer, &m_rt, D3DTEXF_LINEAR);
	m_pDirect3DDevice->EndScene();

	for (i = 0; i<steps; i++)
	{
		offy = (int)(hs*i);
		offx = (int)(ws*i);

		pr.bottom = y + offy;
		pr.top = y - offy;
		pr.left = x - offx;
		pr.right = x + offx;

		if (offy>0 && offx>0)
			m_pDirect3DDevice->Present(&pr, &pr, NULL, NULL);

		Sleep(trantime);
	}
	m_pDirect3DDevice->Present(&m_rt, &m_rt, NULL, NULL);
	LeaveCriticalSection(&m_critial);
}


//ͼ��������������ܷŴ�
void CSpecialEffectD3DShower::ShowSurfaceEnlarge(hvframe * frame)
{
	int steps;
	double ws, hs;
	int x, y, w, h;
	int offx, offy;
	CRect pr;
	int i;
	int trantime;

	CRect m_rt;

	if (m_DisplayMode)
		calculate_display_rect(&m_rt, frame->mImg_Width, frame->mImg_Height, m_rtViewport.Width(), m_rtViewport.Height());
	else
		m_rt = m_rtViewport;

	w = m_rt.Width() / 2;
	h = m_rt.Height() / 2;

	x = m_rt.left + w;
	y = m_rt.top + h;

	steps = m_TransitionDuration *  mPicturesPerSecond;
	ws = (double)w / steps;
	hs = (double)h / steps;

	trantime = 1000 / mPicturesPerSecond;

	EnterCriticalSection(&m_critial);

	if (this->initPictureEveryTime) {
		testInit();
		if (m_DisplayMode)
			calculate_display_rect(&m_rt, frame->mImg_Width, frame->mImg_Height, m_rtViewport.Width(), m_rtViewport.Height());
		else
			m_rt = m_rtViewport;
	}

	m_pDirect3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	for (i = 1; i<steps; i++)
	{
		offy = (int)(hs*i);
		offx = (int)(ws*i);

		pr.bottom = y + offy;
		pr.top = y - offy;
		pr.left = x - offx;
		pr.right = x + offx;

		if (offy > 0 && offx > 0)
		{
			m_pDirect3DDevice->BeginScene();
			IDirect3DSurface9 * pBackBuffer = NULL;
			m_pDirect3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
			m_pDirect3DDevice->StretchRect(m_pDirect3DSurfaceRender, NULL, pBackBuffer, &pr, D3DTEXF_LINEAR);
			m_pDirect3DDevice->EndScene();
			m_pDirect3DDevice->Present(&pr, &pr, NULL, NULL);
		}
		Sleep(trantime);
	}
	m_pDirect3DDevice->Present(&m_rt, &m_rt, NULL, NULL);
	LeaveCriticalSection(&m_critial);
}

//�ӽ��䵽ȫ������չʾȫͼ��
void CSpecialEffectD3DShower::ShowSurfaceDiagonalFull(int direction, hvframe * frame)
{
	int steps;
	double w, h;
	int offx, offy;
	CRect pr;
	int i;
	int trantime;

	CRect m_rt;

	if (m_DisplayMode)
		calculate_display_rect(&m_rt, frame->mImg_Width, frame->mImg_Height, m_rtViewport.Width(), m_rtViewport.Height());
	else
		m_rt = m_rtViewport;

	steps = m_TransitionDuration *  mPicturesPerSecond;
	w = (double)(m_rt.Width()) / steps;
	h = (double)(m_rt.Height()) / steps;

	trantime = 1000 / mPicturesPerSecond;

	EnterCriticalSection(&m_critial);

	if (this->initPictureEveryTime) {
		testInit();
		if (m_DisplayMode)
			calculate_display_rect(&m_rt, frame->mImg_Width, frame->mImg_Height, m_rtViewport.Width(), m_rtViewport.Height());
		else
			m_rt = m_rtViewport;
	}

	m_pDirect3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	m_pDirect3DDevice->BeginScene();
	IDirect3DSurface9 * pBackBuffer = NULL;
	m_pDirect3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
	m_pDirect3DDevice->StretchRect(m_pDirect3DSurfaceRender, NULL, pBackBuffer, &m_rt, D3DTEXF_LINEAR);
	m_pDirect3DDevice->EndScene();

	switch (direction)
	{
	default:
	case 0:
		pr.left = m_rt.left;
		pr.top = m_rt.top;

		for (i = 0; i<steps; i++)
		{
			offy = (int)(h*i);
			offx = (int)(w*i);

			pr.right = pr.left + offx;
			pr.bottom = pr.top + offy;

			if (offy > 0 && offx > 0)
				m_pDirect3DDevice->Present(&pr, &pr, NULL, NULL);

			Sleep(trantime);
		}
		break;
	case 1:
		pr.left = m_rt.left;
		pr.bottom = m_rt.bottom;

		for (i = 0; i<steps; i++)
		{
			offy = (int)(h*i);
			offx = (int)(w*i);

			pr.right = pr.left + offx;
			pr.top = pr.bottom - offy;

			if (offy > 0 && offx > 0)
				m_pDirect3DDevice->Present(&pr, &pr, NULL, NULL);

			Sleep(trantime);
		}
		break;
	case 2:
		pr.right = m_rt.right;
		pr.bottom = m_rt.bottom;

		for (i = 0; i<steps; i++)
		{
			offy = (int)(h*i);
			offx = (int)(w*i);

			pr.left = pr.right - offx;
			pr.top = pr.bottom - offy;

			if (offy > 0 && offx > 0)
				m_pDirect3DDevice->Present(&pr, &pr, NULL, NULL);

			Sleep(trantime);
		}
		break;
	case 3:
		pr.right = m_rt.right;
		pr.top = m_rt.top;

		for (i = 0; i<steps; i++)
		{
			offy = (int)(h*i);
			offx = (int)(w*i);

			pr.left = pr.right - offx;
			pr.bottom = pr.top + offy;

			if (offy > 0 && offx > 0)
				m_pDirect3DDevice->Present(&pr, &pr, NULL, NULL);

			Sleep(trantime);
		}
		break;
	}
	m_pDirect3DDevice->Present(&m_rt, &m_rt, NULL, NULL);
	LeaveCriticalSection(&m_critial);
}

//�ӽ��䵽ȫ����չʾȫͼ��
void CSpecialEffectD3DShower::ShowSurfaceDiagonalEnlarge(int direction, hvframe * frame)
{
	int steps;
	double w, h;
	int offx, offy;
	CRect pr;
	int i;
	int trantime;

	CRect m_rt;

	if (m_DisplayMode)
		calculate_display_rect(&m_rt, frame->mImg_Width, frame->mImg_Height, m_rtViewport.Width(), m_rtViewport.Height());
	else
		m_rt = m_rtViewport;

	steps = m_TransitionDuration *  mPicturesPerSecond;
	w = (double)(m_rt.Width()) / steps;
	h = (double)(m_rt.Height()) / steps;

	trantime = 1000 / mPicturesPerSecond;

	switch (direction)
	{
	default:
	case 0:
		pr.left = m_rt.left;
		pr.top = m_rt.top;

		for (i = 1; i<steps; i++)
		{
			offy = (int)(h*i);
			offx = (int)(w*i);

			pr.right = pr.left + offx;
			pr.bottom = pr.top + offy;

			if (offy > 0 && offx > 0)
			{
				EnterCriticalSection(&m_critial);

				if (this->initPictureEveryTime) {
					testInit();
					if (m_DisplayMode)
						calculate_display_rect(&m_rt, frame->mImg_Width, frame->mImg_Height, m_rtViewport.Width(), m_rtViewport.Height());
					else
						m_rt = m_rtViewport;
				}

				m_pDirect3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
				m_pDirect3DDevice->BeginScene();
				IDirect3DSurface9 * pBackBuffer = NULL;
				m_pDirect3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
				m_pDirect3DDevice->StretchRect(m_pDirect3DSurfaceRender, NULL, pBackBuffer, &pr, D3DTEXF_LINEAR);
				m_pDirect3DDevice->EndScene();
				m_pDirect3DDevice->Present(&pr, &pr, NULL, NULL);
				LeaveCriticalSection(&m_critial);
			}

			Sleep(trantime);
		}
		break;
	case 1:
		pr.left = m_rt.left;
		pr.bottom = m_rt.bottom;

		for (i = 0; i<steps; i++)
		{
			offy = (int)(h*i);
			offx = (int)(w*i);

			pr.right = pr.left + offx;
			pr.top = pr.bottom - offy;

			if (offy > 0 && offx > 0)
			{
				EnterCriticalSection(&m_critial);

				if (this->initPictureEveryTime) {
					testInit();
					if (m_DisplayMode)
						calculate_display_rect(&m_rt, frame->mImg_Width, frame->mImg_Height, m_rtViewport.Width(), m_rtViewport.Height());
					else
						m_rt = m_rtViewport;
				}

				m_pDirect3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
				m_pDirect3DDevice->BeginScene();
				IDirect3DSurface9 * pBackBuffer = NULL;
				m_pDirect3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
				m_pDirect3DDevice->StretchRect(m_pDirect3DSurfaceRender, NULL, pBackBuffer, &pr, D3DTEXF_LINEAR);
				m_pDirect3DDevice->EndScene();
				m_pDirect3DDevice->Present(&pr, &pr, NULL, NULL);
				LeaveCriticalSection(&m_critial);
			}

			Sleep(trantime);
		}
		break;
	case 2:
		pr.right = m_rt.right;
		pr.bottom = m_rt.bottom;

		for (i = 1; i<steps; i++)
		{
			offy = (int)(h*i);
			offx = (int)(w*i);

			pr.left = pr.right - offx;
			pr.top = pr.bottom - offy;

			if (offy > 0 && offx > 0)
			{
				EnterCriticalSection(&m_critial);

				if (this->initPictureEveryTime) {
					testInit();
					if (m_DisplayMode)
						calculate_display_rect(&m_rt, frame->mImg_Width, frame->mImg_Height, m_rtViewport.Width(), m_rtViewport.Height());
					else
						m_rt = m_rtViewport;
				}

				m_pDirect3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
				m_pDirect3DDevice->BeginScene();
				IDirect3DSurface9 * pBackBuffer = NULL;
				m_pDirect3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
				m_pDirect3DDevice->StretchRect(m_pDirect3DSurfaceRender, NULL, pBackBuffer, &pr, D3DTEXF_LINEAR);
				m_pDirect3DDevice->EndScene();
				m_pDirect3DDevice->Present(&pr, &pr, NULL, NULL);
				LeaveCriticalSection(&m_critial);
			}

			Sleep(trantime);
		}
		break;
	case 3:
		pr.right = m_rt.right;
		pr.top = m_rt.top;

		for (i = 0; i<steps; i++)
		{
			offy = (int)(h*i);
			offx = (int)(w*i);

			pr.left = pr.right - offx;
			pr.bottom = pr.top + offy;

			if (offy > 0 && offx > 0)
			{
				EnterCriticalSection(&m_critial);

				if (this->initPictureEveryTime) {
					testInit();
					if (m_DisplayMode)
						calculate_display_rect(&m_rt, frame->mImg_Width, frame->mImg_Height, m_rtViewport.Width(), m_rtViewport.Height());
					else
						m_rt = m_rtViewport;
				}

				m_pDirect3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
				m_pDirect3DDevice->BeginScene();
				IDirect3DSurface9 * pBackBuffer = NULL;
				m_pDirect3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
				m_pDirect3DDevice->StretchRect(m_pDirect3DSurfaceRender, NULL, pBackBuffer, &pr, D3DTEXF_LINEAR);
				m_pDirect3DDevice->EndScene();
				m_pDirect3DDevice->Present(&pr, &pr, NULL, NULL);
				LeaveCriticalSection(&m_critial);
			}

			Sleep(trantime);
		}
		break;
	}
	m_pDirect3DDevice->Present(&m_rt, &m_rt, NULL, NULL);
}


void CSpecialEffectD3DShower::ShowSurfaceRandBlock(hvframe * frame)
{
	int i, j;
	int w, h;
	int rd;
	bool matrix[10][10] = { false };
	float blockw, blockh;
	CRect pr;
	int number = 0;
	int trantime;

	CRect m_rt;

	if (m_DisplayMode)
		calculate_display_rect(&m_rt, frame->mImg_Width, frame->mImg_Height, m_rtViewport.Width(), m_rtViewport.Height());
	else
		m_rt = m_rtViewport;

	trantime = m_TransitionDuration * 10;

	w = m_rt.Width();
	h = m_rt.Height();

	blockw = (float)w / 10;
	blockh = (float)h / 10;

	srand((unsigned int)time(0));

	EnterCriticalSection(&m_critial);

	if (this->initPictureEveryTime) {
		testInit();
		if (m_DisplayMode)
			calculate_display_rect(&m_rt, frame->mImg_Width, frame->mImg_Height, m_rtViewport.Width(), m_rtViewport.Height());
		else
			m_rt = m_rtViewport;
	}

	m_pDirect3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	m_pDirect3DDevice->BeginScene();
	IDirect3DSurface9 * pBackBuffer = NULL;
	m_pDirect3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
	m_pDirect3DDevice->StretchRect(m_pDirect3DSurfaceRender, NULL, pBackBuffer, &m_rt, D3DTEXF_LINEAR);
	m_pDirect3DDevice->EndScene();

	while (1)
	{
		rd = rand() % 100;
		i = rd / 10;
		j = rd % 10;
		if (!matrix[i][j])
		{
			number++;
			matrix[i][j] = true;
			pr.left = m_rt.left + (int)(j*blockw);
			pr.right = m_rt.left + (int)((j + 1)*blockw);
			pr.top = m_rt.top + (int)(i*blockh);
			pr.bottom = m_rt.top + (int)((i + 1)*blockh);

			m_pDirect3DDevice->Present(&pr, &pr, NULL, NULL);

			Sleep(trantime);
		}

		if (number>99)	break;
	}
	m_pDirect3DDevice->Present(&m_rt, &m_rt, NULL, NULL);
	LeaveCriticalSection(&m_critial);
}


void CSpecialEffectD3DShower::ShowSurfacePullCurtain(hvframe * frame)
{
	int length;
	int steps;
	int x, w;
	double h;
	CRect pr;
	int i;
	int trantime;

	CRect m_rt;

	if (m_DisplayMode)
		calculate_display_rect(&m_rt, frame->mImg_Width, frame->mImg_Height, m_rtViewport.Width(), m_rtViewport.Height());
	else
		m_rt = m_rtViewport;

	steps = m_TransitionDuration *  mPicturesPerSecond;
	trantime = 1000 / mPicturesPerSecond;

	pr.top = m_rt.top;
	pr.bottom = m_rt.bottom;

	w = m_rt.Width() / 2;
	x = m_rt.left + w;
	h = (double)w / steps;

	EnterCriticalSection(&m_critial);

	if (this->initPictureEveryTime) {
		testInit();
		if (m_DisplayMode)
			calculate_display_rect(&m_rt, frame->mImg_Width, frame->mImg_Height, m_rtViewport.Width(), m_rtViewport.Height());
		else
			m_rt = m_rtViewport;
	}

	m_pDirect3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	m_pDirect3DDevice->BeginScene();
	IDirect3DSurface9 * pBackBuffer = NULL;
	m_pDirect3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
	m_pDirect3DDevice->StretchRect(m_pDirect3DSurfaceRender, NULL, pBackBuffer, &m_rt, D3DTEXF_LINEAR);
	m_pDirect3DDevice->EndScene();

	for (i = 0; i<steps; i++)
	{
		length = (int)(i*h);
		pr.left = x - length;
		pr.right = x + length;

		if (length)
			m_pDirect3DDevice->Present(&pr, &pr, NULL, NULL);

		Sleep(trantime);
	}
	m_pDirect3DDevice->Present(&m_rt, &m_rt, NULL, NULL);
	LeaveCriticalSection(&m_critial);
}


void CSpecialEffectD3DShower::ShowSurfaceVerticalBlind(int direction, hvframe * frame)
{
	int i, j, width, x[10], w;
	CRect pr[10];
	int trantime;

	CRect m_rt;

	if (m_DisplayMode)
		calculate_display_rect(&m_rt, frame->mImg_Width, frame->mImg_Height, m_rtViewport.Width(), m_rtViewport.Height());
	else
		m_rt = m_rtViewport;

	width = m_rt.Width() / 10;
	trantime = m_TransitionDuration * 100;
	w = width / 10;

	EnterCriticalSection(&m_critial);

	if (this->initPictureEveryTime) {
		testInit();
		if (m_DisplayMode)
			calculate_display_rect(&m_rt, frame->mImg_Width, frame->mImg_Height, m_rtViewport.Width(), m_rtViewport.Height());
		else
			m_rt = m_rtViewport;
	}

	for (i = 0; i<10; i++)
	{
		if (direction)
		{
			if (i == 9)
				x[i] = m_rt.right;
			else
				x[i] = (i + 1)*width;
		}
		else
			x[i] = i*width;
		pr[i].top = m_rt.top;
		pr[i].bottom = m_rt.bottom;
	}

	m_pDirect3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	m_pDirect3DDevice->BeginScene();
	IDirect3DSurface9 * pBackBuffer = NULL;
	m_pDirect3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
	m_pDirect3DDevice->StretchRect(m_pDirect3DSurfaceRender, NULL, pBackBuffer, &m_rt, D3DTEXF_LINEAR);
	m_pDirect3DDevice->EndScene();

	for (i = 0; i<10; i++)
	{
		for (j = 0; j<10; j++)
		{
			if (direction)
			{
				pr[j].left = x[j] - i*w;
				pr[j].right = x[j];
			}
			else
			{
				pr[j].left = x[j];
				pr[j].right = x[j] + i*w;
			}
			m_pDirect3DDevice->Present(&pr[j], &pr[j], NULL, NULL);
		}

		Sleep(trantime);
	}
	m_pDirect3DDevice->Present(&m_rt, &m_rt, NULL, NULL);
	LeaveCriticalSection(&m_critial);
}

void CSpecialEffectD3DShower::testEffect(hvframe *frame){
	CRect m_rt;

	if (m_DisplayMode)
		calculate_display_rect(&m_rt, frame->mImg_Width, frame->mImg_Height, m_rtViewport.Width(), m_rtViewport.Height());
	else
		m_rt = m_rtViewport;

	m_pDirect3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);
	m_pDirect3DDevice->BeginScene();
	m_pDirect3DDevice->EndScene();
	m_pDirect3DDevice->Present(NULL, NULL, NULL, NULL);
	LeaveCriticalSection(&m_critial);
}

void CSpecialEffectD3DShower::testTextureShower(int mode,hvframe *frame) 
{
	CD3DShower m_pTextureShower;
	CRect m_rt;

	if (m_DisplayMode)
		calculate_display_rect(&m_rt, frame->mImg_Width, frame->mImg_Height, m_rtViewport.Width(), m_rtViewport.Height());
	else
		m_rt = m_rtViewport;

	EnterCriticalSection(&m_critial);

	if (this->initPictureEveryTime) {
		testInit();
		/*
		if (m_DisplayMode)
			calculate_display_rect(&m_rt, frame->mImg_Width, frame->mImg_Height, m_rtViewport.Width(), m_rtViewport.Height());
		else
			m_rt = m_rtViewport;*/
	}
	m_pTextureShower.InitD3D_texture(mHand, m_rt.Width(), m_rt.Height(),mode);

	m_pTextureShower.RenderTexture(frame, m_rt.Width(), m_rt.Height());



	m_pDirect3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);
	m_pDirect3DDevice->BeginScene();
	m_pDirect3DDevice->EndScene();


	m_pTextureShower.Cleanup();
	LeaveCriticalSection(&m_critial);
}