#pragma once
#include <Windows.h>
#include <mmsystem.h>
#include <fstream>
#include <iostream>
#include "speech.h"

#pragma comment(lib, "winmm.lib")

class Voice {
public:
	HWAVEIN hWaveIn;  //�����豸
	WAVEFORMATEX waveform; //����Ƶ�ĸ�ʽ���ṹ��
	BYTE *pBuffer1;//����Ƶʱ�����ݻ���
	WAVEHDR wHdr1; //����Ƶʱ�������ݻ���Ľṹ��
	FILE *pf;

	std::string ASR(aip::Speech* client);
	int record();
	std::string recognize();
	std::string transfer2Chinese(std::string unicode);
};