#pragma once
#include "d3dshower.h"
#include "D3D9TextureShower.h"
class CSpecialEffectD3DShower :
	public CD3DShower
{
public:
	CSpecialEffectD3DShower(void);
	~CSpecialEffectD3DShower(void);
	bool Render(hvframe * frame);
	bool ConstructSurface(hvframe * frame);
	void ShowSurfaceNormal(hvframe * frame);
	void ShowSurfaceScan(int direction, hvframe * frame);
	void ShowSurfaceSlide(int direction, hvframe * frame);
	void ShowSurfaceButt(int direction, hvframe * frame);
	void ShowSurfaceCenterFull(hvframe * frame);
	void ShowSurfaceEnlarge(hvframe * frame);
	void ShowSurfaceDiagonalFull(int direction, hvframe * frame);
	void ShowSurfaceDiagonalEnlarge(int direction, hvframe * frame);
	void ShowSurfaceRandBlock(hvframe * frame);
	void ShowSurfacePullCurtain(hvframe * frame);
	void ShowSurfaceVerticalBlind(int direction,hvframe * frame);
	void testEffect(hvframe *frame);
	void testInit();
	void testTextureShower(int,hvframe *frame);
private:
	int mPicturesPerSecond = 60;
public:
	int m_TransitionMode = 0;
	int m_TransitionDuration = 0;

	//test
	bool initPictureEveryTime=false;
};

