#pragma once

#include "stdafx.h"
#include <d3d9.h>
#include "d3dx9.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "libavutil/frame.h"
#ifdef __cplusplus
} // endof extern "C"
#endif

typedef struct tagHVFrame
{
    int              mScreen_Height = 0;
	int              mScreen_Width = 0;
    unsigned char *  mSrcData = NULL;
	int              mLineSize = 0;
    int				 mImg_Width = 0;
    int              mImg_Height = 0;
	AVFrame       *  pAVFrame;
} hvframe;

class CD3DShower
{
public:
	HWND mHand;
	CRect m_rtViewport;
	int m_DisplayMode = 12;

	D3DPRESENT_PARAMETERS d3dpp;
	CRITICAL_SECTION  m_critial;

	IDirect3D9 *m_pDirect3D9 = NULL;
	IDirect3DDevice9 *m_pDirect3DDevice = NULL;
	IDirect3DSurface9 *m_pDirect3DSurfaceRender = NULL;
public:
	CD3DShower(void);
	~CD3DShower(void);
	void Cleanup(void);
	int InitD3D(HWND hwnd);
	virtual bool Render(hvframe * frame);
	unsigned long GetWidth();
	unsigned long GetHight();
	void calculate_display_rect(CRect *rect, int img_width, int img_height, int scr_width, int scr_height);
};

