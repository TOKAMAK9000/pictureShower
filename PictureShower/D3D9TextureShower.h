#pragma once
#include <tchar.h>
#include <d3d9.h>
#include "D3DShower.h"

//Flexible Vertex Format, FVF
typedef struct
{
	FLOAT       x, y, z;     // vertex untransformed position
	FLOAT       rhw;         // eye distance
	D3DCOLOR    diffuse;     // diffuse color
	FLOAT       tu, tv;      // texture relative coordinates
} CUSTOMVERTEX;

// Custom flexible vertex format (FVF), which describes custom vertex structure
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

class CD3D9TextureShower
{
public:
	CRITICAL_SECTION  m_critial;
	HWND     m_hVideoWnd;  // ÊÓÆµ´°¿Ú

	IDirect3D9 *m_pDirect3D9 = NULL;
	IDirect3DDevice9 *m_pDirect3DDevice = NULL;
	IDirect3DTexture9 *m_pDirect3DTexture = NULL;
	IDirect3DVertexBuffer9 *m_pDirect3DVertexBuffer = NULL;

public:
	CD3D9TextureShower();
	~CD3D9TextureShower();
	void Cleanup();
	int InitD3D(HWND hwnd, unsigned long lWidth, unsigned long lHeight,int mode);
	bool Render(hvframe * frame);
};

