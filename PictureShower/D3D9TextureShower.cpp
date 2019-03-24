#include "stdafx.h"

/*

#include "stdafx.h"
#include "D3D9TextureShower.h"
#include "D3DShower.h"


CD3D9TextureShower::CD3D9TextureShower()
{

}


CD3D9TextureShower::~CD3D9TextureShower()
{
	Cleanup();
}


void CD3D9TextureShower::Cleanup()
{
	EnterCriticalSection(&m_critial);
	if (m_pDirect3DVertexBuffer)
		m_pDirect3DVertexBuffer->Release();
	if (m_pDirect3DTexture)
		m_pDirect3DTexture->Release();
	if (m_pDirect3DDevice)
		m_pDirect3DDevice->Release();
	if (m_pDirect3D9)
		m_pDirect3D9->Release();

	LeaveCriticalSection(&m_critial);
}


int CD3D9TextureShower::InitD3D(HWND hwnd, unsigned long lWidth, unsigned long lHeight,int mode)
{
	HRESULT lRet;
	InitializeCriticalSection(&m_critial);

	Cleanup();
	EnterCriticalSection(&m_critial);
	// Create IDirect3D
	m_pDirect3D9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pDirect3D9 == NULL)
	{
		LeaveCriticalSection(&m_critial);
		return -1;
	}

	if (lWidth == 0 || lHeight == 0)
	{
		RECT rt;
		GetClientRect(hwnd, &rt);
		lWidth = rt.right - rt.left;
		lHeight = rt.bottom - rt.top;
	}


	//D3DPRESENT_PARAMETERS Describes the presentation parameters.
	//定义后台缓存表面的参数
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = lWidth;
	d3dpp.BackBufferHeight = lHeight;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	//d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;

	/*指定系统如何将后台缓冲区的内容复制到前台缓冲区，从而在屏幕上显示。它的值有：
	D3DSWAPEFFECT_DISCARD:清除后台缓存的内容
	D3DSWAPEEFECT_FLIP:保留后台缓存的内容。当缓存区>1时。
	D3DSWAPEFFECT_COPY: 保留后台缓存的内容，缓冲区=1时。
	一般情况下使用D3DSWAPEFFECT_DISCAR



	d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
	d3dpp.hDeviceWindow = hwnd;
	d3dpp.Windowed = TRUE;

	//指定是否需要DirectX自动管理深度缓冲（用于关于每个像素点的深度信息）
	d3dpp.EnableAutoDepthStencil = FALSE;

	//向驱动程序提示后缓冲区将包含视频数据。
	d3dpp.Flags = D3DPRESENTFLAG_VIDEO;
	//指定后备缓冲区与前台缓冲区的交换频率.窗口模式下必须使用D3DPRESENT_INTERVAL_DEFAULT。
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;



	m_hVideoWnd = hwnd;


	lRet = m_pDirect3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, NULL,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &d3dpp, &m_pDirect3DDevice);

	lRet = m_pDirect3DDevice->CreateTexture(lWidth, lHeight, 1, D3DUSAGE_SOFTWAREPROCESSING,
		D3DFMT_X8R8G8B8,
		D3DPOOL_MANAGED,
		&m_pDirect3DTexture, NULL);


	if (FAILED(lRet))
	{
		LeaveCriticalSection(&m_critial);
		return -1;
	}
	// Create Vertex Buffer
	lRet = m_pDirect3DDevice->CreateVertexBuffer(4 * sizeof(CUSTOMVERTEX),
		0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &m_pDirect3DVertexBuffer, NULL);
	if (FAILED(lRet))
	{
		LeaveCriticalSection(&m_critial);
		return -1;
	}

	// -0.5f is a "feature" of DirectX and it seems to apply to Direct3d also 
	CUSTOMVERTEX vertices[] = {
		{ -0.5f, -0.5f, 0.0f, 1.0f, D3DCOLOR_ARGB(255, 255, 255, 255), 0.0f, 0.0f },
		{ lWidth - 0.5f, -0.5f, 0.0f, 1.0f, D3DCOLOR_ARGB(255, 255, 255, 255), 1.0f, 0.0f },
		{ lWidth - 0.5f, lHeight - 0.5f, 0.0f, 1.0f, D3DCOLOR_ARGB(255, 255, 255, 255), 1.0f, 1.0f },
		{ -0.5f, lHeight - 0.5f, 0.0f, 1.0f, D3DCOLOR_ARGB(255, 255, 255, 255), 0.0f, 1.0f }
	};

	//Rotate Texture?
	CUSTOMVERTEX vertices_Rotate[] = {
		{ lWidth / 4 - 0.5f, -0.5f, 0.0f, 1.0f, D3DCOLOR_ARGB(255, 255, 255, 255), 0.0f, 0.0f },
		{ lWidth - 0.5f, lHeight / 4 - 0.5f, 0.0f, 1.0f, D3DCOLOR_ARGB(255, 255, 255, 255), 1.0f, 0.0f },
		{ lWidth * 3 / 4 - 0.5f, lHeight - 0.5f, 0.0f, 1.0f, D3DCOLOR_ARGB(255, 255, 255, 255), 1.0f, 1.0f },
		{ -0.5f, lHeight * 3 / 4 - 0.5f, 0.0f, 1.0f, D3DCOLOR_ARGB(255, 255, 255, 255), 0.0f, 1.0f }
	};

	//half texture
	CUSTOMVERTEX vertices_half[] = {
		{ -0.5f, -0.5f, 0.0f, 1.0f, D3DCOLOR_ARGB(255, 255, 255, 255), 0.0f, 0.0f },
		{ lWidth - 0.5f, -0.5f, 0.0f, 1.0f, D3DCOLOR_ARGB(255, 255, 255, 255), 0.5f, 0.0f },
		{ lWidth - 0.5f, lHeight - 0.5f, 0.0f, 1.0f, D3DCOLOR_ARGB(255, 255, 255, 255), 0.5f, 1.0f },
		{ -0.5f, lHeight - 0.5f, 0.0f, 1.0f, D3DCOLOR_ARGB(255, 255, 255, 255), 0.0f, 1.0f }
	};


	// Fill Vertex Buffer
	CUSTOMVERTEX *pVertex;
	lRet = m_pDirect3DVertexBuffer->Lock(0, 4 * sizeof(CUSTOMVERTEX), (void**)&pVertex, 0);
	if (FAILED(lRet)){
		LeaveCriticalSection(&m_critial);
		return -1;
	}
	if (mode == 0)
	{
		memcpy(pVertex, vertices, sizeof(vertices));
	}
	if (mode == 1)
	{
		memcpy(pVertex, vertices_Rotate, sizeof(vertices_Rotate));
	}
	if (mode == 2)
	{
		memcpy(pVertex, vertices_half, sizeof(vertices_half));
	}


	m_pDirect3DVertexBuffer->Unlock();
	LeaveCriticalSection(&m_critial);

	return 0;
}


bool CD3D9TextureShower::Render(hvframe * frame)
{
	LRESULT lRet;

	if (m_pDirect3DDevice == NULL)
		return false;
	
	D3DLOCKED_RECT d3d_rect;
	//Locks a rectangle on a texture resource.
	//And then we can manipulate pixel data in it.
	lRet = m_pDirect3DTexture->LockRect(0, &d3d_rect, 0, 0);
	if (FAILED(lRet)){
		return false;
	}
	// Copy pixel data to texture
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

	m_pDirect3DTexture->UnlockRect(0);

	//Clears one or more surfaces
	lRet = m_pDirect3DDevice->Clear(0, NULL, D3DCLEAR_TARGET,
		D3DCOLOR_XRGB(0, 255, 0), 1.0f, 0);

	//Begin the scene
	if (FAILED(m_pDirect3DDevice->BeginScene())){
		return false;
	}

	lRet = m_pDirect3DDevice->SetTexture(0, m_pDirect3DTexture);

	//Binds a vertex buffer to a device data stream.
	m_pDirect3DDevice->SetStreamSource(0, m_pDirect3DVertexBuffer,
		0, sizeof(CUSTOMVERTEX));

	//Sets the current vertex stream declaration.
	lRet = m_pDirect3DDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

	//Renders a sequence of nonindexed, geometric primitives of the 
	//specified type from the current set of data input streams.
	m_pDirect3DDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
	m_pDirect3DDevice->EndScene();

	//Presents the contents of the next buffer in the sequence of back 
	//buffers owned by the device.
	m_pDirect3DDevice->Present(NULL, NULL, NULL, NULL);

	return true;
}


*/